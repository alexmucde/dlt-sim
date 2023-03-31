#include "TemplatePlugin.h"
#include "Templatesettingsdialog.h"
#include "Templateform.h"

#include <QFile>
#include <QMdiSubWindow>
#include <QSettings>

TemplatePlugin::TemplatePlugin()
{
    form = 0;
    window = 0;
}

TemplatePlugin::~TemplatePlugin()
{

}

void TemplatePlugin::clearSettings()
{

}

void TemplatePlugin::settings(QWidget *parent)
{
    TemplateSettingsDialog dlg(parent);
    dlg.restoreSettings(this);

    if(dlg.exec()==QDialog::Accepted)
    {
        dlg.backupSettings(this);
    }
}

void TemplatePlugin::readSettings(const QString &filename,const int instance)
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
                  if( xml.readElementText() == QString(TEMPLATE_PLUGIN_NAME))
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

void TemplatePlugin::writeSettings(QXmlStreamWriter &xml,const int instance) const
{
    // Plugin type and version
    xml.writeTextElement("Name",TEMPLATE_PLUGIN_NAME);
    xml.writeTextElement("Version",TEMPLATE_PLUGIN_VERSION);
    xml.writeTextElement("Instance",QString("%1").arg(instance));

    // Settings
    xml.writeStartElement("Settings");
    xml.writeEndElement(); // Settings
}

void TemplatePlugin::start(DLTMiniServer *dltMiniServer)
{
    qDebug() << "Template: started";
    pluginStatus = "started";
}

void TemplatePlugin::stop()
{
    qDebug() << "Template: stopped";
    pluginStatus = "stopped";
}

void TemplatePlugin::show(QMdiArea *mdiArea)
{
    if(window == 0)
        window = new QMdiSubWindow;
    if(form == 0)
        form = new TemplateForm(this,window);
    window->setWidget(form);
    mdiArea->addSubWindow(window);
    window->show();
    restoreGeometry();
}

void TemplatePlugin::update()
{
    if(form)
        form->update();
}

void TemplatePlugin::hide()
{
    if(window)
        window->hide();
    delete window;
    window = 0;
    form = 0;
}

void TemplatePlugin::backupGeometry()
{
    QSettings settings;
    settings.setValue("template_plugin_geometry", window->saveGeometry());
}

void TemplatePlugin::restoreGeometry()
{
    QSettings settings;
    window->restoreGeometry(settings.value("template_plugin_geometry").toByteArray());
}

void TemplatePlugin::injection(QString data)
{

}
