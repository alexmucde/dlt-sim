#include "DLTCanPluginsettingsdialog.h"
#include "ui_DLTCanPluginsettingsdialog.h"

DLTCanPluginSettingsDialog::DLTCanPluginSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DLTCanPluginSettingsDialog)
{
    ui->setupUi(this);
}

DLTCanPluginSettingsDialog::~DLTCanPluginSettingsDialog()
{
    delete ui;
}

void DLTCanPluginSettingsDialog::restoreSettings(DLTCanPlugin *templatePlugin)
{

}

void DLTCanPluginSettingsDialog::backupSettings(DLTCanPlugin *templatePlugin)
{

}
