#include "DLTRelaissettingsdialog.h"
#include "DLTRelaisPlugin.h"
#include "dltminiserver.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QSerialPortInfo>
#include <QSettings>

DLTRelaisPlugin::DLTRelaisPlugin()
{
    form = 0;
    window = 0;
    dltMiniServer = 0;

    clearSettings();
}

DLTRelaisPlugin::~DLTRelaisPlugin()
{
    stop();
}

void DLTRelaisPlugin::clearSettings()
{
    // clear settings
    type = 0;
    title = DLT_RELAIS_PLUGIN_NAME;
    for(int num=0;num<4;num++)
        relaisName[num] = QString("Relais%1").arg(num+1);

    interfaceSerialNumber = "";
    interfaceProductIdentifier = 0;
    interfaceVendorIdentifier = 0;

    dltAppId = "DLTR";
    dltCtxId = "REL";
}

void DLTRelaisPlugin::readSettings(const QString &filename,const int instance)
{
    // read settings from XML file
    bool isDLTRelais = false;

    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
             return;

    QXmlStreamReader xml(&file);

    while (!xml.atEnd())
    {
          xml.readNext();

          if(xml.isStartElement())
          {
              if(isDLTRelais)
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
                          isDLTRelais = false;
                  }
                  else if(xml.name() == QString("title"))
                  {
                      title = xml.readElementText();
                  }
                  else if(xml.name() == QString("relaisName1"))
                  {
                      relaisName[0] = xml.readElementText();
                  }
                  else if(xml.name() == QString("relaisName2"))
                  {
                      relaisName[1] = xml.readElementText();
                  }
                  else if(xml.name() == QString("relaisName3"))
                  {
                      relaisName[2] = xml.readElementText();
                  }
                  else if(xml.name() == QString("relaisName4"))
                  {
                      relaisName[3] = xml.readElementText();
                  }
                  else if(xml.name() == QString("interface"))
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
                  else if(xml.name() == QString("type"))
                  {
                      type = xml.readElementText().toInt();
                  }
              }
              else if(xml.name() == QString("Name"))
              {
                  if( xml.readElementText() == QString(DLT_RELAIS_PLUGIN_NAME))
                    isDLTRelais = true;
              }
          }
          else if(xml.isEndElement())
          {
              /* Connection, plugin and filter */
              if(xml.name() == QString("Plugin"))
              {
                    isDLTRelais = false;
              }
          }
    }
    if (xml.hasError())
    {
         qDebug() << "Error in processing filter file" << filename << xml.errorString();
    }

    file.close();
}

void DLTRelaisPlugin::writeSettings(QXmlStreamWriter &xml,const int instance) const
{
    // Plugin type and version
    xml.writeTextElement("Name",DLT_RELAIS_PLUGIN_NAME);
    xml.writeTextElement("Version",DLT_RELAIS_PLUGIN_VERSION);
    xml.writeTextElement("Instance",QString("%1").arg(instance));

    // Settings
    xml.writeStartElement("Settings");
        xml.writeTextElement("title",title);
        xml.writeTextElement("relaisName1",relaisName[0]);
        xml.writeTextElement("relaisName2",relaisName[1]);
        xml.writeTextElement("relaisName3",relaisName[2]);
        xml.writeTextElement("relaisName4",relaisName[3]);
        xml.writeTextElement("interface",interface);
        xml.writeTextElement("interfaceSerialNumber",QSerialPortInfo(interface).serialNumber());
        xml.writeTextElement("interfaceProductIdentifier",QString("%1").arg(QSerialPortInfo(interface).productIdentifier()));
        xml.writeTextElement("interfaceVendorIdentifier",QString("%1").arg(QSerialPortInfo(interface).vendorIdentifier()));
        xml.writeTextElement("type",QString("%1").arg(type));
    xml.writeEndElement(); // Settings
}

void DLTRelaisPlugin::settings(QWidget *parent)
{
    DLTRelaisSettingsDialog dlg(parent);
    dlg.restoreSettings(this);

    if(dlg.exec()==QDialog::Accepted)
    {
        dlg.backupSettings(this);
    }
}


int DLTRelaisPlugin::getType() const
{
    return type;
}

void DLTRelaisPlugin::setType(int newType)
{
    type = newType;
}

const QString &DLTRelaisPlugin::getInterface() const
{
    return interface;
}

void DLTRelaisPlugin::setInterface(const QString &newInterface)
{
    interface = newInterface;
}

const QString & DLTRelaisPlugin::getRelaisName(int num) const
{
    return relaisName[num-1];
}

void DLTRelaisPlugin::setRelaisName(const QString &newRelaisName,int num)
{
    relaisName[num-1] = newRelaisName;
}

void DLTRelaisPlugin::readyRead()
{
    // data on serial port was received

    // loop as long as data is available
    while(serialPort.bytesAvailable())
    {
        char data[256];

        // read one line form serial port
        qint64 size = serialPort.readLine(data,sizeof(data));

        if(size>0)
        {
            // line is not empty
            //qDebug() << "DLTRelais: readLine" << data;

            if(QString(data) == "WD\r\n")
            {
                // watchdog message received
                watchDogCounter++;
            }
            else
            {
                // all other messages forward to status signal
                //status(QString(data));
            }
        }
    }
}

