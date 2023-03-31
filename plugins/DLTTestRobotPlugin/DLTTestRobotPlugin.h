#ifndef DLT_TEST_ROBOT_PLUGIN_H
#define DLT_TEST_ROBOT_PLUGIN_H

#include "plugininterface.h"
#include "DLTTest.h"

class DLTTestRobotPluginForm;
class DLTMiniServer;

#define DLT_TEST_ROBOT_PLUGIN_NAME "DLT Test Robot Plugin"
#define DLT_TEST_ROBOT_PLUGIN_VERSION "0.0.1"

class DLTTestRobotPlugin : public QObject, DLTSimPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "de.alexmucde.DLTSim.PlugignInterface" FILE "DLTTestRobotPlugin.json")
    Q_INTERFACES(DLTSimPluginInterface)
public:
    DLTTestRobotPlugin();
    ~DLTTestRobotPlugin();

    /* Plugin Interface */
    QString name() const { return DLT_TEST_ROBOT_PLUGIN_NAME; }
    QString version() const { return DLT_TEST_ROBOT_PLUGIN_VERSION; }
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

    quint32 getDltViewerTestRobotPluginPort() const;
    void setDltViewerTestRobotPluginPort(quint32 newDltViewerTestRobotPluginPort);

    const QString &getAutoloadTestFilename() const;
    void setAutoloadTestFilename(const QString &newAutoloadTestFilename);

    bool getAutoloadTests() const;
    void setAutoloadTests(bool newAutoloadTests);

    bool getAutostartTests() const;
    void setAutostartTests(bool newAutostartTests);

    QStringList readTests(const QString &filename);

    void send(QString text);

    int size() { return tests.size(); }
    QString testId(int num) { if(num>=0 && num<tests.length()) return tests[num].getId(); else return QString(); }
    QString testDescription(int num) { if(num>=0 && num<tests.length()) return tests[num].getDescription(); else return QString(); }
    int testSize(int num) { if(num>=0 && num<tests.length()) return tests[num].size(); else return 0; }

    void startTest(int num = -1,int repeat = 1);
    void stopTest();

    bool getFailed() const;

    DLTTest getTest(int num) { if(num>=0 && num<tests.length()) return tests[num]; else return DLTTest(); }

    const QString &getTestsFilename() const;

    const QString &getVersion() const;

    bool getAllTests() const;

    int getFailedTestCommands() const;

    DLTMiniServer *dltMiniServer;

    const QString &getDltAppId() const;
    void setDltAppId(const QString &newDltAppId);

    const QString &getDltCtxId() const;
    void setDltCtxId(const QString &newDltCtxId);

signals:
    void updateStatus();

    void status(QString text);
    void statusTests(QString text);
    void report(QString text);
    void reportSummary(QString text);
    void command(int allTestRepeatNum,int allTestRepeat, int testRepeatNum,int testRepeat,int testNum, int commandNum,int commandCount, QString text);

private slots:

    void hostFound();
    void readyRead();
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError);

    void timeout();

private:
    QString pluginStatus;

    // Settings
    quint32 dltViewerTestRobotPluginPort;
    QString autoloadTestFilename;
    bool autoloadTests;
    bool autostartTests;

    void successTestCommand();
    void failedTestCommand();

    enum {Prerun,Running,Postrun,Finished} state;

    void runTest();
    bool nextTest();

    QTimer timer;
    unsigned int watchDogCounter,watchDogCounterLast;

    QTcpSocket tcpSocket;

    QList<DLTTest> tests;

    QString testsFilename;

    QString testsVersion;

    int allTestRepeat;
    int allTestRepeatNum;

    bool allTests;
    int testCount;
    int testNum;

    int testRepeat;
    int testRepeatNum;

    int commandCount;
    int commandNum;

    bool failed;
    int failedTestCommands;

    QString dltAppId,dltCtxId;

    // Visualisation
    DLTTestRobotPluginForm *form;
    QMdiSubWindow *window;
};

#endif // DLT_TEST_ROBOT_PLUGIN_H
