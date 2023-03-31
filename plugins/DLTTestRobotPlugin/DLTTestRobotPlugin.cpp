#include "DLTTestRobotPlugin.h"
#include "DLTTestRobotPluginsettingsdialog.h"
#include "DLTTestRobotPluginform.h"

#include <QFile>
#include <QMdiSubWindow>
#include <QSettings>
#include <QRandomGenerator>

DLTTestRobotPlugin::DLTTestRobotPlugin()
{
    form = 0;
    window = 0;
    dltMiniServer = 0;

    clearSettings();
}

DLTTestRobotPlugin::~DLTTestRobotPlugin()
{
    stop();
}

void DLTTestRobotPlugin::clearSettings()
{
    dltViewerTestRobotPluginPort = 4490;
    autoloadTestFilename = "";
    autoloadTests = false;
    autostartTests = false;

    timer.stop();

    testNum = -1;
    commandNum = -1;

    dltAppId = "DLTT";
    dltCtxId = "STAT";
}

void DLTTestRobotPlugin::settings(QWidget *parent)
{
    DLTTestRobotPluginSettingsDialog dlg(parent);
    dlg.restoreSettings(this);

    if(dlg.exec()==QDialog::Accepted)
    {
        dlg.backupSettings(this);
    }
}

void DLTTestRobotPlugin::readSettings(const QString &filename,const int instance)
{
    // read settings from XML file
    bool isPlugin = false;

    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
             return;

    QXmlStreamReader xml(&file);

    while (!xml.atEnd())
    {
          xml.readNext();

          if(xml.isStartElement())
          {
              if(isPlugin)
              {
                  /* Project settings */
                  if(xml.name() == QString("Version"))
                  {
                      xml.readElementText();
                      /* TODO: Check if the version is compatible */
                  }
                  else if(xml.name() == QString("dltViewerTestRobotPluginPort"))
                  {
                      dltViewerTestRobotPluginPort = xml.readElementText().toUInt();
                      /* TODO: Check if the version is compatible */
                  }
                  else if(xml.name() == QString("autoloadTestFilename"))
                  {
                      autoloadTestFilename = xml.readElementText();
                      /* TODO: Check if the version is compatible */
                  }
                  else if(xml.name() == QString("autoloadTests"))
                  {
                      autoloadTests = xml.readElementText().toInt();
                      /* TODO: Check if the version is compatible */
                  }
                  else if(xml.name() == QString("autostartTests"))
                  {
                      autostartTests = xml.readElementText().toInt();
                      /* TODO: Check if the version is compatible */
                  }
              }
              else if(xml.name() == QString("Name"))
              {
                  if( xml.readElementText() == QString(DLT_TEST_ROBOT_PLUGIN_NAME))
                    isPlugin = true;
              }
          }
          else if(xml.isEndElement())
          {
              /* Connection, plugin and filter */
              if(xml.name() == QString("Plugin"))
              {
                    isPlugin = false;
              }
          }
    }
    if (xml.hasError())
    {
         qDebug() << "Error in processing filter file" << filename << xml.errorString();
    }

    file.close();
}

void DLTTestRobotPlugin::writeSettings(QXmlStreamWriter &xml,const int instance) const
{
    // Plugin type and version
    xml.writeTextElement("Name",DLT_TEST_ROBOT_PLUGIN_NAME);
    xml.writeTextElement("Version",DLT_TEST_ROBOT_PLUGIN_VERSION);
    xml.writeTextElement("Instance",QString("%1").arg(instance));

    // Settings
    xml.writeStartElement("Settings");
        xml.writeTextElement("dltViewerTestRobotPluginPort",QString("%1").arg(dltViewerTestRobotPluginPort));
        xml.writeTextElement("autoloadTestFilename",autoloadTestFilename);
        xml.writeTextElement("autoloadTests",QString("%1").arg(autoloadTests));
        xml.writeTextElement("autostartTests",QString("%1").arg(autostartTests));
    xml.writeEndElement(); // Settings
}

