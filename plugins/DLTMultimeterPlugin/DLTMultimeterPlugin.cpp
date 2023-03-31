#include "DLTMultimeterPlugin.h"
#include "DLTMultimetersettingsdialog.h"
#include "DLTMultimeterform.h"
#include "dltminiserver.h"

#include <QFile>
#include <QMdiSubWindow>
#include <QSerialPortInfo>
#include <QSettings>

DLTMultimeterPlugin::DLTMultimeterPlugin()
{
    form = 0;
    window = 0;
    dltMiniServer = 0;

    clearSettings();

    powerName = "Power";
    readVoltageOngoing = false;
}

DLTMultimeterPlugin::~DLTMultimeterPlugin()
{
    stop();

}

void DLTMultimeterPlugin::checkPortName()
{
    /* check if information is stored, if not do not check */
    if(interfaceSerialNumber.isEmpty() && interfaceProductIdentifier==0 && interfaceVendorIdentifier==0)
        return;

    /* check if port information still matches port name */
    if((QSerialPortInfo(interface).serialNumber()!=interfaceSerialNumber ||
       QSerialPortInfo(interface).productIdentifier()!=interfaceProductIdentifier ||
       QSerialPortInfo(interface).vendorIdentifier()!=interfaceVendorIdentifier))
    {
        qDebug() << "Port" << interface << "not found anymore";

        /* port name has changed, try to find new port name */
        QList<QSerialPortInfo> 	availablePorts  = QSerialPortInfo::availablePorts();
        for(int num = 0; num<availablePorts.length();num++)
        {
            if(availablePorts[num].serialNumber()==interfaceSerialNumber &&
               availablePorts[num].productIdentifier()==interfaceProductIdentifier &&
               availablePorts[num].vendorIdentifier()==interfaceVendorIdentifier)
            {
                // The following is not working, if several interfaces have the same name
                // Must be improved in future, e.g. with Serial Id of each device
                // qDebug() << "Port name has changed from" << interface << "to" << availablePorts[num].portName();
                // interface = availablePorts[num].portName();
            }
        }
    }
}


void DLTMultimeterPlugin::clearSettings()
{
    type = 0;
    powerName = "Power";
    substractCurrent = 0;

    interfaceSerialNumber = "";
    interfaceProductIdentifier = 0;
    interfaceVendorIdentifier = 0;

    dltAppId = "DLTM";
    dltCtxId = "PWR";
}

void DLTMultimeterPlugin::settings(QWidget *parent)
{
    DLTMultimeterSettingsDialog dlg(parent);
    dlg.restoreSettings(this);

    if(dlg.exec()==QDialog::Accepted)
    {
        dlg.backupSettings(this);
    }
}

