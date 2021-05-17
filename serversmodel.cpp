
#include "serversmodel.h"
#include "adapter.h"
#include "mock.h"

ServersModel::ServersModel(QObject *parent)
    : QAbstractListModel(parent)
{
    std::vector<std::string> std_servers = Mock::getServers();
    servers = Adapter::getServers();
}

QVariant ServersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        return QString("Server");
    default:
        return QVariant();
    }
}

int ServersModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;
    else
        return servers.length();
}

QVariant ServersModel::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        return servers[index.row()];
    default:
        return QVariant();
    }

    return QVariant();
}
