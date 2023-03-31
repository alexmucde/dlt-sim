#include "DLTTestRobotPluginform.h"
#include "ui_DLTTestRobotPluginform.h"
#include "DLTTestRobotPlugin.h"
#include "dltminiserver.h"

#include <QFileDialog>
#include <QMessageBox>

DLTTestRobotPluginForm::DLTTestRobotPluginForm(DLTTestRobotPlugin *newPlugin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DLTTestRobotPluginForm)
{
    plugin = newPlugin;

    ui->setupUi(this);

    ui->pushButtonStopTests->setEnabled(false);

    if(plugin->getAutoloadTests())
    {
        ui->lineEditTestsFilename->setText(plugin->getAutoloadTestFilename());
        loadTests(plugin->getAutoloadTestFilename());
    }

    if(plugin->getAutostartTests())
    {
        startTests();
    }
}

DLTTestRobotPluginForm::~DLTTestRobotPluginForm()
{
    plugin->backupGeometry();

    delete ui;
}

void DLTTestRobotPluginForm::on_pushButtonTestFilename_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Tests"), "", tr("DLTTestRobot Tests (*.dtr);;All files (*.*)"));

    if(fileName.isEmpty())
    {
        return;
    }

    loadTests(fileName);

}


void DLTTestRobotPluginForm::on_pushButtonStartTests_clicked()
{
   startTests();

}


void DLTTestRobotPluginForm::on_pushButtonStopTests_clicked()
{
    plugin->stopTest();

    if(plugin->dltMiniServer)
        plugin->dltMiniServer->sendValue(plugin->getDltAppId(),plugin->getDltCtxId(),"test stopped ",DLT_LOG_FATAL);
}

void DLTTestRobotPluginForm::startTests()
{
    // update Command Number
    ui->lineEditCmdNo->setText(QString("%1/%2").arg(0).arg(plugin->testSize(ui->comboBoxCurrentTestName->currentIndex())));

    // create and write to report
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();
    if(reportFile.isOpen())
        reportFile.close();
    reportSummaryList.clear();
    reportFailedCounter=0;
    reportSuccessCounter=0;
    if(ui->checkBoxRunAllTest->isChecked())
        reportFile.setFileName(date.toString("yyyyMMdd_")+time.toString("HHmmss_")+QFileInfo(plugin->getTestsFilename()).baseName()+"_TestReport.txt");
    else
        reportFile.setFileName(date.toString("yyyyMMdd_")+time.toString("HHmmss_")+plugin->testId(ui->comboBoxCurrentTestName->currentIndex())+"_TestReport.txt");
    reportFile.open(QIODevice::WriteOnly | QIODevice::Text);
    reportFile.write(QString("Starting tests at %1 %2\n").arg(date.toString("dd.MM.yyyy")).arg(time.toString("HH:mm:ss")).toLatin1());
    reportFile.write(QString("\nTest File: %1\n").arg(plugin->getTestsFilename()).toLatin1());
    reportFile.write(QString("\nTest Version: %1\n\n").arg(plugin->getVersion()).toLatin1());
    reportFile.flush();

    // open new DLt file and connect all ECUs
    if(ui->checkBoxRunAllTest->isChecked())
        plugin->send(QString("newFile ")+QDir::currentPath()+date.toString("\\yyyyMMdd_")+time.toString("HHmmss_")+QFileInfo(plugin->getTestsFilename()).baseName()+"_Logs.dlt");
    else
        plugin->send(QString("newFile ")+QDir::currentPath()+date.toString("\\yyyyMMdd_")+time.toString("HHmmss_")+plugin->testId(ui->comboBoxCurrentTestName->currentIndex())+"_Logs.dlt");
    plugin->send(QString("connectAllEcu"));

    // update UI
    ui->pushButtonStartTests->setEnabled(false);
    ui->pushButtonStopTests->setEnabled(true);
    ui->pushButtonTestFilename->setEnabled(false);
    ui->comboBoxCurrentTestName->setEnabled(false);
    ui->checkBoxRunAllTest->setEnabled(false);
    ui->spinBoxRepeat->setEnabled(false);
    //ui->pushButtonStop->setEnabled(false);

    // start the tests and write info to log
    if(plugin->dltMiniServer)
            plugin->dltMiniServer->sendValue2(plugin->getDltAppId(),plugin->getDltCtxId(),"Tests start",QFileInfo(plugin->getTestsFilename()).baseName());
    if(ui->checkBoxRunAllTest->isChecked())
        plugin->startTest(-1,ui->spinBoxRepeat->text().toInt());
    else
        plugin->startTest(ui->comboBoxCurrentTestName->currentIndex(),ui->spinBoxRepeat->text().toInt());

}

