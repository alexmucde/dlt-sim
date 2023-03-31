#ifndef DLT_RELAIS_SETTINGSDIALOG_H
#define DLT_RELAIS_SETTINGSDIALOG_H

#include <QDialog>

class DLTRelaisPlugin;

namespace Ui {
class DLTRelaisSettingsDialog;
}

class DLTRelaisSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DLTRelaisSettingsDialog(QWidget *parent = nullptr);
    ~DLTRelaisSettingsDialog();

    void restoreSettings(DLTRelaisPlugin *dltRelais);
    void backupSettings(DLTRelaisPlugin *dltRelais);

private:
    Ui::DLTRelaisSettingsDialog *ui;
};

#endif // DLT_RELAIS_SETTINGSDIALOG_H
