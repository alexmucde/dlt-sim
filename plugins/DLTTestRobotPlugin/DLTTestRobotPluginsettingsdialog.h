#ifndef DLT_TEST_ROBOT_SETTINGSDIALOG_H
#define DLT_TEST_ROBOT_SETTINGSDIALOG_H

#include "DLTTestRobotPlugin.h"

#include <QDialog>

namespace Ui {
class DLTTestRobotPluginSettingsDialog;
}

class DLTTestRobotPluginSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DLTTestRobotPluginSettingsDialog(QWidget *parent = nullptr);
    ~DLTTestRobotPluginSettingsDialog();

    void restoreSettings(DLTTestRobotPlugin *templatePlugin);
    void backupSettings(DLTTestRobotPlugin *templatePlugin);

private slots:
    void on_pushButtonAutoloadtestsFilenameSelect_clicked();

private:
    Ui::DLTTestRobotPluginSettingsDialog *ui;
};

#endif // DLT_TEST_ROBOT_SETTINGSDIALOG_H
