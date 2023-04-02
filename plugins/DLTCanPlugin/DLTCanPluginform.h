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

    void update();

private slots:
    void on_pushButtonSend_clicked();

    void on_checkBoxSendCyclicMsg1Active_clicked(bool checked);

    void on_checkBoxSendCyclicMsg2Active_clicked(bool checked);

private:
    void closeEvent(QCloseEvent *event);

    Ui::DLTCanPluginForm *ui;
    DLTCanPlugin *plugin;
};

#endif // DLT_CAN_PLUGIN_FORM_H
