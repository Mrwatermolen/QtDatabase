#include "dataTableManger.h"

#include <QLayout>
#include <QMessageBox>
#include <QPushButton>

// SELECT   COLUMN_NAME 列名,   COLUMN_COMMENT 名称 ,   COLUMN_TYPE 数据类型,
// DATA_TYPE 字段类型,     CHARACTER_MAXIMUM_LENGTH 长度,   IS_NULLABLE
// 是否必填,   COLUMN_DEFAULT 描述  FROM  INFORMATION_SCHEMA.COLUMNS WHERE
// table_name  = "RawImage";

/* SELECT
  COLUMN_NAME 列名,
  COLUMN_COMMENT 名称 ,
  COLUMN_TYPE 数据类型,
  DATA_TYPE 字段类型,
  CHARACTER_MAXIMUM_LENGTH 长度,
  IS_NULLABLE 是否必填,
  COLUMN_DEFAULT 描述
FROM
 INFORMATION_SCHEMA.COLUMNS
where
-- developerclub为数据库名称，到时候只需要修改成你要导出表结构的数据库即可
table_schema ='litchi'
AND
-- article为表名，到时候换成你要导出的表的名称
--
如果不写的话，默认会查询出所有表中的数据，这样可能就分不清到底哪些字段是哪张表中的了，所以还是建议写上要导出的名名称
table_name  = 'tb_item'
 */

using namespace DatabaseTableConfig;

DataTableManger::DataTableManger(const QString &tableName, QWidget *parent)
    : QWidget(parent), m_tableNmae(tableName) {
  initUI();
}

void DataTableManger::build() {
  auto db{QSqlDatabase::database()};
  if (!db.open()) {
    qDebug() << "DataTableManger::build() error: Database don't open.";
    return;
  }
  m_pTableModel = new CustomSqlTableModel(this, db);
  m_pTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
  m_pTableModel->setTable(m_tableNmae);
  m_pTableView->setModel(m_pTableModel);

  m_pSelectionModel = new QItemSelectionModel(m_pTableModel);
  m_pTableView->setSelectionModel(m_pSelectionModel);

  m_pTableView->setContextMenuPolicy(Qt::CustomContextMenu);
  auto addAct{*m_pMenu->actions().begin()};
  auto editAct{m_pMenu->actions()[1]};
  auto deleteAct{m_pMenu->actions()[2]};
  auto submitAct{m_pMenu->actions()[3]};
  auto revertAct{m_pMenu->actions()[4]};

  connect(m_pTableView, &QTableView::customContextMenuRequested, this,
          &DataTableManger::onCustomContextMenu); // 呼出右键菜单事件
  connect(addAct, &QAction::triggered, this, &DataTableManger::insertRow);
  connect(editAct, &QAction::triggered, this, &DataTableManger::editRow);
  connect(deleteAct, &QAction::triggered, this, &DataTableManger::deleteRows);
  connect(submitAct, &QAction::triggered, this, &DataTableManger::submitAll);
  connect(revertAct, &QAction::triggered, this, &DataTableManger::revertAll);
  refresh();
}

void DataTableManger::refresh() {

  qDebug() << "DataTableManger::refresh() current filter:"
           << m_pTableModel->filter();

  m_pTableModel->select();
}

QTableView *DataTableManger::getTableView() const { return m_pTableView; }

QMenu *DataTableManger::getContextMenu() const { return m_pMenu; }

QSqlTableModel *DataTableManger::getTableModel() const { return m_pTableModel; }

QItemSelectionModel *DataTableManger::getSelectionModel() const {
  return m_pSelectionModel;
}

const DataTableManger::TableActionStatus
DataTableManger::getCurrentTableActionStatus() const {
  return m_status;
}

QModelIndex DataTableManger::getCurentSelectedIndex() const {
  return m_pSelectionModel->currentIndex();
}

