#include "Templateform.h"
#include "ui_Templateform.h"
#include "TemplatePlugin.h"

TemplateForm::TemplateForm(TemplatePlugin *newPlugin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TemplateForm)
{
    plugin = newPlugin;

    ui->setupUi(this);
}

TemplateForm::~TemplateForm()
{
    plugin->backupGeometry();

    delete ui;
}
