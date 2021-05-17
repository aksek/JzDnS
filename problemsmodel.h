#ifndef PROBLEMSMODEL_H
#define PROBLEMSMODEL_H

#include <QAbstractTableModel>

class Problem;

class ProblemsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ProblemsModel(QObject *parent = nullptr);

    void loadAllProblems();
    void loadCurrentProblem();
    void saveProblem(const QModelIndex &index);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    QString problem(int row);
    QString answer(int row);

private:
    QList<Problem *> problems;

    void clearData();
};

#endif // PROBLEMSMODEL_H
