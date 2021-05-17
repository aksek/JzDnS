#ifndef SERVERSMODEL_H
#define SERVERSMODEL_H

#include <QAbstractListModel>
#include <QList>

class ServersModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ServersModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QList<QString> servers;
};

#endif // SERVERSMODEL_H
