#ifndef DLT_RELAIS_FORM_H
#define DLT_RELAIS_FORM_H

#include <QWidget>

class DLTRelaisPlugin;

namespace Ui {
class DLTRelaisForm;
}

class DLTRelaisForm : public QWidget
{
    Q_OBJECT

public:
    explicit DLTRelaisForm(DLTRelaisPlugin *newDltRelais, QWidget *parent = nullptr);
    ~DLTRelaisForm();

    void update();

    void injection(QString data);

private slots:
    void on_pushButtonRelais1_clicked();

    void on_pushButtonRelais2_clicked();

    void on_pushButtonRelais3_clicked();

    void on_pushButtonRelais4_clicked();

    void on_checkBoxRelais1_clicked(bool checked);

    void on_checkBoxRelais2_clicked(bool checked);

    void on_checkBoxRelais3_clicked(bool checked);

    void on_checkBoxRelais4_clicked(bool checked);

private:

    void closeEvent(QCloseEvent *event);

    Ui::DLTRelaisForm *ui;
    DLTRelaisPlugin *dltRelais;
};

#endif // DLT_RELAIS_FORM_H
