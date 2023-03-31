#include "DLTPowerform.h"
#include "ui_DLTPowerform.h"

#include "DLTPowerPlugin.h"

DLTPowerForm::DLTPowerForm(DLTPowerPlugin *newPlugin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DLTPowerForm)
{
    ui->setupUi(this);

    plugin = newPlugin;

    update();
}

DLTPowerForm::~DLTPowerForm()
{
    plugin->backupGeometry();

    delete ui;
}

void DLTPowerForm::update()
{
    ui->lineEditPowerName->setText(plugin->getPowerName());
}

void DLTPowerForm::valueMultimeter(QString value,QString unit)
{
    if(unit=="A" || unit=="mA")
    {
        ui->lineEditUnitA1->setText(unit);
        ui->lineEditValueA1->setText(value);
    }
    if(unit=="V" || unit=="mV")
    {
        ui->lineEditUnitV1->setText(unit);
        ui->lineEditValueV1->setText(value);
    }
}

void DLTPowerForm::on_checkBoxOn_clicked(bool checked)
{
    if(checked)
        plugin->on();
    else
        plugin->off();
}


void DLTPowerForm::on_pushButtonSet_clicked()
{
    plugin->setVoltage(ui->doubleSpinBox->value());
}

void DLTPowerForm::injection(QString data)
{
    QStringList list = data.split(' ');

    //qDebug() << "Injection received: " << text;

    if(plugin->getPowerName() == list[0])
    {
        if(list[1]=="on" || list[1]=="On")
        {
            ui->checkBoxOn->setChecked(true);
            on_checkBoxOn_clicked(true);
        }
        else if(list[1]=="off" || list[1]=="Off")
        {
            ui->checkBoxOn->setChecked(false);
            on_checkBoxOn_clicked(false);
        }
        else if(list[1]=="VOLTAGE")
        {
            float value = list[2].toFloat();
            plugin->setVoltage(value);
            ui->doubleSpinBox->setValue(value);
        }

    }
}
