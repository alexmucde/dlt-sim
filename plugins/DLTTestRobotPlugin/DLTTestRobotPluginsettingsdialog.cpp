#include "DLTTestRobotPluginsettingsdialog.h"
#include "ui_DLTTestRobotPluginsettingsdialog.h"

#include <QFileDialog>

DLTTestRobotPluginSettingsDialog::DLTTestRobotPluginSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DLTTestRobotPluginSettingsDialog)
{
    ui->setupUi(this);
}

DLTTestRobotPluginSettingsDialog::~DLTTestRobotPluginSettingsDialog()
{
    delete ui;
}

void DLTTestRobotPluginSettingsDialog::restoreSettings(DLTTestRobotPlugin *plugin)
{
    ui->lineEditDLTViewerTestRobotPluginPort->setText(QString("%1").arg(plugin->getDltViewerTestRobotPluginPort()));
    ui->lineEditAutoloadTestsFilename->setText(plugin->getAutoloadTestFilename());
    ui->checkBoxAutoloadTests->setChecked(plugin->getAutoloadTests());
    ui->checkBoxAutostartTests->setChecked(plugin->getAutostartTests());
}

void DLTTestRobotPluginSettingsDialog::backupSettings(DLTTestRobotPlugin *plugin)
{
    plugin->setDltViewerTestRobotPluginPort(ui->lineEditDLTViewerTestRobotPluginPort->text().toUInt());
    plugin->setAutoloadTestFilename(ui->lineEditAutoloadTestsFilename->text());
    plugin->setAutoloadTests(ui->checkBoxAutoloadTests->checkState());
    plugin->setAutostartTests(ui->checkBoxAutostartTests->checkState());
}

void DLTTestRobotPluginSettingsDialog::on_pushButtonAutoloadtestsFilenameSelect_clicked()
{
    QString projectFileName = QFileDialog::getOpenFileName(this,
        tr("Open Test Configuration"), "", tr("DLT Test Robot Configuration (*.dtr);;All files (*.*)"));

    if(projectFileName.isEmpty())
    {
        return;
    }

    ui->lineEditAutoloadTestsFilename->setText(projectFileName);

}

