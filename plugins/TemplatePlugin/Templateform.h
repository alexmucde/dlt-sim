#ifndef TEMPLATE_FORM_H
#define TEMPLATE_FORM_H

#include <QWidget>

class TemplatePlugin;

namespace Ui {
class TemplateForm;
}

class TemplateForm : public QWidget
{
    Q_OBJECT

public:
    explicit TemplateForm(TemplatePlugin *plugin, QWidget *parent = nullptr);
    ~TemplateForm();

private:
    Ui::TemplateForm *ui;

    TemplatePlugin *plugin;
};

#endif // TEMPLATE_FORM_H
