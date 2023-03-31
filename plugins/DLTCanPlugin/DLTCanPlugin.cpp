#include "DLTCanPlugin.h"
#include "DLTCanPluginsettingsdialog.h"
#include "DLTCanPluginform.h"

#include <QFile>
#include <QMdiSubWindow>
#include <QSettings>

DLTCanPlugin::DLTCanPlugin()
{
    form = 0;
    window = 0;
}

DLTCanPlugin::~DLTCanPlugin()
{

}

void DLTCanPlugin::clearSettings()
{

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
    xml.writeEndElement(); // Settings
}

void DLTCanPlugin::start(DLTMiniServer *dltMiniServer)
{
    qDebug() << "Template: started";
    pluginStatus = "started";
}

void DLTCanPlugin::stop()
{
    qDebug() << "Template: stopped";
    pluginStatus = "stopped";
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

void DLTCanPlugin::injection(QString data)
{

}
