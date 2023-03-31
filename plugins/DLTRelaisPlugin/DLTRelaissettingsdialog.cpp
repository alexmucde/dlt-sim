#include "DLTRelaissettingsdialog.h"
#include "ui_DLTRelaissettingsdialog.h"
#include "DLTRelaisPlugin.h"

#include <QSerialPortInfo>

DLTRelaisSettingsDialog::DLTRelaisSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DLTRelaisSettingsDialog)
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

DLTRelaisSettingsDialog::~DLTRelaisSettingsDialog()
{
    delete ui;
}

void DLTRelaisSettingsDialog::restoreSettings(DLTRelaisPlugin *dltRelais)
{
    ui->lineEditTitle->setText(dltRelais->getTitle());
    ui->lineEditRelaisName1->setText(dltRelais->getRelaisName(1));
    ui->lineEditRelaisName2->setText(dltRelais->getRelaisName(2));
    ui->lineEditRelaisName3->setText(dltRelais->getRelaisName(3));
    ui->lineEditRelaisName4->setText(dltRelais->getRelaisName(4));
    ui->comboBoxSerialPort->setCurrentText(dltRelais->getInterface());
    ui->comboBoxType->setCurrentIndex(dltRelais->getType());
}

void DLTRelaisSettingsDialog::backupSettings(DLTRelaisPlugin *dltRelais)
{
    dltRelais->setInterface(ui->comboBoxSerialPort->currentText());
    dltRelais->setTitle(ui->lineEditTitle->text());
    dltRelais->setRelaisName(ui->lineEditRelaisName1->text(),1);
    dltRelais->setRelaisName(ui->lineEditRelaisName2->text(),2);
    dltRelais->setRelaisName(ui->lineEditRelaisName3->text(),3);
    dltRelais->setRelaisName(ui->lineEditRelaisName4->text(),4);
    dltRelais->setType(ui->comboBoxType->currentIndex());
}

