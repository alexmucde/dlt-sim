#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "version.h"

#include <QInputDialog>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , plugins(&dltMiniServer,this)
    , dltMiniServer(this)
{
    ui->setupUi(this);

    ui->tableViewPlugins->setModel(&plugins);


    //  load global settings from registry
    QSettings settings;
    QString projectFileName = settings.value("autoload/projectFileName").toString();
    bool autoload = settings.value("autoload/checked").toBool();
    bool autostart = settings.value("autostart/checked").toBool();

    if(autoload && !projectFileName.isEmpty())
    {
        plugins.loadSettings(projectFileName);
    }

    if(autostart)
    {
        on_actionStartAll_triggered();
        on_actionShowAll_triggered();
    }

    ui->tableViewPlugins->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewPlugins->setSelectionMode(QAbstractItemView::SingleSelection);

    restoreGeometry(settings.value("geometry").toByteArray());

    quint16 dltPort = settings.value("dlt/port",3491).toUInt();
    dltMiniServer.setPort(dltPort);

    dltMiniServer.start();

    connect(&dltMiniServer,SIGNAL(injection(QString)),this,SLOT(injection(QString)));

    ui->tableViewPlugins->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableViewPlugins, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(ShowContextMenu(const QPoint &)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowContextMenu(const QPoint &pos)
{
   QMenu contextMenu(tr("Context menu"), this);

   QAction action1("Add", this);
   connect(&action1, SIGNAL(triggered()), this, SLOT(on_actionAdd_triggered()));
   contextMenu.addAction(&action1);

   QAction action2("Delete", this);
   connect(&action2, SIGNAL(triggered()), this, SLOT(on_actionDelete_triggered()));
   contextMenu.addAction(&action2);

   contextMenu.addSeparator();

   QAction action3("Start", this);
   connect(&action3, SIGNAL(triggered()), this, SLOT(on_actionStart_triggered()));
   contextMenu.addAction(&action3);

   QAction action4("Stop", this);
   connect(&action4, SIGNAL(triggered()), this, SLOT(on_actionStop_triggered()));
   contextMenu.addAction(&action4);

   contextMenu.addSeparator();

   QAction action5("Show", this);
   connect(&action5, SIGNAL(triggered()), this, SLOT(on_actionShow_triggered()));
   contextMenu.addAction(&action5);

   QAction action6("Hide", this);
   connect(&action6, SIGNAL(triggered()), this, SLOT(on_actionHide_triggered()));
   contextMenu.addAction(&action6);

   contextMenu.exec(mapToGlobal(pos));
}


void MainWindow::on_actionNew_triggered()
{
    plugins.clear();
}


void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Project"), "", tr("DLTSim Project (*.xml);;All files (*.*)"));

    if(fileName.isEmpty())
    {
        // No file was selected or cancel was pressed
        return;
    }

    plugins.loadSettings(fileName);
}


void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Project"), "", tr("DLTSim Project (*.xml);;All files (*.*)"));

    if(fileName.isEmpty())
    {
        // No file was selected or cancel was pressed
        return;
    }

    plugins.saveSettings(fileName);
}


void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog dlg(this);
    dlg.restoreSettings(plugins);
    dltMiniServer.stop();

    if(dlg.exec()==QDialog::Accepted)
    {
        dlg.backupSettings(plugins);
        QSettings settings;
        quint16 dltPort = settings.value("dlt/port",3491).toUInt();
        dltMiniServer.setPort(dltPort);
    }
    dltMiniServer.start();
}


void MainWindow::on_actionExit_triggered()
{
    close();
}


void MainWindow::on_actionAdd_triggered()
{
    QStringList pluginNames = plugins.getPluginNames();


    QString text = QInputDialog::getItem(this, tr("Add Plugin"),
                                         tr("Plugin:"), pluginNames,0,false);
    if (!text.isEmpty())
    {
        if(!plugins.add(text))
        {
            // Add failed
            QMessageBox(QMessageBox::Critical,"Add Plugin",QString("Cannot add plugin %1, already added").arg(text)).exec();
        }
    }
}


void MainWindow::on_actionDelete_triggered()
{
    QItemSelectionModel *select = ui->tableViewPlugins->selectionModel();

    if(select->hasSelection())
    {
        plugins.deletePlugin(select->selectedRows().at(0).row());
    }
}

void MainWindow::on_actionStart_triggered()
{
    QItemSelectionModel *select = ui->tableViewPlugins->selectionModel();

    if(select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();
        plugins.start(row);
    }
}

void MainWindow::on_actionStop_triggered()
{
    QItemSelectionModel *select = ui->tableViewPlugins->selectionModel();

    if(select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();
        plugins.stop(row);
    }
}

void MainWindow::on_actionInfo_triggered()
{
    QMessageBox msgBox(this);

    msgBox.setWindowTitle("Info DLT Simulatorr");
    msgBox.setTextFormat(Qt::RichText);

    QString text;
    text += QString("Version:<br>");
    text += QString("%1<br>").arg(DLT_SIM_VERSION);
    text += "<br>";
    text += "Information and Documentation can be found here:<br>";
    text += "<br>";
    text += "Contact: Alexander Wenzel <a href='mailto:Alexander.AW.Wenzel@bmw.de'>Alexander.AW.Wenzel@bmw.de</a><br><br>";
    text += "<br>";
    text += "(C) 2023 BMW AG";

    msgBox.setText(text);

    msgBox.setStandardButtons(QMessageBox::Ok);

    msgBox.exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
}

void MainWindow::on_actionConfigure_triggered()
{
    QItemSelectionModel *select = ui->tableViewPlugins->selectionModel();

    if(select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();
        plugins.settings(row,this);
        plugins.update(row);
    }
}

void MainWindow::on_tableViewPlugins_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    plugins.settings(row,this);
    plugins.update(row);
}


void MainWindow::on_actionShow_triggered()
{
    QItemSelectionModel *select = ui->tableViewPlugins->selectionModel();

    if(select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();
        plugins.show(ui->mdiArea,row);
    }
}


void MainWindow::on_actionHide_triggered()
{
    QItemSelectionModel *select = ui->tableViewPlugins->selectionModel();

    if(select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();
        plugins.hide(row);
    }
}


void MainWindow::on_actionStartAll_triggered()
{
    plugins.start();
}


void MainWindow::on_actionStopAll_triggered()
{
    plugins.stop();
}


void MainWindow::on_actionShowAll_triggered()
{
    plugins.show(ui->mdiArea);
}


void MainWindow::on_actionHideAll_triggered()
{
    plugins.hide();
}

void MainWindow::injection(QString data)
{
    plugins.injection(data);
}
