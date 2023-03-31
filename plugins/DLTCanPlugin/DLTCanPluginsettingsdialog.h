#ifndef DLT_CAN_PLUGIN_SETTINGSDIALOG_H
#define DLT_CAN_PLUGIN_SETTINGSDIALOG_H

#include "DLTCanPlugin.h"

#include <QDialog>

namespace Ui {
class DLTCanPluginSettingsDialog;
}

class DLTCanPluginSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DLTCanPluginSettingsDialog(QWidget *parent = nullptr);
    ~DLTCanPluginSettingsDialog();

    void restoreSettings(DLTCanPlugin *templatePlugin);
    void backupSettings(DLTCanPlugin *templatePlugin);

private:
    Ui::DLTCanPluginSettingsDialog *ui;
};

#endif // DLT_CAN_PLUGIN_SETTINGSDIALOG_H
