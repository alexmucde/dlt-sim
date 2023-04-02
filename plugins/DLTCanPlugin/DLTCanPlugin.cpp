#include "DLTCanPlugin.h"
#include "DLTCanPluginsettingsdialog.h"
#include "DLTCanPluginform.h"
#include "dltminiserver.h"

#include <QFile>
#include <QMdiSubWindow>
#include <QSettings>

DLTCanPlugin::DLTCanPlugin()
{
    form = 0;
    window = 0;
    dltMiniServer = 0;

    rxMsgCounter = 0;
    txMsgCounter= 0;

    clearSettings();
}

DLTCanPlugin::~DLTCanPlugin()
{
}

void DLTCanPlugin::clearSettings()
{
    title = "DLT Can Plugin";

    interfaceSerialNumber = "";
    interfaceProductIdentifier = 0;
    interfaceVendorIdentifier = 0;
}

void DLTCanPlugin::settings(QWidget *parent)
{
    DLTCanPluginSettingsDialog dlg(parent);
    dlg.restoreSettings(this);

    if(dlg.exec()==QDialog::Accepted)
    {
        dlg.backupSettings(this);
    }
}

void DLTCanPlugin::readSettings(const QString &filename,const int instance)
{
    // read settings from XML file
    bool isDLTCan = false;

    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
             return;

    QXmlStreamReader xml(&file);

    while (!xml.atEnd())
    {
          xml.readNext();

          if(xml.isStartElement())
          {
              if(isDLTCan)
              {
                  /* Project settings */
                  if(xml.name() == QString("Version"))
                  {
                      xml.readElementText();
                      /* TODO: Check if the version is compatible */
                  }
                  else if(xml.name() == QString("Instance"))
                  {
                      if(instance != xml.readElementText().toInt())
                          isDLTCan = false;
                  }
                  else if(xml.name() == QString("title"))
                  {
                      title = xml.readElementText();
                  }
                  else if(xml.name() == QString("interface"))
                  {
                      interface = xml.readElementText();
                  }
              }
              else if(xml.name() == QString("Name"))
              {
                  if( xml.readElementText() == QString(DLT_CAN_PLUGIN_NAME))
                    isDLTCan = true;
              }
          }
          else if(xml.isEndElement())
          {
              /* Connection, plugin and filter */
              if(xml.name() == QString("Plugin"))
              {
                    isDLTCan = false;
              }
          }
    }
    if (xml.hasError())
    {
         qDebug() << "Error in processing filter file" << filename << xml.errorString();
    }

    file.close();
}

void DLTCanPlugin::writeSettings(QXmlStreamWriter &xml,const int instance) const
{
    // Plugin type and version
    xml.writeTextElement("Name",DLT_CAN_PLUGIN_NAME);
    xml.writeTextElement("Version",DLT_CAN_PLUGIN_VERSION);
    xml.writeTextElement("Instance",QString("%1").arg(instance));

    // Settings
    xml.writeStartElement("Settings");
        xml.writeTextElement("title",title);
        xml.writeTextElement("interface",interface);
    xml.writeEndElement(); // Settings
}

void DLTCanPlugin::start(DLTMiniServer *newDltMiniServer)
{
    dltMiniServer = newDltMiniServer;

    // set serial port parameters
    serialPort.setBaudRate(QSerialPort::Baud115200);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);
    serialPort.setPortName(interface);

    // open serial port
    if(serialPort.open(QIODevice::ReadWrite)==true)
    {
        // open with success

        // prevent flash mode of Wemos D1 mini
        serialPort.setDataTerminalReady(false);

        // connect slot to receive data from serial port
        connect(&serialPort, SIGNAL(readyRead()), this, SLOT(readyRead()));

        pluginStatus = "started";
        updateStatus();
        qDebug() << "DLTCan: started" << interface;
     }
    else
    {
        // open failed
        pluginStatus = "error";
        updateStatus();
        qDebug() << "DLTCan: Failed to open interface" << interface;
    }

    serialData.clear();

    // connect slot watchdog timer and start watchdog timer
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer.start(5000);
    watchDogCounter = 0;
    watchDogCounterLast = 0;
    startFound = false;

}