void DLTTestRobotPluginForm::stopTests()
{
    // write summary and close report
    writeSummaryToReport();
    reportFile.close();
}

void DLTTestRobotPluginForm::writeSummaryToReport()
{
    // write summary
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();
    reportFile.write(QString("\nSummary\n").toLatin1());
    for(int num=0;num<reportSummaryList.size();num++)
    {
        reportFile.write((reportSummaryList[num]+"\n").toLatin1());
    }
    reportFile.write(QString("\nSuccess tests: %1\nFailed tests: %2\nFailed test commands: %3\n").arg(reportSuccessCounter).arg(reportFailedCounter).arg(plugin->getFailedTestCommands()).toLatin1());
    reportFile.write(QString("\nTests STOPPED at %1 %2\n").arg(date.toString("dd.MM.yyyy")).arg(time.toString("HH:mm:ss")).toLatin1());
}

void DLTTestRobotPluginForm::loadTests(QString fileName)
{
    ui->lineEditTestsFilename->setText(fileName);

    // read the tests from dtr file
    QStringList errors = plugin->readTests(fileName);

    if(!errors.isEmpty())
    {
        // There are some error during parsing test file
        QMessageBox msgBox(QMessageBox::Critical,"Loading Test Cases","");
        msgBox.setText(("Loading all Test Cases failed with following Errors:\n\n")+errors.join('\n')+("\n\nPlease fix Errors and Reload Test Cases!"));
        msgBox.exec();
    }
    else
    {
        // Loading Test Cases were succesful
        QMessageBox msgBox;
        msgBox.setText(QString("Loading all Test Cases was succesful:\n\nNumber of Test Cases: %1").arg(plugin->size()));
        msgBox.exec();
    }

    ui->comboBoxCurrentTestName->clear();
    for(int num=0;num<plugin->size();num++)
    {
        ui->comboBoxCurrentTestName->addItem(QString("%1 %2 (%3)").arg(plugin->testId(num)).arg(plugin->testSize(num)).arg(plugin->testDescription(num)));
    }
}

