#include "DLTMultimetersettingsdialog.h"
#include "ui_DLTMultimetersettingsdialog.h"

#include <QSerialPortInfo>

DLTMultimeterSettingsDialog::DLTMultimeterSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DLTMultimeterSettingsDialog)
{
    ui->setupUi(this);

    QList<QSerialPortInfo> 	availablePorts  = QSerialPortInfo::availablePorts();
    ui->comboBoxSerialPort->clear();
    qDebug() << "portName" << "description" << "manufacturer" << "serialNumber" << "productIdentifier" << "vendorIdentifier" << "systemLocation";
    for(int num = 0; num<availablePorts.length();num++)
    {
        qDebug() << availablePorts[num].portName() << availablePorts[num].description() << availablePorts[num].manufacturer() << availablePorts[num].serialNumber() << availablePorts[num].productIdentifier() << availablePorts[num].vendorIdentifier() << availablePorts[num].systemLocation();
        ui->comboBoxSerialPort->addItem(availablePorts[num].portName());
    }
}

DLTMultimeterSettingsDialog::~DLTMultimeterSettingsDialog()
{
    delete ui;
}

void DLTMultimeterSettingsDialog::restoreSettings(DLTMultimeterPlugin *plugin)
{
    ui->comboBoxSerialPort->setCurrentText(plugin->getInterface());
    ui->comboBoxType->setCurrentIndex(plugin->getType());
    ui->lineEditPowerName->setText(plugin->getPowerName());
    ui->lineEditPowerSubstractCurrent->setText(QString("%1").arg(plugin->getSubstractCurrent()));
}

void DLTMultimeterSettingsDialog::backupSettings(DLTMultimeterPlugin *plugin)
{
    plugin->setInterface(ui->comboBoxSerialPort->currentText());
    plugin->setType(ui->comboBoxType->currentIndex());
    plugin->setPowerName(ui->lineEditPowerName->text());
    plugin->setSubstractCurrent(ui->lineEditPowerSubstractCurrent->text().toFloat());
}
