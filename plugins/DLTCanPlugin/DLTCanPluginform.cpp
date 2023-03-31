#include "DLTCanPluginform.h"
#include "ui_DLTCanPluginform.h"
#include "DLTCanPlugin.h"

DLTCanPluginForm::DLTCanPluginForm(DLTCanPlugin *newPlugin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DLTCanPluginForm)
{
    plugin = newPlugin;

    ui->setupUi(this);
}

DLTCanPluginForm::~DLTCanPluginForm()
{
    plugin->backupGeometry();

    delete ui;
}
