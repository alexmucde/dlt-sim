#ifndef DLT_POWER_SETTINGSDIALOG_H
#define DLT_POWER_SETTINGSDIALOG_H

#include "DLTPowerPlugin.h"

#include <QDialog>

namespace Ui {
class DLTPowerSettingsDialog;
}

class DLTPowerSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DLTPowerSettingsDialog(QWidget *parent = nullptr);
    ~DLTPowerSettingsDialog();

    void restoreSettings(DLTPowerPlugin *templatePlugin);
    void backupSettings(DLTPowerPlugin *templatePlugin);

private:
    Ui::DLTPowerSettingsDialog *ui;
};

#endif // DLT_POWER_SETTINGSDIALOG_H
