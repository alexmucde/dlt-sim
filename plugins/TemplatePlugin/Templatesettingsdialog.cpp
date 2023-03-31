#include "Templatesettingsdialog.h"
#include "ui_Templatesettingsdialog.h"

TemplateSettingsDialog::TemplateSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TemplateSettingsDialog)
{
    ui->setupUi(this);
}

TemplateSettingsDialog::~TemplateSettingsDialog()
{
    delete ui;
}

void TemplateSettingsDialog::restoreSettings(TemplatePlugin *templatePlugin)
{

}

void TemplateSettingsDialog::backupSettings(TemplatePlugin *templatePlugin)
{

}