void DLTRelaisPlugin::timeout()
{
    // watchdog timeout

    // check if watchdog was triggered between last call
    if(watchDogCounter!=watchDogCounterLast)
    {
        watchDogCounterLast = watchDogCounter;
        pluginStatus = "running";
        updateStatus();
    }
    else
    {
        // no watchdog was received
        qDebug() << "DLTRelais: Watchdog expired try to reconnect" ;

        // if serial port is open close serial port
        if(serialPort.isOpen())
        {
            serialPort.close();
            disconnect(&serialPort, SIGNAL(readyRead()), this, SLOT(readyRead()));
        }

        // check if port name has changed
        // checkPortName();

        // try to reopen serial port
        if(serialPort.open(QIODevice::ReadWrite)==true)
        {
            // retry was succesful

            // prevent flash mode of Wemos D1 mini
            serialPort.setDataTerminalReady(false);

            // connect slot to receive data from serial port
            connect(&serialPort, SIGNAL(readyRead()), this, SLOT(readyRead()));
            pluginStatus = "reconnect";
            updateStatus();
            qDebug() << "DLTRelais: reconnect" << interface;
        }
        else
        {
            // retry failed
            qDebug() << "DLTRelais: Failed to open interface" << interface;
            pluginStatus = "error";
            updateStatus();
        }
    }
}

const QString &DLTRelaisPlugin::getTitle() const
{
    return title;
}

void DLTRelaisPlugin::setTitle(const QString &newTitle)
{
    title = newTitle;
}

void DLTRelaisPlugin::start(DLTMiniServer *newDltMiniServer)
{
    dltMiniServer = newDltMiniServer;

    // start communication
    // checkPortName();

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

        // prevent flash mode of Wemos D1 mini and Wemos D1, not for Arduino boards
        if(type==2) // Arduino Boards
            serialPort.setDataTerminalReady(true);
        else
            serialPort.setDataTerminalReady(false);

        // connect slot to receive data from serial port
        connect(&serialPort, SIGNAL(readyRead()), this, SLOT(readyRead()));

        //status(QString("started"));
        qDebug() << "DLTRelais: started" << interface;
    }
    else
    {
        // open failed
        qDebug() << "DLTRelais: Failed to open interface" << interface;
        pluginStatus = "error";
        updateStatus();
    }

    // connect slot watchdog timer and start watchdog timer
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer.start(5000);
    watchDogCounter = 0;
    watchDogCounterLast = 0;

    qDebug() << "DLTRelais: started" << interface;
    pluginStatus = "started";
}

void DLTRelaisPlugin::stop()
{
    // stop communication
    pluginStatus = "stopped";
    updateStatus();
    qDebug() << "DLTRelais: stopped" << interface;

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

void DLTRelaisPlugin::trigger(int num,unsigned int duration)
{
    // trigger a Relais for 500ms
    qDebug() << "DLTRelais: trigger" << num << duration;

    serialPort.write(QString("R%1T%2\n").arg(num).arg(duration).toLatin1());

    if(dltMiniServer)
        dltMiniServer->sendValue3(dltAppId,dltCtxId,relaisName[num-1],"Trigger",QString("%1").arg(duration));
}

void DLTRelaisPlugin::on(int num)
{
    // set Relais to on
    qDebug() << "DLTRelais: on" << num;

    if(num==1)
        serialPort.write("R11\n");
    else if(num==2)
        serialPort.write("R21\n");
    else if(num==3)
        serialPort.write("R31\n");
    else if(num==4)
        serialPort.write("R41\n");

    if(dltMiniServer)
        dltMiniServer->sendValue2(dltAppId,dltCtxId,relaisName[num-1],"On");
}

void DLTRelaisPlugin::off(int num)
{
    // set Relais to off
    qDebug() << "DLTRelais: off" << num;

    if(num==1)
        serialPort.write("R10\n");
    else if(num==2)
        serialPort.write("R20\n");
    else if(num==3)
        serialPort.write("R30\n");
    else if(num==4)
        serialPort.write("R40\n");

    if(dltMiniServer)
        dltMiniServer->sendValue2(dltAppId,dltCtxId,relaisName[num-1],"Off");
}

void DLTRelaisPlugin::show(QMdiArea *mdiArea)
{
    if(window == 0)
        window = new QMdiSubWindow;
    if(form == 0)
        form = new DLTRelaisForm(this,window);
    window->setWidget(form);
    mdiArea->addSubWindow(window);
    window->show();
    restoreGeometry();
}

void DLTRelaisPlugin::update()
{
    if(form)
        form->update();
}

void DLTRelaisPlugin::hide()
{
    if(window)
        window->hide();
    delete window;
    window = 0;
    form = 0;
}

void DLTRelaisPlugin::backupGeometry()
{
    QSettings settings;
    settings.setValue("dlt_relais_plugin_geometry", window->saveGeometry());
}

void DLTRelaisPlugin::restoreGeometry()
{
    QSettings settings;
    window->restoreGeometry(settings.value("dlt_relais_plugin_geometry").toByteArray());
}

void DLTRelaisPlugin::injection(QString data)
{
    if(form)
        form->injection(data);
}

