#ifndef DLT_CAN_PLUGIN_H
#define DLT_CAN_PLUGIN_H

#include "plugininterface.h"

#include <QTimer>
#include <QSerialPort>

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

    const QString &getInterface() const;
    void setInterface(const QString &newInterface);

    const QString &getTitle() const;
    void setTitle(const QString &newTitle);

    void sendMessage(unsigned short id,unsigned char *data,int length);
    void startCyclicMessage1(int timeout);
    void startCyclicMessage2(int timeout);
    void setCyclicMessage1(unsigned short id,QByteArray data);
    void setCyclicMessage2(unsigned short id,QByteArray data);
    void stopCyclicMessage1();
    void stopCyclicMessage2();

    unsigned short getMessageId() const;
    void setMessageId(unsigned short value);

    unsigned short getCyclicMessageId1() const;
    void setCyclicMessageId1(unsigned short value);

    unsigned short getCyclicMessageId2() const;
    void setCyclicMessageId2(unsigned short value);

    QByteArray getMessageData() const;
    void setMessageData(const QByteArray &value);

    QByteArray getCyclicMessageData1() const;
    void setCyclicMessageData1(const QByteArray &value);

    QByteArray getCyclicMessageData2() const;
    void setCyclicMessageData2(const QByteArray &value);

    int getCyclicMessageTimeout1() const;
    void setCyclicMessageTimeout1(int value);

    int getCyclicMessageTimeout2() const;
    void setCyclicMessageTimeout2(int value);

    bool getCyclicMessageActive1() const;
    void setCyclicMessageActive1(bool value);

    bool getCyclicMessageActive2() const;
    void setCyclicMessageActive2(bool value);

    ulong getRxMsgCounter() const;

    ulong getTxMsgCounter() const;

signals:
    void updateStatus();

private slots:

    void readyRead();

    // Watchdog Timeout
    void timeout();

    void timeoutCyclicMessage1();
    void timeoutCyclicMessage2();

private:

    void message(unsigned int id,QString direction,QByteArray data);

    QString pluginStatus;
    DLTMiniServer *dltMiniServer;

    // settings
    QString title;
    QString interface;

    // temporary data
    QSerialPort serialPort;
    QTimer timer;
    QTimer timerRequest;
    unsigned int watchDogCounter,watchDogCounterLast;

    QString interfaceSerialNumber;
    ushort interfaceProductIdentifier;
    ushort interfaceVendorIdentifier;
    bool active;

    QByteArray serialData;

    QByteArray rawData;
    bool startFound;

    bool cyclicMessageActive1,cyclicMessageActive2;
    int cyclicMessageTimeout1,cyclicMessageTimeout2;
    unsigned short messageId,cyclicMessageId1,cyclicMessageId2;
    QByteArray messageData,cyclicMessageData1,cyclicMessageData2;

    QTimer timerCyclicMessage1;
    QTimer timerCyclicMessage2;

    ulong rxMsgCounter, txMsgCounter;

    // Visualisation
    DLTCanPluginForm *form;
    QMdiSubWindow *window;
};

#endif // DLT_CAN_PLUGIN_H
