#include "DLTCanPluginform.h"
#include "ui_DLTCanPluginform.h"
#include "DLTCanPlugin.h"

#include <QCloseEvent>

DLTCanPluginForm::DLTCanPluginForm(DLTCanPlugin *newPlugin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DLTCanPluginForm)
{
    ui->setupUi(this);

    plugin = newPlugin;

    update();
}

DLTCanPluginForm::~DLTCanPluginForm()
{
    plugin->backupGeometry();

    delete ui;
}

void DLTCanPluginForm::on_pushButtonSend_clicked()
{
    unsigned short id = ui->lineEditSendMsgId->text().toUShort(nullptr,16);
    QByteArray data = QByteArray::fromHex(ui->lineEditSendMsgData->text().toLatin1());
    plugin->sendMessage(id,(unsigned char*)data.data(),data.length());
}


void DLTCanPluginForm::on_checkBoxSendCyclicMsg1Active_clicked(bool checked)
{
    if(checked)
    {
        plugin->setCyclicMessage1(ui->lineEditSendCylicMsg1Id->text().toUShort(nullptr,16),QByteArray::fromHex(ui->lineEditSendCylicMsg1Data->text().toLatin1()));
        plugin->startCyclicMessage1(ui->lineEditSendCylicMsg1Cycle->text().toInt());
    }
    else
    {
        plugin->stopCyclicMessage1();
    }
}


void DLTCanPluginForm::on_checkBoxSendCyclicMsg2Active_clicked(bool checked)
{
    if(checked)
    {
        plugin->setCyclicMessage2(ui->lineEditSendCylicMsg2Id->text().toUShort(nullptr,16),QByteArray::fromHex(ui->lineEditSendCylicMsg2Data->text().toLatin1()));
        plugin->startCyclicMessage2(ui->lineEditSendCylicMsg2Cycle->text().toInt());
    }
    else
    {
        plugin->stopCyclicMessage2();
    }
}

void DLTCanPluginForm::update()
{
    setWindowTitle(plugin->getTitle());

    ui->lineEditCountMessagesRx->setText(QString("%1").arg(plugin->getRxMsgCounter()));
    ui->lineEditCountMessagesTx->setText(QString("%1").arg(plugin->getTxMsgCounter()));
}

void DLTCanPluginForm::closeEvent(QCloseEvent *event)
{
    event->ignore();
}
