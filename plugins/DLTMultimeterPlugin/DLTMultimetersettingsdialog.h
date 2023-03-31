#ifndef DLT_MULTIMETER_SETTINGSDIALOG_H
#define DLT_MULTIMETER_SETTINGSDIALOG_H

#include "DLTMultimeterPlugin.h"

#include <QDialog>

namespace Ui {
class DLTMultimeterSettingsDialog;
}

class DLTMultimeterSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DLTMultimeterSettingsDialog(QWidget *parent = nullptr);
    ~DLTMultimeterSettingsDialog();

    void restoreSettings(DLTMultimeterPlugin *templatePlugin);
    void backupSettings(DLTMultimeterPlugin *templatePlugin);

private:
    Ui::DLTMultimeterSettingsDialog *ui;
};

#endif // DLT_MULTIMETER_SETTINGSDIALOG_H
