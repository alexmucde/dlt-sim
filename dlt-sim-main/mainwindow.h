#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pluginmodel.h"
#include "dltminiserver.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSettings_triggered();

    void on_actionExit_triggered();

    void on_actionAdd_triggered();

    void on_actionDelete_triggered();

    void on_actionConfigure_triggered();

    void on_actionStart_triggered();

    void on_actionStop_triggered();

    void on_actionInfo_triggered();

    void on_tableViewPlugins_doubleClicked(const QModelIndex &index);

    void on_actionShow_triggered();

    void on_actionHide_triggered();

    void on_actionStartAll_triggered();

    void on_actionStopAll_triggered();

    void on_actionShowAll_triggered();

    void on_actionHideAll_triggered();

    void injection(QString data);

    void ShowContextMenu(const QPoint &);

private:

    void closeEvent(QCloseEvent *event);

    Ui::MainWindow *ui;

    PluginModel plugins;
    DLTMiniServer dltMiniServer;
};
#endif // MAINWINDOW_H
