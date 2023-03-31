#ifndef DLT_CAN_PLUGIN_H
#define DLT_CAN_PLUGIN_H

#include "plugininterface.h"

class DLTCanPluginForm;
class DLTMiniServer;

#define DLT_CAN_PLUGIN_NAME "DLT CAN Plugin"
#define DLT_CAN_PLUGIN_VERSION "0.0.1"

class DLTCanPlugin : public QObject, DLTSimPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "de.alexmucde.DLTSim.PlugignInterface" FILE "DLTCanPlugin.json")
    Q_INTERFACES(DLTSimPluginInterface)
public:
    DLTCanPlugin();
    ~DLTCanPlugin();

    /* Plugin Interface */
    QString name() const { return DLT_CAN_PLUGIN_NAME; }
    QString version() const { return DLT_CAN_PLUGIN_VERSION; }
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
    DLTCanPluginForm *form;
    QMdiSubWindow *window;
};

#endif // DLT_CAN_PLUGIN_H
