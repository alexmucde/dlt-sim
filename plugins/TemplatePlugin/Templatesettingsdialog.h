#ifndef TEMPLATE_SETTINGSDIALOG_H
#define TEMPLATE_SETTINGSDIALOG_H

#include "TemplatePlugin.h"

#include <QDialog>

namespace Ui {
class TemplateSettingsDialog;
}

class TemplateSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TemplateSettingsDialog(QWidget *parent = nullptr);
    ~TemplateSettingsDialog();

    void restoreSettings(TemplatePlugin *templatePlugin);
    void backupSettings(TemplatePlugin *templatePlugin);

private:
    Ui::TemplateSettingsDialog *ui;
};

#endif // TEMPLATE_SETTINGSDIALOG_H