void DLTMultimeterPlugin::readSettings(const QString &filename,const int instance)
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
                  /* Project settings */
                  if(xml.name() == QString("interface"))
                  {
                      interface = xml.readElementText();
                  }
                  else if(xml.name() == QString("interfaceSerialNumber"))
                  {
                      interfaceSerialNumber = xml.readElementText();
                  }
                  else if(xml.name() == QString("interfaceProductIdentifier"))
                  {
                      interfaceProductIdentifier = xml.readElementText().toUShort();
                  }
                  else if(xml.name() == QString("interfaceVendorIdentifier"))
                  {
                      interfaceVendorIdentifier = xml.readElementText().toUShort();
                  }
                  else if(xml.name() == QString("interfaceSerialNumber"))
                  {
                      interfaceSerialNumber = xml.readElementText();
                  }
                  else if(xml.name() == QString("interfaceProductIdentifier"))
                  {
                      interfaceProductIdentifier = xml.readElementText().toUShort();
                  }
                  else if(xml.name() == QString("interfaceVendorIdentifier"))
                  {
                      interfaceVendorIdentifier = xml.readElementText().toUShort();
                  }
                  else if(xml.name() == QString("type"))
                  {
                      type = xml.readElementText().toInt();
                  }
                  else if(xml.name() == QString("powerName"))
                  {
                      powerName = xml.readElementText();
                  }
                  else if(xml.name() == QString("substractCurrent"))
                  {
                      substractCurrent = xml.readElementText().toFloat();
                  }
              }
              else if(xml.name() == QString("Name"))
              {
                  if( xml.readElementText() == QString(DLT_MULTIMETER_PLUGIN_NAME))
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

void DLTMultimeterPlugin::writeSettings(QXmlStreamWriter &xml,const int instance) const
{
    // Plugin type and version
    xml.writeTextElement("Name",DLT_MULTIMETER_PLUGIN_NAME);
    xml.writeTextElement("Version",DLT_MULTIMETER_PLUGIN_VERSION);
    xml.writeTextElement("Instance",QString("%1").arg(instance));

    // Settings
    xml.writeStartElement("Settings");
        xml.writeTextElement("interface",interface);
        xml.writeTextElement("interfaceSerialNumber",QSerialPortInfo(interface).serialNumber());
        xml.writeTextElement("interfaceProductIdentifier",QString("%1").arg(QSerialPortInfo(interface).productIdentifier()));
        xml.writeTextElement("interfaceVendorIdentifier",QString("%1").arg(QSerialPortInfo(interface).vendorIdentifier()));
        xml.writeTextElement("type",QString("%1").arg(type));
        xml.writeTextElement("powerName",powerName);
        xml.writeTextElement("substractCurrent",QString("%1").arg(substractCurrent));
    xml.writeEndElement(); // Settings
}

void DLTMultimeterPlugin::start(DLTMiniServer *newDltMiniServer)
{
    dltMiniServer = newDltMiniServer;
    // start communication
    // checkPortName();

    value = 0;

    // set serial port parameters
    if(type==0) // Holdpeak HP-90EPC
        serialPort.setBaudRate(QSerialPort::Baud2400);
    else if(type==1) // Mason HCS-3302 USB
        serialPort.setBaudRate(QSerialPort::Baud9600);
    else if(type==2) // WemosD1MiniPower
        serialPort.setBaudRate(QSerialPort::Baud115200);
    else if(type==3) // OWON XDM1041
        serialPort.setBaudRate(QSerialPort::Baud115200);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setPortName(interface);

    // open serial port
    if(serialPort.open(QIODevice::ReadWrite)==true)
    {
        // open with success

        // connect slot to receive data from serial port
        connect(&serialPort, SIGNAL(readyRead()), this, SLOT(readyRead()));

        pluginStatus = "started";
        emit updateStatus();
        qDebug() << "DLTMultimeterPlugin: started" << interface;
     }
    else
    {
        // open failed

        qDebug() << "DLTMultimeterPlugin: Failed to open interface" << interface;
        pluginStatus = "error";
        emit updateStatus();
    }

    if(type==1) // Mason HCS-3302 USB
    {
        timerRequest.start(1000);
        connect(&timerRequest, SIGNAL(timeout()), this, SLOT(timeoutRequest()));
    }
    if(type==3) // OWON XDM1041
    {
        timerRequest.start(1000);
        connect(&timerRequest, SIGNAL(timeout()), this, SLOT(timeoutRequest()));
        serialPort.write("CONF:CURR\r\n");
    }

    serialData.clear();

    // connect slot watchdog timer and start watchdog timer
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer.start(5000);
    watchDogCounter = 0;
    watchDogCounterLast = 0;
    readVoltageOngoing = false;
}

void DLTMultimeterPlugin::stop()
{
    // stop communication
    pluginStatus = "stopped";
    emit updateStatus();
    qDebug() << "DLTMultimeterPlugin: stopped" << interface;

    if(type==1) // Mason HCS-3302 USB
    {
        timerRequest.stop();
        disconnect(&timerRequest, SIGNAL(timeout()), this, SLOT(timeoutRequest()));
    }
    if(type==3) // OWON XDM1041
    {
        timerRequest.stop();
        disconnect(&timerRequest, SIGNAL(timeout()), this, SLOT(timeoutRequest()));
    }

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

void DLTMultimeterPlugin::show(QMdiArea *mdiArea)
{
    if(window == 0)
        window = new QMdiSubWindow;
    if(form == 0)
        form = new DLTMultimeterForm(this,window);
    window->setWidget(form);
    mdiArea->addSubWindow(window);
    window->show();
    restoreGeometry();
}

void DLTMultimeterPlugin::update()
{
    if(form)
        form->update();
}

void DLTMultimeterPlugin::hide()
{
    if(window)
        window->hide();
    delete window;
    window = 0;
    form = 0;
}

void DLTMultimeterPlugin::readyRead()
{
    if(type==0) // Holdpeak HP-90EPC
    {
        QByteArray data = serialPort.readAll();
        for(int num=0;num<data.length();num++)
        {
            switch(data[num]&0xf0)
            {
                case 0x10:
                    rawData.clear();
                    rawData+=data[num];
                    break;
                case 0xe0:
                    data.clear();
                    rawData+=data[num];
                    //qDebug() << "DLTMultimeterPlugin: Raw Data " << rawData.toHex();
                    calculateValue();
                    if(form)
                        form->valueMultimeter(QString("%1").arg(value),unit);
                    if(unit=="A" && dltMiniServer)
                        dltMiniServer->sendValue2(dltAppId,"AMP",powerName,QString("%1").arg(value));
                    else if(unit=="V" && dltMiniServer)
                        dltMiniServer->sendValue2(dltAppId,"VOLT",powerName,QString("%1").arg(value));
                    else if(unit=="mA" && dltMiniServer)
                        dltMiniServer->sendValue2(dltAppId,"MAMP",powerName,QString("%1").arg(value));
                    else if(unit=="mV" && dltMiniServer)
                        dltMiniServer->sendValue2(dltAppId,"MVOL",powerName,QString("%1").arg(value));
                    watchDogCounter++;
                    //qDebug() << "DLTMultimeterPlugin: Value received" << interface << value << unit;
                    break;
                default:
                    rawData+=data[num];
                    break;
            }
        }
    }
    else if(type==1) // Mason HCS-3302 USB
    {
        // loop as long as data is available
        while(serialPort.bytesAvailable())
        {
            // read one line form serial port
            serialData += serialPort.readAll();

            int pos;
            pos = serialData.indexOf('\r');
            while(pos!=-1)
            {
                QString line(serialData.mid(0,pos));

                //qDebug() << "DLTMultimeterPlugin: readLine" << line;

                if(line=="OK")
                {
                    watchDogCounter++;
                }
                else if(line.length()==9)
                {
                    double current = (QString("%1%2.%3%4%5").arg(line[4]).arg(line[5]).arg(line[6]).arg(line[7]).arg(line[8])).toDouble() - substractCurrent;
//                    valueMultimeter(QString("%1").arg(current),"A");
                    if(form)
                        form->valueMultimeter(QString::number(current, 'f', 3),"A");
                    if(form)
                        form->valueMultimeter(QString("%1%2.%3%4").arg(line[0]).arg(line[1]).arg(line[2]).arg(line[3]),"V");
                    if(dltMiniServer)
                        dltMiniServer->sendValue2(dltAppId,"AMP",powerName,QString::number(current, 'f', 3));
                    if(dltMiniServer)
                        dltMiniServer->sendValue2(dltAppId,"VOLT",powerName,QString("%1%2.%3%4").arg(line[0]).arg(line[1]).arg(line[2]).arg(line[3]));
                    readVoltageOngoing = false;
                    if(!voltageCmd.isEmpty())
                    {
                        serialPort.write(voltageCmd.toLatin1());
                        voltageCmd.clear();
                    }
                }

                serialData.remove(0,pos+1);
                pos = serialData.indexOf('\r');
            }
        }
    }
    else if(type==2) // WemosD1MiniPower
    {
        // loop as long as data is available
        while(serialPort.bytesAvailable())
        {
            char data[256];

            // read one line form serial port
            qint64 size = serialPort.readLine(data,sizeof(data));

            if(size>0)
            {
                // line is not empty
                //qDebug() << "DLTMultimeterPlugin: readLine" << data;

                if(QString(data) == "WDMP\r\n")
                {
                    // watchdog message received
                    watchDogCounter++;
                }
                else if(QString(data) == "failed\r\n")
                {
                }
                else if(data[0]=='C')
                {
                    QString text = QString(data);
                    QStringList list = text.split(' ');
                    float current = list[1].toFloat();
                    if(form)
                        form->valueMultimeter(QString("%1").arg(current/(float)1000),"A");
                    if(dltMiniServer)
                        dltMiniServer->sendValue2(dltAppId,"AMP",powerName,QString("%1").arg(current/(float)1000));
                }
                else if(data[0]=='V')
                {
                    QString text = QString(data);
                    QStringList list = text.split(' ');
                    float voltage = list[1].toFloat();
                    if(form)
                        form->valueMultimeter(QString("%1").arg(voltage),"V");
                    if(unit=="V" && dltMiniServer)
                        dltMiniServer->sendValue2(dltAppId,"VOLT",powerName,QString("%1").arg(voltage));
                }
            }
        }
    }
    else if(type==3) // OWON XDM1041
    {
        // loop as long as data is available
        while(serialPort.bytesAvailable())
        {
            // read one line form serial port
            serialData += serialPort.readAll();

            int pos;
            pos = serialData.indexOf('\r');
            while(pos!=-1)
            {
                QString line(serialData.mid(0,pos));
                line.remove('\n');

                //qDebug() << "DLTMultimeterPlugin: readLine" << line;

                if(!line.isEmpty())
                {
                    watchDogCounter++;
                    double current = line.toFloat();
//                    valueMultimeter(QString("%1").arg(current,0,'g',3),"A");
                    if(form)
                        form->valueMultimeter(QString::number(current, 'f', 3),"A");
                    if(dltMiniServer)
                        dltMiniServer->sendValue2(dltAppId,"AMP",powerName,QString::number(current, 'f', 3));
                }

                serialData.remove(0,pos+1);
                pos = serialData.indexOf('\r');
            }
        }
    }
}

void DLTMultimeterPlugin::timeoutRequest()
{
    if(type==1) // Mason HCS-3302 USB
    {
        serialPort.write("GETD\r");
        readVoltageOngoing = true;
    }
    if(type==3) // OWON XDM1041
    {
        serialPort.write("MEAS?\r\n");
        readVoltageOngoing = true;
    }

}

float DLTMultimeterPlugin::getSubstractCurrent() const
{
    return substractCurrent;
}

void DLTMultimeterPlugin::setSubstractCurrent(float newSubstractCurrent)
{
    substractCurrent = newSubstractCurrent;
}

void DLTMultimeterPlugin::timeout()
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
        qDebug() << "DLTMultimeterPlugin: Watchdog expired try to reconnect" ;

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

            // connect slot to receive data from serial port
            connect(&serialPort, SIGNAL(readyRead()), this, SLOT(readyRead()));

            pluginStatus = "reconnect";
            emit updateStatus();
            qDebug() << "DLTMultimeterPlugin: reconnect" << interface;
        }
        else
        {
            // retry failed

            qDebug() << "DLTMultimeterPlugin: Failed to open interface" << interface;
            pluginStatus = "error";
            emit updateStatus();
        }
    }

}

