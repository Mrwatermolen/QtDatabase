#include "customSqlTableModel.h"

CustomSqlTableModel::CustomSqlTableModel(QObject *parent,
                                         const QSqlDatabase &db)
    : QSqlRelationalTableModel(parent, db) {}

Qt::ItemFlags CustomSqlTableModel::flags(const QModelIndex &index) const {
  if (index.row() == m_canEditRow) {
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  }
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void CustomSqlTableModel::setRowRecordEditEnable(const int &row) {
  m_canEditRow = row;
}