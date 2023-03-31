#ifndef DLT_CAN_PLUGIN_FORM_H
#define DLT_CAN_PLUGIN_FORM_H

#include <QWidget>

class DLTCanPlugin;

namespace Ui {
class DLTCanPluginForm;
}

class DLTCanPluginForm : public QWidget
{
    Q_OBJECT

public:
    explicit DLTCanPluginForm(DLTCanPlugin *plugin, QWidget *parent = nullptr);
    ~DLTCanPluginForm();

private:
    Ui::DLTCanPluginForm *ui;

    DLTCanPlugin *plugin;
};

#endif // DLT_CAN_PLUGIN_FORM_H