void DLTCanPlugin::stop()
{
    qDebug() << "DLTCan: stopped";
    pluginStatus = "stopped";
    updateStatus();

    // close serial port, if it is open
    if(serialPort.isOpen())
    {
        serialPort.close();

        // disconnect slot to receive data from serial port
        disconnect(&serialPort, SIGNAL(readyRead()), this, SLOT(readyRead()));
    }

    // stop watchdog timer
    timer.stop();
    disconnect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));

}

void DLTCanPlugin::show(QMdiArea *mdiArea)
{
    if(window == 0)
        window = new QMdiSubWindow;
    if(form == 0)
        form = new DLTCanPluginForm(this,window);
    window->setWidget(form);
    mdiArea->addSubWindow(window);
    window->show();
    restoreGeometry();
}

void DLTCanPlugin::update()
{
    if(form)
        form->update();
}

void DLTCanPlugin::hide()
{
    if(window)
        window->hide();
    delete window;
    window = 0;
    form = 0;
}

void DLTCanPlugin::backupGeometry()
{
    QSettings settings;
    settings.setValue("dlt_can_plugin_geometry", window->saveGeometry());
}

void DLTCanPlugin::restoreGeometry()
{
    QSettings settings;
    window->restoreGeometry(settings.value("dlt_can_plugin_geometry").toByteArray());
}

const QString &DLTCanPlugin::getInterface() const
{
    return interface;
}

void DLTCanPlugin::setInterface(const QString &newInterface)
{
    interface = newInterface;
}

const QString &DLTCanPlugin::getTitle() const
{
    return title;
}

void DLTCanPlugin::setTitle(const QString &newTitle)
{
    title = newTitle;
}

void DLTCanPlugin::injection(QString data)
{
    QStringList list = data.split(' ');

    //qDebug() << "Injection received: " << data;

    if(list[0] == "CAN")
    {
        unsigned short id = list[1].toUShort(nullptr,16);
        QByteArray data = QByteArray::fromHex(list[2].toLatin1());
        sendMessage(id,(unsigned char*)data.data(),data.length());
    }
    else if(list[0] == "CANCYC1")
    {
        if(list[1]=="off")
        {
            stopCyclicMessage1();
            if(window)
                window->update();
        }
        else
        {
            unsigned short time = list[1].toUShort();
            unsigned short id = list[2].toUShort(nullptr,16);
            QByteArray data = QByteArray::fromHex(list[3].toLatin1());

            setCyclicMessage1(id,data);
            startCyclicMessage1(time);

            if(window)
                window->update();
            //restoreSettings();
        }
    }
    else if(list[0] == "CANCYC2")
    {
        if(list[1]=="off")
        {
            stopCyclicMessage2();
            if(window)
                window->update();
        }
        else
        {
            unsigned short time = list[1].toUShort();
            unsigned short id = list[2].toUShort(nullptr,16);
            QByteArray data = QByteArray::fromHex(list[3].toLatin1());

            setCyclicMessage2(id,data);
            startCyclicMessage2(time);

            if(window)
                window->update();
            //restoreSettings();
        }
    }


}

