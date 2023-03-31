#ifndef TEMPLATE_PLUGIN_H
#define TEMPLATE_PLUGIN_H

#include "plugininterface.h"

class TemplateForm;
class DLTMiniServer;

#define TEMPLATE_PLUGIN_NAME "Template Plugin"
#define TEMPLATE_PLUGIN_VERSION "0.0.1"

class TemplatePlugin : public QObject, DLTSimPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "de.alexmucde.DLTSim.PlugignInterface" FILE "TemplatePlugin.json")
    Q_INTERFACES(DLTSimPluginInterface)
public:
    TemplatePlugin();
    ~TemplatePlugin();

    /* Plugin Interface */
    QString name() const { return TEMPLATE_PLUGIN_NAME; }
    QString version() const { return TEMPLATE_PLUGIN_VERSION; }
    QString status() const { return pluginStatus; }
    void start(DLTMiniServer *dltMiniServer);
    void stop();
    void show(QMdiArea *mdiArea);
    void update();
    void hide();
    void injection(QString data);
    void clearSettings();
    void readSettings(const QString &filename,const int instance);
    void writeSettings(QXmlStreamWriter &xml,const int instance) const ;
    void settings(QWidget *parent);
    QObject* getObject() { return this; }

    void backupGeometry();
    void restoreGeometry();

signals:
    void updateStatus();

private:
    QString pluginStatus;

    // Visualisation
    TemplateForm *form;
    QMdiSubWindow *window;
};

#endif // TEMPLATE_PLUGIN_H
