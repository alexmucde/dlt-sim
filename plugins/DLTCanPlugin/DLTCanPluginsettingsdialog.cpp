#include "DLTCanPluginsettingsdialog.h"
#include "ui_DLTCanPluginsettingsdialog.h"
#include "DLTCanPlugin.h"

#include <QSerialPortInfo>

DLTCanPluginSettingsDialog::DLTCanPluginSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DLTCanPluginSettingsDialog)
{
    ui->setupUi(this);

    /* update serial ports list */
    QList<QSerialPortInfo> 	availablePorts  = QSerialPortInfo::availablePorts();
    ui->comboBoxSerialPort->clear();
    qDebug() << "portName" << "description" << "manufacturer" << "serialNumber" << "productIdentifier" << "vendorIdentifier" << "systemLocation";
    for(int num = 0; num<availablePorts.length();num++)
    {
        qDebug() << availablePorts[num].portName() << availablePorts[num].description() << availablePorts[num].manufacturer() << availablePorts[num].serialNumber() << availablePorts[num].productIdentifier() << availablePorts[num].vendorIdentifier() << availablePorts[num].systemLocation();
        ui->comboBoxSerialPort->addItem(availablePorts[num].portName());
    }

}

DLTCanPluginSettingsDialog::~DLTCanPluginSettingsDialog()
{
    delete ui;
}

void DLTCanPluginSettingsDialog::restoreSettings(DLTCanPlugin *plugin)
{
    ui->lineEditTitle->setText(plugin->getTitle());
    ui->comboBoxSerialPort->setCurrentText(plugin->getInterface());
}

void DLTCanPluginSettingsDialog::backupSettings(DLTCanPlugin *plugin)
{
    plugin->setTitle(ui->lineEditTitle->text());
    plugin->setInterface(ui->comboBoxSerialPort->currentText());
}
