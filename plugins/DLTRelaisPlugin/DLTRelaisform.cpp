#include "DLTRelaisPlugin.h"
#include "DLTRelaisform.h"
#include "ui_DLTRelaisform.h"

#include <QSettings>
#include <QCloseEvent>

DLTRelaisForm::DLTRelaisForm(DLTRelaisPlugin *newDltRelais, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DLTRelaisForm)
{
    ui->setupUi(this);
    dltRelais = newDltRelais;

    update();

    QSettings settings;
    restoreGeometry(settings.value("dlt_relais_plugin_geometry").toByteArray());
}

DLTRelaisForm::~DLTRelaisForm()
{
    dltRelais->backupGeometry();

    delete ui;
}

void DLTRelaisForm::update()
{
    ui->checkBoxRelais1->setText(dltRelais->getRelaisName(1));
    ui->checkBoxRelais2->setText(dltRelais->getRelaisName(2));
    ui->checkBoxRelais3->setText(dltRelais->getRelaisName(3));
    ui->checkBoxRelais4->setText(dltRelais->getRelaisName(4));

    if(dltRelais->getType()==0)
    {
        ui->checkBoxRelais4->hide();
        ui->pushButtonRelais4->hide();
    }
    else
    {
        ui->checkBoxRelais4->show();
        ui->pushButtonRelais4->show();
    }

    setWindowTitle(dltRelais->getTitle());
}

void DLTRelaisForm::on_pushButtonRelais1_clicked()
{
    dltRelais->trigger(1,500);
}


void DLTRelaisForm::on_pushButtonRelais2_clicked()
{
    dltRelais->trigger(2,500);
}


void DLTRelaisForm::on_pushButtonRelais3_clicked()
{
    dltRelais->trigger(3,500);
}


void DLTRelaisForm::on_pushButtonRelais4_clicked()
{
    dltRelais->trigger(4,500);
}


void DLTRelaisForm::on_checkBoxRelais1_clicked(bool checked)
{
    if(checked)
        dltRelais->on(1);
    else
        dltRelais->off(1);
}


void DLTRelaisForm::on_checkBoxRelais2_clicked(bool checked)
{
    if(checked)
        dltRelais->on(2);
    else
        dltRelais->off(2);
}


void DLTRelaisForm::on_checkBoxRelais3_clicked(bool checked)
{
    if(checked)
        dltRelais->on(3);
    else
        dltRelais->off(3);
}


void DLTRelaisForm::on_checkBoxRelais4_clicked(bool checked)
{
    if(checked)
        dltRelais->on(4);
    else
        dltRelais->off(4);
}

void DLTRelaisForm::closeEvent(QCloseEvent *event)
{
    event->ignore();
}

void DLTRelaisForm::injection(QString data)
{
    QStringList list = data.split(' ');

    //qDebug() << "Injection received: " << text;

    if(dltRelais->getRelaisName(1) == list[0])
    {
        if(list[1]=="on" || list[1]=="On")
        {
            ui->checkBoxRelais1->setChecked(true);
            on_checkBoxRelais1_clicked(true);
        }
        else if(list[1]=="off" || list[1]=="Off")
        {
            ui->checkBoxRelais1->setChecked(false);
            on_checkBoxRelais1_clicked(false);
        }
        else if(list[1]=="trigger" || list[1]=="Trigger")
        {
            if(list.length()>2)
            {
                dltRelais->trigger(1,list[2].toUInt());
            }
            else
            {
               dltRelais->trigger(1,500);
            }
        }
    }
    else if(dltRelais->getRelaisName(2) == list[0])
    {
        if(list[1]=="on" || list[1]=="On")
        {
            ui->checkBoxRelais2->setChecked(true);
            on_checkBoxRelais2_clicked(true);
        }
        else if(list[1]=="off" || list[1]=="Off")
        {
            ui->checkBoxRelais2->setChecked(false);
            on_checkBoxRelais2_clicked(false);
        }
        else if(list[1]=="trigger" || list[1]=="Trigger")
        {
            if(list.length()>2)
            {
                dltRelais->trigger(2,list[2].toUInt());
            }
            else
            {
               dltRelais->trigger(2,500);
            }
        }
    }
    else if(dltRelais->getRelaisName(3) == list[0])
    {
        if(list[1]=="on" || list[1]=="On")
        {
            ui->checkBoxRelais3->setChecked(true);
            on_checkBoxRelais3_clicked(true);
        }
        else if(list[1]=="off" || list[1]=="Off")
        {
            ui->checkBoxRelais3->setChecked(false);
            on_checkBoxRelais3_clicked(false);
        }
        else if(list[1]=="trigger" || list[1]=="Trigger")
        {
            if(list.length()>2)
            {
                dltRelais->trigger(3,list[2].toUInt());
            }
            else
            {
               dltRelais->trigger(3,500);
            }
        }
    }    else if(dltRelais->getRelaisName(4) == list[0])
    {
        if(list[1]=="on" || list[1]=="On")
        {
            ui->checkBoxRelais4->setChecked(true);
            on_checkBoxRelais4_clicked(true);
        }
        else if(list[1]=="off" || list[1]=="Off")
        {
            ui->checkBoxRelais4->setChecked(false);
            on_checkBoxRelais4_clicked(false);
        }
        else if(list[1]=="trigger" || list[1]=="Trigger")
        {
            if(list.length()>2)
            {
                dltRelais->trigger(4,list[2].toUInt());
            }
            else
            {
               dltRelais->trigger(4,500);
            }
        }
    }
}
