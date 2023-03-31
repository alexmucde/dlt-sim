#include "pluginmodel.h"
#include "dltminiserver.h"

#include <QDir>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QObject>

PluginModel::PluginModel(DLTMiniServer *newDltMiniServer, QObject *parent)
    : QAbstractTableModel{parent}
{
    dltMiniServer= newDltMiniServer;

}

int PluginModel::rowCount(const QModelIndex& parent) const
{
    return plugins.size();
}

int PluginModel::columnCount(const QModelIndex& parent) const
{
    return 2;
}

QVariant PluginModel::headerData(int section,Qt::Orientation,int role) const
{
    if ( role == Qt::DisplayRole)
    {
        if(section==0)
        {
            return QString("Plugin");
        }
        else if(section==1)
        {
            return "Status";
        }
        else
        {
            return QVariant();
        }
    }
    else
    {
        return QVariant();
    }
}

QVariant PluginModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if ( role == Qt::DisplayRole)
    {
        if ( index.column() == 0)
            return plugins[index.row()]->name();
        else if ( index.column() == 1)
            return plugins[index.row()]->status();
        else
            return QVariant();
    }
    else if (role == Qt::BackgroundRole)
    {
        if ( index.column() == 1)
        {
            if(plugins[index.row()]->status()=="running")
            {
                QColor color(0,255,0);
                return color;
            }
            else if(plugins[index.row()]->status()=="error")
            {
                QColor color(255,0,0);
                return color;
            }
        }
        QColor color(255,255,255);
        return color;
    }
    return QVariant();
}

QStringList PluginModel::getPluginNames()
{
    DLTSimPluginInterface *pluginInterface;
    QStringList pluginNames;

    QDir pluginsDir(QCoreApplication::applicationDirPath());
    pluginsDir.cd("plugins");
    const QStringList entries = pluginsDir.entryList(QDir::Files);
    for (const QString &fileName : entries) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            pluginInterface = qobject_cast<DLTSimPluginInterface *>(plugin);
            pluginNames.append(pluginInterface->name());
            pluginLoader.unload();
        }
    }
    QDir customPluginsDir(QCoreApplication::applicationDirPath());
    customPluginsDir.cd("custom_plugins");
    const QStringList customEntries = customPluginsDir.entryList(QDir::Files);
    for (const QString &fileName : customEntries) {
        QPluginLoader pluginLoader(customPluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            pluginInterface = qobject_cast<DLTSimPluginInterface *>(plugin);
            pluginNames.append(pluginInterface->name());
            pluginLoader.unload();
        }
    }

    return pluginNames;
}


bool PluginModel::add(QString &pluginName)
{
    DLTSimPluginInterface *pluginInterface;

    // check if already added
    for(int num=0;num<plugins.size();num++)
    {
        if(plugins[num]->name()==pluginName)
        {
            // already added
            return false;
        }
    }


    beginInsertRows(index(plugins.size(),0),plugins.size(),plugins.size());

    QDir pluginsDir(QCoreApplication::applicationDirPath());
    pluginsDir.cd("plugins");
    const QStringList entries = pluginsDir.entryList(QDir::Files);
    for (const QString &fileName : entries)
    {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin)
        {
            pluginInterface = qobject_cast<DLTSimPluginInterface *>(plugin);
            if(pluginInterface->name()==pluginName)
            {
                plugins.append(pluginInterface);
            }
            else
            {
                pluginLoader.unload();
            }
        }
    }
    QDir customPluginsDir(QCoreApplication::applicationDirPath());
    customPluginsDir.cd("custom_plugins");
    const QStringList customEntries = customPluginsDir.entryList(QDir::Files);
    for (const QString &fileName : customEntries)
    {
        QPluginLoader pluginLoader(customPluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin)
        {
            pluginInterface = qobject_cast<DLTSimPluginInterface *>(plugin);
            if(pluginInterface->name()==pluginName)
            {
                plugins.append(pluginInterface);
            }
            else
            {
                pluginLoader.unload();
            }
        }
    }

    endInsertRows();

    return true;
}

