#ifndef SETTINGS_H
#define SETTINGS_H

#include "pluginmodel.h"

#include <QDialog>

namespace Ui {
class DialogSettings;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    void restoreSettings(PluginModel &model);
    void backupSettings(PluginModel &model);

private slots:
    void on_pushButtonProject_clicked();


    void on_groupBoxAutoload_clicked(bool checked);

    void on_checkBoxAutostart_clicked(bool checked);

private:
    Ui::DialogSettings *ui;
};

#endif // SETTINGS_H