void DLTMultimeterPlugin::calculateValue()
{
    if(rawData.length()!=14)
        return;

    value=calculateNumber(rawData[1]&0x7,rawData[2]&0xf);
    value=value*10+calculateNumber(rawData[3]&0x7,rawData[4]&0xf);
    value=value*10+calculateNumber(rawData[5]&0x7,rawData[6]&0xf);
    value=value*10+calculateNumber(rawData[7]&0x7,rawData[8]&0xf);

    if(rawData[1]&0x08) value*=-1;

    if(rawData[3]&0x08) value/=1000;
    if(rawData[5]&0x08) value/=100;
    if(rawData[7]&0x08) value/=10;

    if(rawData[10]&0x08) value/=1000; // milli
    if(rawData[9]&0x08) value/=1000; // mycro

    if(rawData[12]&0x08)
        unit = "A";
    else if(rawData[12]&0x04)
        unit = "V";
    else
        unit = "unknown";
}

int DLTMultimeterPlugin::calculateNumber(unsigned char a,unsigned char b)
{
    if(a==0x7 && b==0x0d)
        return 0;
    else if(a==0x0 && b==0x05)
        return 1;
    else if(a==0x5 && b==0x0b)
        return 2;
    else if(a==0x1 && b==0x0f)
        return 3;
    else if(a==0x2 && b==0x07)
        return 4;
    else if(a==0x3 && b==0x0e)
        return 5;
    else if(a==0x7 && b==0x0e)
        return 6;
    else if(a==0x1 && b==0x05)
        return 7;
    else if(a==0x7 && b==0x0f)
        return 8;
    else if(a==0x3 && b==0x0f)
        return 9;
    else
        return 0;

}

