#ifndef DLT_MULTIMETER_FORM_H
#define DLT_MULTIMETER_FORM_H

#include <QWidget>

class DLTMultimeterPlugin;

namespace Ui {
class DLTMultimeterForm;
}

class DLTMultimeterForm : public QWidget
{
    Q_OBJECT

public:
    explicit DLTMultimeterForm(DLTMultimeterPlugin *plugin, QWidget *parent = nullptr);
    ~DLTMultimeterForm();

    void valueMultimeter(QString value,QString unit);

    void update();

    void injection(QString data);

private slots:
    void on_checkBoxOn_clicked(bool checked);

    void on_pushButtonSet_clicked();

private:
    Ui::DLTMultimeterForm *ui;

    DLTMultimeterPlugin * plugin;
};

#endif // DLT_MULTIMETER_FORM_H