void DataTableManger::setTableFilter(const QStringList &fieldList,
                                     const QStringList &valueList,
                                     const QStringList &conditionList) {
  if (fieldList.size() != valueList.size() ||
      fieldList.size() != conditionList.size() + 1) {
    clearTableFilter();
    return;
  }
  QString clause;
  for (int i = 0; i < fieldList.size(); i++) {
    clause += fieldList[i] + "=" + valueList[i] + " ";
    if (i == fieldList.size() - 1) {
      // 最后一个条件
      break;
    }

    clause += conditionList[i] + " ";
  }
  if (m_pTableModel->filter() == clause) {
    return;
  }
  qDebug() << "DataTableManger::setTableFilter() clause:" << clause;
  m_pTableModel->setFilter(clause);
}

void DataTableManger::clearTableFilter() { m_pTableModel->setFilter(""); }

bool DataTableManger::isDirty() { return m_pTableModel->isDirty(); }

void DataTableManger::initUI() {
  setLayout(new QVBoxLayout);
  m_pTableView = new QTableView(this);
  m_pTableView->setSelectionBehavior(
      QAbstractItemView::SelectRows);    // 行选择模式
  m_pTableView->setSortingEnabled(true); // 允许排序

  layout()->addWidget(m_pTableView);

  m_pMenu = new QMenu(m_pTableView);
  m_pMenu->addAction(new QAction(tr("插入行")));
  m_pMenu->addAction(new QAction(tr("编辑行")));
  m_pMenu->addAction(new QAction(tr("删除所选行")));
  m_pMenu->addAction(new QAction(tr("提交所有更改")));
  m_pMenu->addAction(new QAction(tr("撤销所有更改")));
  m_pMenu->addSeparator();
}

void DataTableManger::onCustomContextMenu(const QPoint &pos) {
  if (!m_pMenu || !m_pSelectionModel->currentIndex().isValid()) {
    return;
  }

  m_pMenu->exec(QCursor::pos());
}

void DataTableManger::insertRow() {
  qDebug() << "DataTableManger::insertRecord() Table Action Status:"
           << m_status;
  auto row{-1};
  if (auto index{m_pSelectionModel->currentIndex()}; !index.isValid()) {
    row = m_pTableModel->rowCount();
  } else {
    row = index.row() + 1;
  }
  m_pTableModel->insertRow(row, QModelIndex());
  auto currentIndex{m_pTableModel->index(row, 0)};
  m_pSelectionModel->clearSelection();
  // m_pTableModel->setRowRecordEditEnable(row);
  m_pTableView->setCurrentIndex(currentIndex);
  m_status = TableActionStatus::Adding;
}

void DataTableManger::editRow() {
  auto index{m_pSelectionModel->currentIndex()};

  qDebug() << "DataTableManger::editRecord()" << index
           << "Table Action Status:" << m_status;
  if (!index.isValid()) {
    return;
  }

  m_pSelectionModel->clearSelection();
  m_pTableView->setCurrentIndex(index);
  // m_pTableModel->setRowRecordEditEnable(index.row());
  m_status = TableActionStatus::Editing;
}

void DataTableManger::deleteRows() {
  auto deleteIndexList{m_pSelectionModel->selectedRows()};
  if (deleteIndexList.isEmpty()) {
    return;
  }
  auto msg{QMessageBox(this)};
  msg.setIcon(QMessageBox::Warning);
  msg.setWindowTitle("删除数据");
  msg.setText("是否确认删除所选的" + QString::number(deleteIndexList.size()) +
              "条数据。注意该操作无法撤回！！");
  QAbstractButton *cancel =
      msg.addButton(tr("Cancel"), QMessageBox::RejectRole);
  QAbstractButton *ok = msg.addButton(tr("OK"), QMessageBox::AcceptRole);

  msg.exec();
  if (msg.clickedButton() != ok) {
    return;
  }
  int count{0};
  for (auto &&i : deleteIndexList) {
    if (!i.isValid()) {
      continue;
    }

    m_pTableModel->removeRow(i.row());
    ++count;
  }
  QMessageBox::information(
      this, "操作成功",
      "成功删除" + QString::number(deleteIndexList.size()) + "条数据");
  submitAll();
  refresh();
}

auto DataTableManger::submitAll() -> QSqlError {
  m_pTableModel->submitAll();
  return m_pTableModel->lastError();
}

void DataTableManger::revertAll() { m_pTableModel->revertAll(); }