void PluginModel::deletePlugin(int num)
{
    beginResetModel();

    plugins[num]->stop();
    plugins[num]->hide();
    plugins.removeAt(num);

    endResetModel();

}


void PluginModel::clear()
{
    beginResetModel();

    stop();
    hide();
    plugins.clear();

    endResetModel();
}

void PluginModel::loadSettings(const QString &fileName)
{
    int instance = 1;

    clear();

    // read settings from XML file
    bool isPlugin = false;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
             return;

    QXmlStreamReader xml(&file);

    while (!xml.atEnd())
    {
          xml.readNext();

          if(xml.isStartElement())
          {
              if(isPlugin && xml.name() == QString("Name"))
              {
                  QString name =  xml.readElementText();
                  add(name);
                  isPlugin = false;
                  if(plugins.size())
                  {
                    plugins[plugins.size()-1]->readSettings(fileName,instance++);
                  }
              }
              else if(xml.name() == QString("Plugin"))
              {
                    isPlugin = true;
              }
          }
          else if(xml.isEndElement())
          {
              /* Connection, plugin and filter */
              if(xml.name() == QString("Plugin"))
              {
                  isPlugin = false;
              }
          }
    }
    if (xml.hasError())
    {
         qDebug() << "Error in processing filter file" << fileName << xml.errorString();
    }

    file.close();

}

void PluginModel::saveSettings(const QString &fileName)
{
    int instance = 1;

    // read the settings from XML file
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        // Cannot open the file for writing
        return;
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);

    // FIXME: Cannot read data from XML file, which contains a start document
    // So currently do not call StartDocument
    //xml.writeStartDocument();

    xml.writeStartElement("DLTSim");

    for(int num=0;num<plugins.size();num++)
    {
        xml.writeStartElement("Plugin");
        plugins[num]->writeSettings(xml,instance++);
        xml.writeEndElement(); // DLTSim
    }

    xml.writeEndElement(); // DLTSim

    // FIXME: Cannot read data from XML file, which contains a end document
    // So currently do not call EndDocument
    //xml.writeEndDocument();
    file.close();
}

void PluginModel::settings(int row,QWidget *parent)
{    
    plugins[row]->settings(parent);
}

void PluginModel::start(int row)
{
    if(row>=0)
    {
        plugins[row]->start(dltMiniServer);
        connect(plugins[row]->getObject(),SIGNAL(updateStatus()),this,SLOT(updateStatus()));
    }
    else
    {
        for(int num=0;num<plugins.size();num++)
        {
            plugins[num]->start(dltMiniServer);
            connect(plugins[num]->getObject(),SIGNAL(updateStatus()),this,SLOT(updateStatus()));
        }
    }
}

void PluginModel::stop(int row)
{
    if(row>=0)
    {
        plugins[row]->stop();
        disconnect(plugins[row]->getObject(),SIGNAL(updateStatus()),this,SLOT(updateStatus()));
    }
    else
    {
        for(int num=0;num<plugins.size();num++)
        {
            plugins[num]->stop();
            disconnect(plugins[num]->getObject(),SIGNAL(updateStatus()),this,SLOT(updateStatus()));
        }
    }
}

void PluginModel::show(QMdiArea *mdiArea,int row)
{
    if(row>=0)
    {
        plugins[row]->show(mdiArea);
    }
    else
    {
        for(int num=0;num<plugins.size();num++)
        {
            plugins[num]->show(mdiArea);
        }
    }
}

void PluginModel::update(int row)
{

    plugins[row]->update();
}

void PluginModel::hide(int row)
{
    if(row>=0)
    {
        plugins[row]->hide();
    }
    else
    {
        for(int num=0;num<plugins.size();num++)
        {
            plugins[num]->hide();
        }
    }
}

void PluginModel::updateStatus()
{
    emit dataChanged(index(0,1),index(plugins.size()-1,1));
}

void PluginModel::injection(QString data)
{
    qDebug() << "Injection received: " << data;

    for(int num=0;num<plugins.size();num++)
    {
        plugins[num]->injection(data);
    }
}
