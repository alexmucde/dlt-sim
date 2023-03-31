#ifndef DLT_TEST_ROBOT_FORM_H
#define DLT_TEST_ROBOT_FORM_H

#include <QWidget>
#include <QFile>

class DLTTestRobotPlugin;

namespace Ui {
class DLTTestRobotPluginForm;
}

class DLTTestRobotPluginForm : public QWidget
{
    Q_OBJECT

public:
    explicit DLTTestRobotPluginForm(DLTTestRobotPlugin *plugin, QWidget *parent = nullptr);
    ~DLTTestRobotPluginForm();

private slots:
    void on_pushButtonTestFilename_clicked();

    void on_pushButtonStartTests_clicked();

    void on_pushButtonStopTests_clicked();

    void report(QString text);
    void reportSummary(QString text);
    void command(int allTestRepeatNum,int allTestRepeat, int testRepeatNum,int testRepeat,int testNum, int commandNum,int commandCount, QString text);
    void statusTests(QString text);

private:
    Ui::DLTTestRobotPluginForm *ui;

    DLTTestRobotPlugin *plugin;

    QFile reportFile;
    QStringList reportSummaryList;
    int reportFailedCounter,reportSuccessCounter;

    void loadTests(QString fileName);

    void startTests();
    void stopTests();

    void writeSummaryToReport();

};

#endif // DLT_TEST_ROBOT_FORM_H
