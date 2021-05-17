#include "adapter.h"
#include "problemsmodel.h"
#include "problem.h"

ProblemsModel::ProblemsModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

void ProblemsModel::loadAllProblems()
{
    clearData();
    problems.append(Adapter::getAllProblems());
}

void ProblemsModel::loadCurrentProblem()
{
    clearData();
    problems.push_back(Adapter::getCurrentProblem());
}

void ProblemsModel::saveProblem(const QModelIndex &index)
{
    Adapter::saveProblem(problems[index.row()]);
}

QVariant ProblemsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QString("Problems");
}

int ProblemsModel::rowCount(const QModelIndex &parent) const
{
    return problems.length();
}

int ProblemsModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant ProblemsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        if (index.column() == 0) return (problems[index.row()])->name();
        if (index.column() == 1) return (problems[index.row()])->question();
        if (index.column() == 2) return (problems[index.row()])->answer();
    }

    return QVariant();
}

bool ProblemsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
         if (!checkIndex(index))
             return false;
         //save value from editor to member
         if (index.column() == 0) problems[index.row()]->setName(value.toString());
         if (index.column() == 1) problems[index.row()]->setQuestion(value.toString());
         if (index.column() == 2) problems[index.row()]->setAnswer(value.toString());

         return true;
    }
    return false;
}

Qt::ItemFlags ProblemsModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

bool ProblemsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || row > problems.size()) return false;
    if (count <= 0) return false;

    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; i++) {
        problems.insert(row + i, new Problem("Problem", "", "", this));
    }
    endInsertRows();
    return true;
}

bool ProblemsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || row > problems.size()) return false;
    if (count <= 0) return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; i++) {
        delete problems[row];
        problems.removeAt(row);
    }
    endRemoveRows();

    return true;
}

QString ProblemsModel::problem(int row)
{
    return problems[row]->question();
}

QString ProblemsModel::answer(int row)
{
    return problems[row]->answer();
}

void ProblemsModel::clearData()
{
    if (problems.size() != 0) {
        for (int i = 0; i < problems.size(); i++) delete problems[i];
        problems.clear();
    }
}