void DLTCanPlugin::readyRead()
{
    QByteArray data = serialPort.readAll();
    qDebug() << "DLTCan: Received " << data.toHex();
    for(int num=0;num<data.length();num++)
    {
       if(data.at(num)==0x7f)
       {
            if(startFound)
            {
                // already last Byte was start byte; remove duplicate
                rawData+=data[num];
                startFound = false;
            }
            else
            {
                 startFound = true;
            }
       }
       else
       {
           if(startFound)
           {
               // a new message starts
               rawData.clear();
           }
           rawData+=data[num];
           startFound = false;
       }

       // check if it is already a complete message
       if(rawData.size()==1 && (unsigned char)rawData.at(0)==0x01)
       {
           // send ok
           qDebug() << "DLTCan: Raw Data " << rawData.toHex();
           qDebug() << "DLTCan: Send ok";
           pluginStatus = "send ok";
           updateStatus();
           rawData.clear();
       }
       else if(rawData.size()==1 && (unsigned char)rawData.at(0)==0x02)
       {
           // send ok
           qDebug() << "DLTCan: Raw Data " << rawData.toHex();
           qDebug() << "DLTCan: Watchdog";
           watchDogCounter++;
           rawData.clear();
       }
       else if(rawData.size()==1 && (unsigned char)rawData.at(0)==0xfe)
       {
           // error send
           qDebug() << "DLTCan: Raw Data " << rawData.toHex();
           qDebug() << "DLTCan: Send error";
           pluginStatus = "send error";
           updateStatus();
           rawData.clear();
       }
       else if(rawData.size()==1 && (unsigned char)rawData.at(0)==0x00)
       {
           // init ok
           qDebug() << "DLTCan: Raw Data " << rawData.toHex();
           qDebug() << "DLTCan: Init ok";
           pluginStatus = "init ok";
           updateStatus();
           rawData.clear();
       }
       else if(rawData.size()==1 && (unsigned char)rawData.at(0)==0xff)
       {
           // init error
           qDebug() << "DLTCan: Raw Data " << rawData.toHex();
           qDebug() << "DLTCan: Init Error";
           pluginStatus = "init error";
           updateStatus();
           rawData.clear();
       }
       else if(rawData.size()>=1 && (unsigned char)rawData.at(0)==0x80)
       {
           // standard CAN message
           if(rawData.size()>=4)
           {
               unsigned char length = rawData.at(1);
               if(rawData.size()>=(4+length))
               {
                   qDebug() << "DLTCan: Raw Data " << rawData.toHex();
                   unsigned short id = ((unsigned short)rawData.at(2)<<8)|((unsigned short)rawData.at(3));
                   QByteArray data = rawData.mid(4,length);
                   qDebug() << "DLTCan: Standard CAN message " << id << length << data.toHex();
                   emit message(id,"Rx",data);
                   rawData.clear();
               }
           }
       }
       else if(rawData.size()>=1 && (unsigned char)rawData.at(0)==0x81)
       {
           // extended CAN message

           if(rawData.size()>=6)
           {
               qDebug() << "DLTCan: Raw Data " << rawData.toHex();
               unsigned char length = rawData.at(1);
               if(rawData.size()>=(6+length))
               {
                   unsigned int id = ((unsigned int)rawData.at(2)<<8)|((unsigned int)rawData.at(3)<<8)|((unsigned int)rawData.at(4)<<8)|((unsigned int)rawData.at(5));
                   QByteArray data = rawData.mid(6,length);
                   qDebug() << "DLTCan: Extended CAN message " << id << length << data.toHex();
                   emit message(id,"Rx",data);
                   rawData.clear();
               }
           }
       }

    }
}

void DLTCanPlugin::timeout()
{
    // watchdog timeout

    // check if watchdog was triggered between last call
    if(watchDogCounter!=watchDogCounterLast)
    {
        watchDogCounterLast = watchDogCounter;
        pluginStatus = "started";
        emit updateStatus();
    }
    else
    {
        // no watchdog was received
        qDebug() << "DLTCan: Watchdog expired try to reconnect" ;

        // if serial port is open close serial port
        if(serialPort.isOpen())
        {
            serialPort.close();
            disconnect(&serialPort, SIGNAL(readyRead()), this, SLOT(readyRead()));
        }

        // check if port name has changed
        // checkPortName();

        serialData.clear();

        // try to reopen serial port
        if(serialPort.open(QIODevice::ReadWrite)==true)
        {
            // retry was succesful

            // prevent flash mode of Wemos D1 mini
            serialPort.setDataTerminalReady(false);

            // connect slot to receive data from serial port
            connect(&serialPort, SIGNAL(readyRead()), this, SLOT(readyRead()));

            pluginStatus = "reconnect";
            emit updateStatus();
            qDebug() << "DLTCan: reconnect" << interface;
        }
        else
        {
            // retry failed

            qDebug() << "DLTCan: Failed to open interface" << interface;
            pluginStatus = "error";
            emit updateStatus();
        }
    }

}