const QString &DLTMultimeterPlugin::getDltCtxId() const
{
    return dltCtxId;
}

void DLTMultimeterPlugin::setDltCtxId(const QString &newDltCtxId)
{
    dltCtxId = newDltCtxId;
}

const QString &DLTMultimeterPlugin::getDltAppId() const
{
    return dltAppId;
}

void DLTMultimeterPlugin::setDltAppId(const QString &newDltAppId)
{
    dltAppId = newDltAppId;
}

void DLTMultimeterPlugin::on()
{
    if(type==1) // Mason HCS-3302 USB
    {
        serialPort.write("SOUT0\r");

        qDebug() << "DLTMultimeterPlugin: power on";
        if(dltMiniServer)
            dltMiniServer->sendValue2(dltAppId,"PWR",powerName,"On");
    }
}

void DLTMultimeterPlugin::off()
{
    if(type==1) // Mason HCS-3302 USB
    {
        serialPort.write("SOUT1\r");

        qDebug() << "DLTMultimeterPlugin: power off";
        if(dltMiniServer)
            dltMiniServer->sendValue2(dltAppId,"PWR",powerName,"Off");
    }
}

void DLTMultimeterPlugin::setVoltage(float value)
{
    if(type==1) // Mason HCS-3302 USB
    {
        QString text;

        // Voltage set cmd syntax: VOLT### (### is the voltage in dV)
        text = QString("VOLT%L1\r").arg((int)(value*10), 3, 10, QLatin1Char('0'));

        if(!readVoltageOngoing)
        {
            serialPort.write(text.toLatin1());
        }
        else
        {
            voltageCmd = text;
        }

        //qDebug() << "DLTMultimeterPlugin: setVoltage" << text;
    }
}

void DLTMultimeterPlugin::backupGeometry()
{
    QSettings settings;
    settings.setValue("dlt_multimeter_plugin_geometry", window->saveGeometry());
}

void DLTMultimeterPlugin::restoreGeometry()
{
    QSettings settings;
    window->restoreGeometry(settings.value("dlt_multimeter_plugin_geometry").toByteArray());
}

void DLTMultimeterPlugin::injection(QString data)
{
    if(form)
        form->injection(data);
}
