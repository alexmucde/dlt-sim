#include "settings.h"
#include "ui_settings.h"

#include <QSettings>
#include <QFileDialog>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);

    /*  load global settings */
    QSettings settings;
    QString projectFileName = settings.value("autoload/projectFileName").toString();
    ui->lineEditProject->setText(projectFileName);
    bool autoload = settings.value("autoload/checked").toBool();
    ui->groupBoxAutoload->setChecked(autoload);
    bool autostart = settings.value("autostart/checked").toBool();
    ui->checkBoxAutostart->setChecked(autostart);
    quint16 dltPort = settings.value("dlt/port",3491).toUInt();
    ui->lineEditPort->setText(QString("%1").arg(dltPort));
}

SettingsDialog::~SettingsDialog()
{
    QSettings settings;
    settings.setValue("dlt/port",ui->lineEditPort->text().toUInt());

    delete ui;
}

void SettingsDialog::restoreSettings(PluginModel &model)
{

}

void SettingsDialog::backupSettings(PluginModel &model)
{

}

void SettingsDialog::on_pushButtonProject_clicked()
{
    QString projectFileName = QFileDialog::getOpenFileName(this,
        tr("Open Settings"), "", tr("DLTRelais Settings (*.xml);;All files (*.*)"));

    if(projectFileName.isEmpty())
    {
        return;
    }

    ui->lineEditProject->setText(projectFileName);

    QSettings settings;
    settings.setValue("autoload/projectFileName",projectFileName);
}



void SettingsDialog::on_groupBoxAutoload_clicked(bool checked)
{
    QSettings settings;
    settings.setValue("autoload/checked",checked);
}


void SettingsDialog::on_checkBoxAutostart_clicked(bool checked)
{
    QSettings settings;
    settings.setValue("autostart/checked",checked);
}