void DLTCanPlugin::sendMessage(unsigned short id,unsigned char *data,int length)
{
    if(!active)
    {
        return;
    }

    unsigned char msg[256];

    msg[0]=0x7f;
    msg[1]=0x80;
    msg[2]=length;
    msg[3]=(id>>8)&0xff;
    msg[4]=id&0xff;
    int pos = 5;
    for(int num=0;num<length;num++)
    {
        msg[pos++]=data[num];
        //if(data[num]==0x7f)
        //    msg[pos++]=0x7f; // add stuff byte to be able to detect unique header
    }
    //memcpy((void*)(msg+5),(void*)data,length);
    serialPort.write((char*)msg,pos);

    messageId = id;
    messageData = QByteArray((char*)data,length);

    qDebug() << "DLTCan: Send CAN message " << id << length << QByteArray((char*)msg,pos).toHex();

    emit message(id,"Tx",QByteArray((char*)data,length));

}

void DLTCanPlugin::startCyclicMessage1(int timeout)
{
    connect(&timerCyclicMessage1, SIGNAL(timeout()), this, SLOT(timeoutCyclicMessage1()));

    cyclicMessageTimeout1 = timeout;
    cyclicMessageActive1 = true;

    timerCyclicMessage1.start(timeout);
}

void DLTCanPlugin::startCyclicMessage2(int timeout)
{
    connect(&timerCyclicMessage2, SIGNAL(timeout()), this, SLOT(timeoutCyclicMessage2()));

    cyclicMessageTimeout2 = timeout;
    cyclicMessageActive2 = true;

    timerCyclicMessage2.start(timeout);
}

void DLTCanPlugin::stopCyclicMessage1()
{
    timerCyclicMessage1.stop();
    cyclicMessageActive1 = false;

    disconnect(&timerCyclicMessage1, SIGNAL(timeout()), this, SLOT(timeoutCyclicMessage1()));
}

void DLTCanPlugin::stopCyclicMessage2()
{
    timerCyclicMessage2.stop();
    cyclicMessageActive2 = false;

    disconnect(&timerCyclicMessage2, SIGNAL(timeout()), this, SLOT(timeoutCyclicMessage2()));
}

void DLTCanPlugin::timeoutCyclicMessage1()
{
    if(!active)
    {
        return;
    }

    unsigned char msg[256];

    msg[0]=0x7f;
    msg[1]=0x80;
    msg[2]=cyclicMessageData1.length();
    msg[3]=(cyclicMessageId1>>8)&0xff;
    msg[4]=cyclicMessageId1&0xff;
    memcpy((void*)(msg+5),(void*)cyclicMessageData1.constData(),cyclicMessageData1.length());
    serialPort.write((char*)msg,cyclicMessageData1.length()+5);

    qDebug() << "DLTCan: Send CAN message " << cyclicMessageId1 << cyclicMessageData1.length() << QByteArray((char*)msg,cyclicMessageData1.length()+5).toHex();

    message(cyclicMessageId1,"Tx",QByteArray((char*)cyclicMessageData1.constData(),cyclicMessageData1.length()));
}

