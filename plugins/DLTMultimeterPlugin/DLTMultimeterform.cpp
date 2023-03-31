#include "DLTMultimeterform.h"
#include "ui_DLTMultimeterform.h"

#include "DLTMultimeterPlugin.h"

DLTMultimeterForm::DLTMultimeterForm(DLTMultimeterPlugin *newPlugin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DLTMultimeterForm)
{
    ui->setupUi(this);

    plugin = newPlugin;

    update();
}

DLTMultimeterForm::~DLTMultimeterForm()
{
    plugin->backupGeometry();

    delete ui;
}

void DLTMultimeterForm::update()
{
    ui->lineEditPowerName->setText(plugin->getPowerName());
}

void DLTMultimeterForm::valueMultimeter(QString value,QString unit)
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

    /*if(unit=="A")
        dltMiniServer.sendValue2(plugin->getDltAppId(),"AMP",dltMultimeter1.getPowerName(),value);
    else if(unit=="V")
        dltMiniServer.sendValue2(plugin->getDltAppId(),"VOLT",dltMultimeter1.getPowerName(),value);
    else if(unit=="mA")
        dltMiniServer.sendValue2(plugin->getDltAppId(),"MAMP",dltMultimeter1.getPowerName(),value);
    else if(unit=="mV")
        dltMiniServer.sendValue2(plugin->getDltAppId(),"MVOL",dltMultimeter1.getPowerName(),value);
    */
}

void DLTMultimeterForm::on_checkBoxOn_clicked(bool checked)
{
    if(checked)
        plugin->on();
    else
        plugin->off();
}


void DLTMultimeterForm::on_pushButtonSet_clicked()
{

}

void DLTMultimeterForm::injection(QString data)
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
    }
}
