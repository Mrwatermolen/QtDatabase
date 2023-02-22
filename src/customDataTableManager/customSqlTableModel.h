#ifndef _CUSTOMSQLTABLEMODEL_H_
#define _CUSTOMSQLTABLEMODEL_H_

#include <QSqlRelationalTableModel>

class CustomSqlTableModel;

class CustomSqlTableModel : public QSqlRelationalTableModel {
  Q_OBJECT
public:
  CustomSqlTableModel(QObject *parent = nullptr,
                      const QSqlDatabase &db = QSqlDatabase());
  ~CustomSqlTableModel() = default;

public:
  Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
  void setRowRecordEditEnable(const int &row);

private:
  int m_canEditRow{-1};
};

#endif // _CUSTOMSQLTABLEMODEL_H_