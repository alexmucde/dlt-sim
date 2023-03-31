#ifndef DLT_SIM_PLUGIN_INTERFACE
#define DLT_SIM_PLUGIN_INTERFACE

#include <QString>
#include <QtPlugin>
#include <QMdiArea>
#include <QXmlStreamWriter>

class DLTMiniServer;

class DLTSimPluginInterface
{
public:
    virtual QString name() const = 0;
    virtual QString version() const = 0;
    virtual QString status() const = 0;
    virtual void start(DLTMiniServer *dltMiniServer) = 0;
    virtual void stop() = 0;
    virtual void show(QMdiArea *mdiArea) = 0;
    virtual void update() = 0;
    virtual void hide() = 0;
    virtual void injection(QString data) = 0;

    virtual void clearSettings() = 0;
    virtual void readSettings(const QString &filename,const int instance) = 0;
    virtual void writeSettings(QXmlStreamWriter &xml,const int instance) const = 0;
    virtual void settings(QWidget *parent) = 0;
    virtual QObject * getObject () = 0;

};
Q_DECLARE_INTERFACE(DLTSimPluginInterface, "de.alexmucde.DLTSim.PluginInterface/1.0");

#endif // DLT_SIM_PLUGIN_INTERFACE
