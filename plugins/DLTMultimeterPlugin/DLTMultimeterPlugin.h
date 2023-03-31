#ifndef DLT_MULTIMETER_PLUGIN_H
#define DLT_MULTIMETER_PLUGIN_H

#include "plugininterface.h"

#include <QSerialPort>
#include <QTimer>

class DLTMultimeterForm;
class DLTMiniServer;

#define DLT_MULTIMETER_PLUGIN_NAME "DLT Multimeter Plugin"
#define DLT_MULTIMETER_PLUGIN_VERSION "0.0.1"

class DLTMultimeterPlugin : public QObject, DLTSimPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "de.alexmucde.DLTSim.PlugignInterface" FILE "DLTMultimeterPlugin.json")
    Q_INTERFACES(DLTSimPluginInterface)
public:
    DLTMultimeterPlugin();
    ~DLTMultimeterPlugin();

    /* Plugin Interface */
    QString name() const { return DLT_MULTIMETER_PLUGIN_NAME; }
    QString version() const { return DLT_MULTIMETER_PLUGIN_VERSION; }
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

    void checkPortName();

    QString getInterface() { return interface; }
    void setInterface(QString interface) { this->interface = interface; }

    int getType() { return type; }
    void setType(int type) { this->type = type; }

    void on();
    void off();
    void setVoltage(float value);

    QString getPowerName() { return powerName; }
    void setPowerName(const QString &powerName) { this->powerName = powerName; }

    float getSubstractCurrent() const;
    void setSubstractCurrent(float newSubstractCurrent);

    void backupGeometry();
    void restoreGeometry();

    const QString &getDltAppId() const;
    void setDltAppId(const QString &newDltAppId);

    const QString &getDltCtxId() const;
    void setDltCtxId(const QString &newDltCtxId);

signals:
    void updateStatus();

private slots:

    void readyRead();

    // Watchdog Timeout
    void timeout();

    void timeoutRequest();

private:
    QString pluginStatus;
    DLTMiniServer *dltMiniServer;

    // Visualisation
    DLTMultimeterForm *form;
    QMdiSubWindow *window;

    QSerialPort serialPort;
    QTimer timer;
    QTimer timerRequest;
    unsigned int watchDogCounter,watchDogCounterLast;

    int type;
    QString interface;
    QString interfaceSerialNumber;
    ushort interfaceProductIdentifier;
    ushort interfaceVendorIdentifier;
    QString powerName;
    float substractCurrent;

    QByteArray serialData;

    QByteArray rawData;
    float value;
    QString unit;

    void calculateValue();
    int calculateNumber(unsigned char a,unsigned char b);

    bool readVoltageOngoing;
    QString voltageCmd;

    QString dltAppId, dltCtxId;
};

#endif // DLT_MULTIMETER_PLUGIN_H