void DLTCanPlugin::timeoutCyclicMessage2()
{
    if(!active)
    {
        return;
    }

    unsigned char msg[256];

    msg[0]=0x7f;
    msg[1]=0x80;
    msg[2]=cyclicMessageData2.length();
    msg[3]=(cyclicMessageId2>>8)&0xff;
    msg[4]=cyclicMessageId2&0xff;
    memcpy((void*)(msg+5),(void*)cyclicMessageData2.constData(),cyclicMessageData2.length());
    serialPort.write((char*)msg,cyclicMessageData2.length()+5);

    qDebug() << "DLTCan: Send CAN message " << cyclicMessageId2 << cyclicMessageData2.length() << QByteArray((char*)msg,cyclicMessageData2.length()+5).toHex();

    message(cyclicMessageId2,"Tx",QByteArray((char*)cyclicMessageData2.constData(),cyclicMessageData2.length()));
}

ulong DLTCanPlugin::getTxMsgCounter() const
{
    return txMsgCounter;
}

ulong DLTCanPlugin::getRxMsgCounter() const
{
    return rxMsgCounter;
}

bool DLTCanPlugin::getCyclicMessageActive2() const
{
    return cyclicMessageActive2;
}

void DLTCanPlugin::setCyclicMessageActive2(bool value)
{
    cyclicMessageActive2 = value;
}

bool DLTCanPlugin::getCyclicMessageActive1() const
{
    return cyclicMessageActive1;
}

void DLTCanPlugin::setCyclicMessageActive1(bool value)
{
    cyclicMessageActive1 = value;
}

int DLTCanPlugin::getCyclicMessageTimeout2() const
{
    return cyclicMessageTimeout2;
}

void DLTCanPlugin::setCyclicMessageTimeout2(int value)
{
    cyclicMessageTimeout2 = value;
}

int DLTCanPlugin::getCyclicMessageTimeout1() const
{
    return cyclicMessageTimeout1;
}

void DLTCanPlugin::setCyclicMessageTimeout1(int value)
{
    cyclicMessageTimeout1 = value;
}

QByteArray DLTCanPlugin::getCyclicMessageData2() const
{
    return cyclicMessageData2;
}

void DLTCanPlugin::setCyclicMessageData2(const QByteArray &value)
{
    cyclicMessageData2 = value;
}

QByteArray DLTCanPlugin::getCyclicMessageData1() const
{
    return cyclicMessageData1;
}

void DLTCanPlugin::setCyclicMessageData1(const QByteArray &value)
{
    cyclicMessageData1 = value;
}

QByteArray DLTCanPlugin::getMessageData() const
{
    return messageData;
}

void DLTCanPlugin::setMessageData(const QByteArray &value)
{
    messageData = value;
}

unsigned short DLTCanPlugin::getCyclicMessageId2() const
{
    return cyclicMessageId2;
}

void DLTCanPlugin::setCyclicMessageId2(unsigned short value)
{
    cyclicMessageId2 = value;
}

unsigned short DLTCanPlugin::getCyclicMessageId1() const
{
    return cyclicMessageId1;
}

void DLTCanPlugin::setCyclicMessageId1(unsigned short value)
{
    cyclicMessageId1 = value;
}

unsigned short DLTCanPlugin::getMessageId() const
{
    return messageId;
}

void DLTCanPlugin::setMessageId(unsigned short value)
{
    messageId = value;
}

void DLTCanPlugin::setCyclicMessage1(unsigned short id,QByteArray data)
{
    cyclicMessageId1 = id;
    cyclicMessageData1 = data;
}

void DLTCanPlugin::setCyclicMessage2(unsigned short id,QByteArray data)
{
    cyclicMessageId2 = id;
    cyclicMessageData2 = data;
}

void  DLTCanPlugin::message(unsigned int id,QString direction,QByteArray data)
{
    if(dltMiniServer)
        dltMiniServer->sendValue3(direction,QString("%1").arg(id, 3, 16, QLatin1Char( '0' )),data.toHex());

    if(direction=="Rx")
        rxMsgCounter++;
    if(direction=="Tx")
        txMsgCounter++;

    emit updateStatus();
}
