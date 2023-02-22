#include "dataTableManagerWithTableConfig.h"
#include "common.h"

using namespace DatabaseTableConfig;

DataTableMangerWithTableConfig::DataTableMangerWithTableConfig(
    const DatabaseTableConfig::TableConfig &tableConfig, QWidget *parent)
    : DataTableManger(tableConfig.tableName, parent),
      m_tableConfig(tableConfig), m_pTableItem(new QMenu(this)) {
  m_pActionHiddenTableItem = getContextMenu()->addMenu(m_pTableItem);
  m_pActionHiddenTableItem->setText("显示隐藏项");
  getContextMenu()->addSeparator();
}

void DataTableMangerWithTableConfig::build() {
  DataTableManger::build();
  setTableMangerWithTableConfig();
  connect(m_pSelectionModel, &QItemSelectionModel::currentRowChanged, this,
          [this](const QModelIndex &current, const QModelIndex &previous) {
            emit this->currentSelectedRowChnaged(current, previous);
          });
  connect(m_pSelectionModel, &QItemSelectionModel::currentRowChanged, this,
          &DataTableMangerWithTableConfig::onCurrentSelectedRowChnaged);
}

void DataTableMangerWithTableConfig::refresh() {
  getTableModel()->select();

  if (m_lastTableConfigVersion < m_tableConfig.version) {
    setTableMangerWithTableConfig();
    m_lastTableConfigVersion = m_tableConfig.version;
  }
}

const DatabaseTableConfig::TableConfig &
DataTableMangerWithTableConfig::getTableConfig() {
  return m_tableConfig;
}

void DataTableMangerWithTableConfig::setTableConfig(
    const DatabaseTableConfig::TableConfig &tableConfig) {
  if (tableConfig.version < m_tableConfig.version ||
      tableConfig.tableName != m_tableConfig.tableName) {
    return;
  }

  m_lastTableConfigVersion = m_tableConfig.version;
  m_tableConfig = tableConfig;
}

void DataTableMangerWithTableConfig::setTableMangerWithTableConfig() {
  for (auto &&i : m_pTableItem->actions()) {
    delete i;
  }
  m_pTableItem->clear();

  // 根据字段的不同状态放置其显示位置
  for (auto &&i : m_tableConfig.fieldConfig) {
    auto columnIndex{m_pTableModel->fieldIndex(i.field)};
    if (i.type == FieldType::Others || i.status == FieldStatus::Disable) {
      m_pTableView->setColumnHidden(columnIndex, true);
      continue;
    }

    if (i.type == FieldType::ForeignKey) {
      auto relationTable{dynamic_cast<QSqlRelationalTableModel *>(getTableModel())};
      // relationTable->setRelation(columnIndex, QSqlRelation("tableName", "indexCol", "displayCol"));
    }

    if (i.status == FieldStatus::InMenu) {
      auto act{new QAction()};
      m_pTableItem->addAction(act);
      m_pTableItem->addSeparator();
      m_pTableView->setColumnHidden(columnIndex, true);
      continue;
    }

    if (i.type == FieldType::DateTime && i.status == FieldStatus::InTableView) {
      m_pTableView->setColumnHidden(columnIndex, false);
      m_pTableModel->setHeaderData(columnIndex, Qt::Horizontal, i.name);
      continue;
    }

    m_pTableView->setColumnHidden(columnIndex, false);
    m_pTableModel->setHeaderData(columnIndex, Qt::Horizontal, i.name);
  }
}

void DataTableMangerWithTableConfig::refreshTableItemMenu(
    const QModelIndex &index) {
  int counter = 0;
  for (auto &&i : m_tableConfig.fieldConfig) {
    if (i.status != FieldStatus::InMenu) {
      continue;
    }
    m_pTableItem->actions()[counter]->setText(
        i.name + ":" +
        m_pTableModel->index(index.row(), m_pTableModel->fieldIndex(i.field))
            .data()
            .toString());
    counter += 2; // 还有speactor 所以加二
  }
}

void DataTableMangerWithTableConfig::onCurrentSelectedRowChnaged(
    const QModelIndex &current, const QModelIndex &previous) {
  if (current.row() == previous.row()) {
    return;
  }

  refreshTableItemMenu(current);
}