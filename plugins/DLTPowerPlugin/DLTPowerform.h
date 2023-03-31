#ifndef DLT_POWER_FORM_H
#define DLT_POWER_FORM_H

#include <QWidget>

class DLTPowerPlugin;

namespace Ui {
class DLTPowerForm;
}

class DLTPowerForm : public QWidget
{
    Q_OBJECT

public:
    explicit DLTPowerForm(DLTPowerPlugin *plugin, QWidget *parent = nullptr);
    ~DLTPowerForm();

    void valueMultimeter(QString value,QString unit);

    void update();

    void injection(QString data);

private slots:
    void on_checkBoxOn_clicked(bool checked);

    void on_pushButtonSet_clicked();

private:
    Ui::DLTPowerForm *ui;

    DLTPowerPlugin *plugin;
};

#endif // DLT_POWER_FORM_H
