#ifndef _TABLEMANGER_H_
#define _TABLEMANGER_H_

#include "common.h"
#include "customSqlTableModel.h"

#include <QMenu>
#include <QSqlError>
#include <QSqlTableModel>
#include <QTableView>
#include <QWidget>

class CustomSqlTableModel;
class DataTableManger;

class DataTableManger : public QWidget {
  Q_OBJECT
public:
  DataTableManger(const QString &tableName, QWidget *parent = nullptr);
  ~DataTableManger() = default;

  enum TableActionStatus { Idle, Editing, Adding };

protected:
  QTableView *m_pTableView{nullptr};
  QMenu *m_pMenu{nullptr};
  CustomSqlTableModel *m_pTableModel{nullptr};
  QItemSelectionModel *m_pSelectionModel{nullptr};

private:
  QString m_tableNmae{};
  TableActionStatus m_status{Idle};

public:
  virtual void build();
  virtual void refresh();

  QTableView *getTableView() const;
  QMenu *getContextMenu() const;
  QSqlTableModel *getTableModel() const;
  QItemSelectionModel *getSelectionModel() const;
  const TableActionStatus getCurrentTableActionStatus() const;
  QModelIndex getCurentSelectedIndex() const;

  /**
   * @brief 设置TableModel的sql WHERE条件 满足 fieldList.size == valueList.size
   * == conditionList.size+1
   *
   * @param fieldList 字段
   * @param valueList 字段对应的条件
   * @param conditionList 条件词 接受 'AND' 'OR'等
   */
  void setTableFilter(const QStringList &fieldList,
                      const QStringList &valueList,
                      const QStringList &conditionList);

  void clearTableFilter();

  /**
   * @brief 表中是否还存在未提交的数据
   *
   * @return true
   * @return false
   */
  bool isDirty();

private:
  void initUI();

public slots:
  /**
   * @brief 在当前所选行后插入空的一行
   *
   */
  void insertRow();
  void deleteRows();
  void editRow();
  virtual auto submitAll() -> QSqlError;
  void revertAll();

protected slots:

private slots:
  /**
   * @brief 呼出右键菜单事件槽函数
   *
   */
  void onCustomContextMenu(const QPoint &pos);
};

#endif // _TABLEMANGER_H_