void DLTTestRobotPluginForm::command(int allTestRepeatNum,int allTestRepeat, int testRepeatNum,int testRepeat,int testNum, int commandNum,int commandCount, QString text)
{
    ui->lineEditCmdNo->setText(QString("%1/%2").arg(commandNum+1).arg(commandCount));
    ui->lineEditAllRepeatNo->setText(QString("%1/%2").arg(allTestRepeatNum+1).arg(allTestRepeat));
    ui->lineEditTestRepeatNo->setText(QString("%1/%2").arg(testRepeatNum+1).arg(testRepeat));
    ui->lineEditCurrenTest->setText(QString("%1 (%2)").arg(plugin->testId(testNum)).arg(plugin->testDescription(testNum)));
    ui->lineEditFailedTestCommands->setText(QString("%1").arg(plugin->getFailedTestCommands()));

    if(plugin->getFailedTestCommands()>0)
    {
        QPalette palette;
        palette.setColor(QPalette::Base,Qt::red);
        ui->lineEditFailedTestCommands->setPalette(palette);
    }
    else
    {
        QPalette palette;
        palette.setColor(QPalette::Base,Qt::green);
        ui->lineEditFailedTestCommands->setPalette(palette);
    }

    if(text=="started")
    {
        if(plugin->dltMiniServer)
            plugin->dltMiniServer->sendValue2(plugin->getDltAppId(),plugin->getDltCtxId(),"test start",plugin->testId(testNum));
        ui->listWidgetCommands->clear();
        ui->listWidgetCommands->addItems(plugin->getTest(testNum).getCommands());
        ui->listWidgetCommands->setCurrentRow(commandNum);

        // write to report
        QTime time = QTime::currentTime();
        reportFile.write(QString("\n%1 test start %2 (%3/%4)\n").arg(time.toString("HH:mm:ss")).arg(plugin->testId(testNum)).arg(testRepeatNum+1).arg(testRepeat).toLatin1());
        reportFile.flush();
    }
    else if(text=="end success")
    {
        if(plugin->dltMiniServer)
            plugin->dltMiniServer->sendValue2(plugin->getDltAppId(),plugin->getDltCtxId(),"test end success",plugin->testId(testNum));

        // write to report
        QTime time = QTime::currentTime();
        reportFile.write(QString("%1 test end SUCCESS\n").arg(time.toString("HH:mm:ss")).toLatin1());
        reportFile.flush();

        reportSummaryList.append(QString("SUCCESS %1 (%2/%3)").arg(plugin->testId(testNum)).arg(testRepeatNum+1).arg(testRepeat));
        reportSuccessCounter++;
    }
    else if(text=="end")
    {
        if(plugin->dltMiniServer)
            plugin->dltMiniServer->sendValue2(plugin->getDltAppId(),plugin->getDltCtxId(),"Tests end",QFileInfo(plugin->getTestsFilename()).baseName());

        stopTests();

    }
    else if(text=="failed")
    {
        if(plugin->dltMiniServer)
            plugin->dltMiniServer->sendValue2(plugin->getDltAppId(),plugin->getDltCtxId(),"test failed",plugin->testId(testNum),DLT_LOG_FATAL);

        // write to report
        QTime time = QTime::currentTime();
        reportFile.write(QString("%1 test FAILED\n").arg(time.toString("HH:mm:ss")).toLatin1());
        reportFile.flush();

        reportSummaryList.append(QString("FAILED %1 (%2/%3)").arg(plugin->testId(testNum)).arg(testRepeatNum+1).arg(testRepeat));
        reportFailedCounter++;
        plugin->send(QString("marker"));

    }
    else if(text=="stopped")
    {
        stopTests();
    }
    else
    {
        ui->listWidgetCommands->setCurrentRow(commandNum);
        if(plugin->dltMiniServer)
            plugin->dltMiniServer->sendValue3(plugin->getDltAppId(),plugin->getDltCtxId(),"test step",QString("%1").arg(commandNum),text);

        // write to report
        QTime time = QTime::currentTime();
        reportFile.write(QString("%1 test step %2 %3\n").arg(time.toString("HH:mm:ss")).arg(commandNum).arg(text).toLatin1());
        reportFile.flush();
    }
}

void DLTTestRobotPluginForm::statusTests(QString text)
{
    // status from Tests

    if(text == "Prerun")
    {
        QPalette palette;
        palette.setColor(QPalette::Base,Qt::green);
        ui->lineEditTestsStatus->setPalette(palette);
        ui->lineEditTestsStatus->setText(text);
    }
    else if(text == "Running")
    {
        QPalette palette;
        palette.setColor(QPalette::Base,Qt::green);
        ui->lineEditTestsStatus->setPalette(palette);
        ui->lineEditTestsStatus->setText(text);
    }
    else if(text == "Postrun")
    {
        QPalette palette;
        palette.setColor(QPalette::Base,Qt::green);
        ui->lineEditTestsStatus->setPalette(palette);
        ui->lineEditTestsStatus->setText(text);
    }
    else if(text == "Finished")
    {
        QPalette palette;
        palette.setColor(QPalette::Base,Qt::green);
        ui->lineEditTestsStatus->setPalette(palette);
        ui->lineEditTestsStatus->setText(text);

        // close DLT file and disconnect ECUs
        plugin->send(QString("disconnectAllEcu"));
        plugin->send(QString("clearFile"));

        // update UI
        ui->pushButtonStartTests->setEnabled(true);
        ui->pushButtonStopTests->setEnabled(false);
        ui->pushButtonTestFilename->setEnabled(true);
        ui->comboBoxCurrentTestName->setEnabled(true);
        ui->checkBoxRunAllTest->setEnabled(true);
        ui->spinBoxRepeat->setEnabled(true);
        //ui->pushButtonStop->setEnabled(true);
    }
}

void DLTTestRobotPluginForm::report(QString text)
{
    // write to report
    QTime time = QTime::currentTime();
    reportFile.write(QString("=> %1 %2\n").arg(time.toString("HH:mm:ss")).arg(text).toLatin1());
}

void DLTTestRobotPluginForm::reportSummary(QString text)
{
    // write to report
    reportSummaryList.append(text);
}