void DLTTestRobotPlugin::start(DLTMiniServer *newDltMiniServer)
{
    dltMiniServer = newDltMiniServer;

    qDebug() << "DLTTestRobotPlugin: started";
    pluginStatus = "started";
    emit updateStatus();

    connect(&tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(&tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(&tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(&tcpSocket, SIGNAL(hostFound()), this, SLOT(hostFound()));
    connect(&tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

    pluginStatus = "connect";
    emit updateStatus();

    tcpSocket.connectToHost("localhost",4490);

    // if it is already connected set connected state
    if(tcpSocket.state()==QAbstractSocket::ConnectedState)
    {
        pluginStatus = "connected";
        emit updateStatus();
    }

    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void DLTTestRobotPlugin::stop()
{    
    tcpSocket.close();

    disconnect(&tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    disconnect(&tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    disconnect(&tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    disconnect(&tcpSocket, SIGNAL(hostFound()), this, SLOT(hostFound()));
    disconnect(&tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

    timer.stop();
    disconnect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));

    testNum = -1;
    commandNum = -1;

    qDebug() << "DLTTestRobotPlugin: stopped";
    pluginStatus = "stopped";
}

void DLTTestRobotPlugin::show(QMdiArea *mdiArea)
{
    if(window == 0)
        window = new QMdiSubWindow;
    if(form == 0)
        form = new DLTTestRobotPluginForm(this,window);
    window->setWidget(form);
    mdiArea->addSubWindow(window);
    window->show();
    restoreGeometry();

    connect(this, SIGNAL(command(int,int,int,int,int,int,int,QString)), form, SLOT(command(int,int,int,int,int,int,int,QString)));
    connect(this, SIGNAL(statusTests(QString)), form, SLOT(statusTests(QString)));
    connect(this, SIGNAL(report(QString)), form, SLOT(report(QString)));
    connect(this, SIGNAL(reportSummary(QString)), form, SLOT(reportSummary(QString)));
}

void DLTTestRobotPlugin::update()
{
    if(form)
        form->update();
}

void DLTTestRobotPlugin::hide()
{
    disconnect(this, SIGNAL(command(int,int,int,int,int,int,int,QString)), form, SLOT(command(int,int,int,int,int,int,int,QString)));

    if(window)
        window->hide();
    delete window;
    window = 0;
    form = 0;
}

void DLTTestRobotPlugin::backupGeometry()
{
    QSettings settings;
    settings.setValue("dlt_test_robot_plugin_geometry", window->saveGeometry());
}

void DLTTestRobotPlugin::restoreGeometry()
{
    QSettings settings;
    window->restoreGeometry(settings.value("dlt_test_robot_plugin_geometry").toByteArray());
}

quint32 DLTTestRobotPlugin::getDltViewerTestRobotPluginPort() const
{
    return dltViewerTestRobotPluginPort;
}

void DLTTestRobotPlugin::setDltViewerTestRobotPluginPort(quint32 newDltViewerTestRobotPluginPort)
{
    dltViewerTestRobotPluginPort = newDltViewerTestRobotPluginPort;
}

const QString &DLTTestRobotPlugin::getAutoloadTestFilename() const
{
    return autoloadTestFilename;
}

void DLTTestRobotPlugin::setAutoloadTestFilename(const QString &newAutoloadTestFilename)
{
    autoloadTestFilename = newAutoloadTestFilename;
}

bool DLTTestRobotPlugin::getAutoloadTests() const
{
    return autoloadTests;
}

void DLTTestRobotPlugin::setAutoloadTests(bool newAutoloadTests)
{
    autoloadTests = newAutoloadTests;
}

bool DLTTestRobotPlugin::getAutostartTests() const
{
    return autostartTests;
}

void DLTTestRobotPlugin::setAutostartTests(bool newAutostartTests)
{
    autostartTests = newAutostartTests;
}

void DLTTestRobotPlugin::injection(QString data)
{

}

void DLTTestRobotPlugin::readyRead()
{
    // data on was received
    while (tcpSocket.canReadLine())
    {
        QString text = QString(tcpSocket.readLine());

        if(text.size()>0 && commandNum!=-1 && testNum!=-1)
        {
            text.chop(1);

            // line is not empty
            //qDebug() << "DltTestRobot: readLine" << text;

            QStringList list = text.split(' ');

            QString currentCommand;
            if(testNum>=0 && testNum<tests.size())
                currentCommand= tests[testNum].at(commandNum);
            QStringList listCommand = currentCommand.split(' ');

            if(listCommand.size()>=7 && list.size()>=5 && listCommand[0]=="find" && listCommand[3]==list[0] && listCommand[4]==list[1] && listCommand[5]==list[2])
            {
                if(listCommand[1]=="equal")
                {
                    listCommand.removeAt(0);
                    listCommand.removeAt(0);
                    listCommand.removeAt(0);
                    listCommand.removeAt(0);
                    listCommand.removeAt(0);
                    listCommand.removeAt(0);
                    //qDebug() << "DltTestRobot: find equal" << listCommand.join(' ');
                    if(text.contains(listCommand.join(' ')))
                    {
                        emit this->report(text);
                        qDebug() << "DltTestRobot: find equal matches";
                        timer.stop();

                        successTestCommand();
                    }
                }
                else if(listCommand[1]=="unequal")
                {
                    listCommand.removeAt(0);
                    listCommand.removeAt(0);
                    listCommand.removeAt(0);
                    listCommand.removeAt(0);
                    listCommand.removeAt(0);
                    listCommand.removeAt(0);
                    //qDebug() << "DltTestRobot: find unequal" << listCommand.join(' ');
                    if(text.contains(listCommand.join(' ')))
                    {
                        emit this->report(text);
                        qDebug() << "DltTestRobot: find unequal matches";
                        timer.stop();

                        failedTestCommand();
                    }
                }
                else if(listCommand[1]=="greater" && listCommand[6]==list[3])
                {
                    float value = list[4].toFloat();
                    float commandValue = listCommand[7].toFloat();
                    //qDebug() << "DltTestRobot: find greater" << commandValue;
                    if(value>commandValue)
                    {
                        emit this->report(list[4]);
                        qDebug() << "DltTestRobot: find greater matches";
                        timer.stop();

                        successTestCommand();
                    }
                }
                else if(listCommand[1]=="smaller" && listCommand[6]==list[3])
                {
                    float value = list[4].toFloat();
                    float commandValue = listCommand[7].toFloat();
                    //qDebug() << "DltTestRobot: find smaller" << commandValue;
                    if(value<commandValue)
                    {
                        emit this->report(list[4]);
                        qDebug() << "DltTestRobot: find smaller matches";
                        timer.stop();

                        successTestCommand();
                    }
                }
            }
            else if(listCommand.size()==7 && list.size()>=5 &&  listCommand[0]=="measure" && listCommand[2]==list[0] && listCommand[3]==list[1] && listCommand[4]==list[2] && listCommand[5]==list[3])
            {
                qDebug() << "DltTestRobot: measure" << listCommand[6] << list[4];
                emit this->report(QString("Measure %1 %2").arg(listCommand[6]).arg(list[4]));
                emit this->reportSummary(QString("Measure %1 %2").arg(listCommand[6]).arg(list[4]));
                timer.stop();

                successTestCommand();
            }
        }
    }

}

void DLTTestRobotPlugin::connected()
{
    pluginStatus = "connected";
    emit updateStatus();

    qDebug() << "DLTTestRobot: connected";
}

void DLTTestRobotPlugin::disconnected()
{
    pluginStatus = "disconnected";
    emit updateStatus();

    qDebug() << "DLTTestRobot: disconnected";

    tcpSocket.connectToHost("localhost",4490);
}

void DLTTestRobotPlugin::hostFound()
{
    qDebug() << "DLTTestRobot: hostFound";
}

void DLTTestRobotPlugin::error(QAbstractSocket::SocketError socketError)
{
    qDebug() << "DLTTestRobot: error" << socketError;

    if(tcpSocket.state()==QAbstractSocket::UnconnectedState)
        tcpSocket.connectToHost("localhost",4490);
}

void DLTTestRobotPlugin::send(QString text)
{
    qDebug() << "DLTTestRobot: send" << text;

    text += "\n";

    tcpSocket.write(text.toLatin1());

}

QStringList DLTTestRobotPlugin::readTests(const QString &filename)
{
    QStringList errors;
    int lineCounter = 0;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "DLTTestRobot: failed to open file" << filename;
        errors.append(QString("Cannot open file!"));
        return errors;
    }

    testsFilename = filename;

    tests.clear();

    DLTTest test;
    bool isTest=false;
    QTextStream in(&file);
    while (!in.atEnd())
    {
       QString line = in.readLine();
       lineCounter++;

       if(line.size()==0)
       {
            // empty line is ignored
       }
       else if(line.at(0)==';')
       {
           // lines starting with ; are comments
           qDebug() << "DLTTestRobot: comment" << line;
       }
       else
       {
           line = line.simplified();
           QStringList list = line.split(' ');
           if(list.size()<1)
           {
               errors.append(QString("ERROR: Missing command!"));
               errors.append(QString("=> line %1: \"%2\"").arg(lineCounter).arg(line));
           }
           else if(list[0]=="test")
           {
               if(list.size()<2)
               {
                   errors.append(QString("ERROR: test command must be at least 2 words!"));
                   errors.append(QString("=> line %1: \"%2\"").arg(lineCounter).arg(line));
               }
               else if(list[1]=="id")
               {
                   if(list.size()!=3)
                   {
                       errors.append(QString("ERROR: test id command must be exactly 3 words!"));
                       errors.append(QString("=> line %1: \"%2\"").arg(lineCounter).arg(line));
                   }
                   else
                   {
                       qDebug() << "DLTTestRobot: id" << list[2];
                       test.setId(list[2]);
                   }
               }
               else if(list[1]=="description")
               {
                   if(list.size()<3)
                   {
                       errors.append(QString("ERROR: test description command must be at least 3 words!"));
                       errors.append(QString("=> line %1: \"%2\"").arg(lineCounter).arg(line));
                   }
                   else
                   {
                       list.removeAt(0);
                       list.removeAt(0);
                       qDebug() << "DLTTestRobot: description" << list.join(' ');
                       test.setDescription(list.join(' '));
                   }
               }
               else if(list[1]=="repeat")
               {
                   if(list.size()!=3)
                   {
                       errors.append(QString("ERROR: test repeat command must be exactly 3 words!"));
                       errors.append(QString("=> line %1: \"%2\"").arg(lineCounter).arg(line));
                   }
                   else
                   {
                       qDebug() << "DLTTestRobot: repeat" << list[2];
                       test.setRepeat(list[2].toInt());
                   }
               }
               else if(list[1]=="fail")
               {
                   if(list.size()!=3)
                   {
                       errors.append(QString("ERROR: test fail command must be exactly 3 words!"));
                       errors.append(QString("=> line %1: \"%2\"").arg(lineCounter).arg(line));
                   }
                   else
                   {
                       if(list[2]!="next" && list[2]!="continue" && list[2]!="stop")
                       {
                           errors.append(QString("ERROR: test fail command is not next, continue or stop!"));
                           errors.append(QString("=> line %1: \"%2\"").arg(lineCounter).arg(line));
                       }
                       else
                       {
                       qDebug() << "DLTTestRobot: fail" << list[2];
                       test.setFail(list[2]);
                       }
                   }
               }
               else if(list[1]=="begin")
               {
                   if(list.size()!=2)
                   {
                       errors.append(QString("ERROR: test begin command must be exactly 2 words!"));
                       errors.append(QString("=> line %1: \"%2\"").arg(lineCounter).arg(line));
                   }
                   else
                   {
                       qDebug() << "DLTTestRobot: begin" << test.getId();
                       isTest = true;
                   }
               }
               else if(list[1]=="end")
               {
                   if(list.size()!=2)
                   {
                       errors.append(QString("ERROR: test end command must be exactly 2 words!"));
                       errors.append(QString("=> line %1: \"%2\"").arg(lineCounter).arg(line));
                   }
                   else
                   {
                       qDebug() << "DLTTestRobot: end" << test.getId();
                       isTest=false;
                       tests.append(test);
                       test.clear();
                   }
               }
               else
               {
                   errors.append(QString("ERROR: Unknown command %1!").arg(list[1]));
                   errors.append(QString("=> line %1: \"%2\"").arg(lineCounter).arg(line));
               }
           }
           else if(list[0]=="version")
           {
                if(list.size()!=2)
                {
                    errors.append(QString("ERROR: Version must be one word!"));
                    errors.append(QString("=> line %1: \"%2\"").arg(lineCounter).arg(line));
                }
                else
                    testsVersion = list[1];
           }
           else if(isTest)
           {
                if(
                    ((list.size()==2 && list[0]=="filter" && list[1]=="clear")) ||
                    ((list.size()==5 && list[0]=="filter" && list[1]=="add")) ||
                    ((list.size()>=3 && list[0]=="injection")) ||
                    (((list.size()==2 || list.size()==3)  && list[0]=="wait")) ||
                    ((list.size()>=7  && list[0]=="find" && (list[1]=="equal" || list[1]=="unequal"))) ||
                    ((list.size()>=8 && list[0]=="find" && (list[1]=="greater" || list[1]=="smaller"))) ||
                    (list.size()>=7  && list[0]=="measure" )

                  )
                {
                    qDebug() << "DLTTestRobot: command" << line;
                    test.append(line);
                }
                else
                {
                    errors.append(QString("ERROR: Unknown command or syntac error!"));
                    errors.append(QString("=> line %1: \"%2\"").arg(lineCounter).arg(line));
                }

           }
       }
    }

    if(isTest)
    {
        errors.append(QString("ERROR: Test end is missing!"));
    }

    file.close();

    return errors;
}

void DLTTestRobotPlugin::startTest(int num,int repeat)
{
    emit statusTests("Prerun");
    timer.start(3000);
    state = Prerun;

    if(repeat<1)
        allTestRepeat = 1;
    else
        allTestRepeat = repeat;
    allTestRepeatNum = 0;

    if(num<0)
    {
        allTests = true;
        testNum = 0;
    }
    else
    {
        allTests = false;
        testNum = num;
    }

    failed = false;
}

void DLTTestRobotPlugin::stopTest()
{
    if(testNum != -1 && commandNum!=-1)
    {
        timer.stop();

        command(allTestRepeatNum,allTestRepeat,testRepeatNum,testRepeat,testNum,commandNum,commandCount,"stopped");

        testNum = -1;
        commandNum = -1;
    }

    emit statusTests("Postrun");
    timer.start(3000);
    state = Postrun;

    qDebug() << "DLTTestRobot: stopped test" ;
}

void DLTTestRobotPlugin::runTest()
{
    while(commandNum<commandCount)
    {

        QString currentCommand = tests[testNum].at(commandNum);

        command(allTestRepeatNum,allTestRepeat,testRepeatNum,testRepeat,testNum,commandNum,commandCount,currentCommand);

        QStringList list = currentCommand.split(' ');

        if(list.size()>=2 && list[0]=="wait")
        {
            if(list.size()==3)
            {
                // Random wait
                uint waitTime = QRandomGenerator::global()->bounded(list[1].toUInt(), list[2].toUInt());
                qDebug() << "DLTTestRobot: start random wait timer" << waitTime;
                timer.start(waitTime);
                emit this->report(QString("Random Wait %1").arg(waitTime));
                emit this->reportSummary(QString("Random Wait %1").arg(waitTime));
            }
            else
                timer.start(list[1].toUInt());
            qDebug() << "DLTTestRobot: start wait timer" << list[1].toUInt();
            return;
        }
        else if(list.size()>=3 && list[0]=="find")
        {
            timer.start(list[2].toUInt());
            qDebug() << "DLTTestRobot: start find timer" << list[2].toUInt();
            return;
        }
        else if(list.size()>=2 && list[0]=="measure")
        {
            timer.start(list[1].toUInt());
            qDebug() << "DLTTestRobot: start measure timer" << list[1].toUInt();
            return;
        }
        else
        {
            send(currentCommand);
        }
        commandNum++;
    }
    // end reached
    if(failed)
        command(allTestRepeatNum,allTestRepeat,testRepeatNum,testRepeat,testNum,commandNum,commandCount,"failed");
    else
        command(allTestRepeatNum,allTestRepeat,testRepeatNum,testRepeat,testNum,commandNum,commandCount,"end success");

    qDebug() << "DLTTestRobot: end test" << tests[testNum].getId();

    nextTest();
}

bool DLTTestRobotPlugin::nextTest()
{
    testRepeatNum++;
    if(testRepeatNum<testRepeat)
    {
        commandCount = tests[testNum].size();
        commandNum = 0;
        failed = false;

        qDebug() << "DLTTestRobot: start test" << tests[testNum].getId();

        command(allTestRepeatNum,allTestRepeat,testRepeatNum,testRepeat,testNum,commandNum,commandCount,"started");

        runTest();
        return true;
    }

    if(allTests)
    {
        testNum++;
        if(testNum<testCount)
        {
            testRepeat = tests[testNum].getRepeat();
            testRepeatNum = 0;

            commandCount = tests[testNum].size();
            commandNum = 0;
            failed = false;

            qDebug() << "DLTTestRobot: start test" << tests[testNum].getId();

            command(allTestRepeatNum,allTestRepeat,testRepeatNum,testRepeat,testNum,commandNum,commandCount,"started");

            runTest();
            return true;
        }
    }

    allTestRepeatNum++;
    if(allTestRepeatNum<allTestRepeat)
    {
        if(allTests)
        {
            testNum = 0;
        }

        testRepeat = tests[testNum].getRepeat();
        testRepeatNum = 0;

        commandCount = tests[testNum].size();
        commandNum = 0;
        failed = false;

        qDebug() << "DLTTestRobot: start test" << tests[testNum].getId();

        command(allTestRepeatNum,allTestRepeat,testRepeatNum,testRepeat,testNum,commandNum,commandCount,"started");

        runTest();
        return true;
    }

    command(allTestRepeatNum,allTestRepeat,testRepeatNum,testRepeat,testNum,commandNum,commandCount,"end");

    testNum = -1;
    commandNum = -1;

    qDebug() << "DLTTestRobot: all tests done" ;

    emit statusTests("Postrun");
    timer.start(3000);
    state = Postrun;

    return false;
}

const QString &DLTTestRobotPlugin::getDltCtxId() const
{
    return dltCtxId;
}

void DLTTestRobotPlugin::setDltCtxId(const QString &newDltCtxId)
{
    dltCtxId = newDltCtxId;
}

const QString &DLTTestRobotPlugin::getDltAppId() const
{
    return dltAppId;
}

void DLTTestRobotPlugin::setDltAppId(const QString &newDltAppId)
{
    dltAppId = newDltAppId;
}

void DLTTestRobotPlugin::timeout()
{
    timer.stop();

    qDebug() << "DLTTestRobot: timer expired";

    if(state==Prerun)
    {
        state = Running;
        emit statusTests("Running");

        testCount = tests.size();

        testRepeat = tests[testNum].getRepeat();
        testRepeatNum = 0;

        commandCount = tests[testNum].size();
        commandNum = 0;

        failedTestCommands = 0;

        qDebug() << "DLTTestRobot: start test" << tests[testNum].getId();

        command(allTestRepeatNum,allTestRepeat,testRepeatNum,testRepeat,testNum,commandNum,commandCount,"started");

        runTest();
    }
    else if(state==Running)
    {
        QString currentCommand = tests[testNum].at(commandNum);
        QStringList list = currentCommand.split(' ');

        if(list.size()>=1 && list[0]!="wait" && list[0]!= "measure")
        {
            if(list[1]=="unequal")
                successTestCommand();
            else
                failedTestCommand();

            return;
        }

        commandNum++;
        runTest();

    }
    else if(state==Postrun)
    {
        state = Finished;
        emit statusTests("Finished");
    }
}

void DLTTestRobotPlugin::successTestCommand()
{
    commandNum++;
    runTest();
}

void DLTTestRobotPlugin::failedTestCommand()
{
    failedTestCommands++;
    failed = true;

    if(tests[testNum].getFail()=="continue")
    {
        // continue with current test job
        //command(allTestRepeatNum,allTestRepeat,testRepeatNum,testRepeat,testNum,commandNum,commandCount,"failed");
        emit report("Command FAILED, but continue");
        qDebug() << "DLTTestRobot: fail continue current test" ;

        commandNum++;
        runTest();
    }
    else if(tests[testNum].getFail()=="stop")
    {
        // stop all tests
        emit report("Command FAILED, stop all tests");
        command(allTestRepeatNum,allTestRepeat,testRepeatNum,testRepeat,testNum,commandNum,commandCount,"failed");
        command(allTestRepeatNum,allTestRepeat,testRepeatNum,testRepeat,testNum,commandNum,commandCount,"end");

        testNum = -1;
        commandNum = -1;

        qDebug() << "DLTTestRobot: fail stop all tests" ;

        emit statusTests("Postrun");
        timer.start(3000);
        state = Postrun;
    }
    else /* tests[testNum].getFail()=="next" */
    {
        // default, run next test
        emit report("Command FAILED, next test");
        command(allTestRepeatNum,allTestRepeat,testRepeatNum,testRepeat,testNum,commandNum,commandCount,"failed");

        qDebug() << "DLTTestRobot: fail run next test";

        nextTest();
    }

}

int DLTTestRobotPlugin::getFailedTestCommands() const
{
    return failedTestCommands;
}

bool DLTTestRobotPlugin::getAllTests() const
{
    return allTests;
}

const QString &DLTTestRobotPlugin::getVersion() const
{
    return testsVersion;
}

const QString &DLTTestRobotPlugin::getTestsFilename() const
{
    return testsFilename;
}

bool DLTTestRobotPlugin::getFailed() const
{
    return failed;
}


