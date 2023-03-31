#ifndef PLUGINMODEL_H
#define PLUGINMODEL_H

#include "plugininterface.h"

#include <QAbstractTableModel>

class PluginModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PluginModel(DLTMiniServer *newDltMiniServer, QObject *parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section,Qt::Orientation,int role) const;

    QStringList getPluginNames();
    bool add(QString &pluginName);
    void deletePlugin(int num);

    void clear();
    void loadSettings(const QString &filename);
    void saveSettings(const QString &filename);
    void settings(int row,QWidget *parent);
    void start(int row = -1);
    void stop(int row = -1);
    void show(QMdiArea *mdiArea, int row = -1);
    void update(int row = -1);
    void hide(int row1 = -1);

    void injection(QString data);

private slots:
    void updateStatus();

private:
    QList<DLTSimPluginInterface *> plugins;
    DLTMiniServer *dltMiniServer;
};

#endif // PLUGINMODEL_H
