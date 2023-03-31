#ifndef DLT_RELAIS_PLUGIN_H
#define DLT_RELAIS_PLUGIN_H

#include "plugininterface.h"
#include "DLTRelaisform.h"

#include <QTimer>
#include <QSerialPort>
#include <QMdiSubWindow>

class DLTMiniServer;

#define DLT_RELAIS_PLUGIN_NAME "DLT Relais Plugin"
#define DLT_RELAIS_PLUGIN_VERSION "0.0.1"

class DLTRelaisPlugin : public QObject, DLTSimPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "de.alexmucde.DLTSim.PlugignInterface" FILE "dltrelaisplugin.json")
    Q_INTERFACES(DLTSimPluginInterface)
public:
    DLTRelaisPlugin();
    ~DLTRelaisPlugin();

    /* Plugin Interface */
    QString name() const { return DLT_RELAIS_PLUGIN_NAME; }
    QString version() const { return DLT_RELAIS_PLUGIN_VERSION; }
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

    void trigger(int num,unsigned int duration);
    void on(int num);
    void off(int num);

    /* Setter and Getters */
    int getType() const;
    void setType(int newType);
    const QString &getInterface() const;
    void setInterface(const QString &newInterface);
    const QString &getRelaisName(int num) const;
    void setRelaisName(const QString &newRelaisName,int num);
    const QString &getTitle() const;
    void setTitle(const QString &newTitle);

    void backupGeometry();
    void restoreGeometry();

private slots:

    // Serial data available
    void readyRead();

    // Watchdog Timeout
    void timeout();

signals:
    void updateStatus();

private:

    // Temporary variables
    QSerialPort serialPort;
    QTimer timer;
    unsigned int watchDogCounter,watchDogCounterLast;
    QString pluginStatus;
    DLTMiniServer *dltMiniServer;

    // Settings
    int type;
    QString title;
    QString interface;
    QString interfaceSerialNumber;
    ushort interfaceProductIdentifier;
    ushort interfaceVendorIdentifier;
    QString relaisName[4];
    QString dltAppId,dltCtxId;

    // Visualisation
    DLTRelaisForm *form;
    QMdiSubWindow *window;
};

#endif // DLT_RELAIS_PLUGIN_H
