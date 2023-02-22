#include "dataTableMangerpage.h"
#include "advancedquerywidget.h"
#include "common.h"
#include "dataTableManagerWithTableConfig.h"
#include "dataTableManger.h"
#include "dataViewDialog.h"

#include <QLayout>
#include <QMessageBox>

using namespace DatabaseTableConfig;
DataTableMangerPage::DataTableMangerPage(const TableConfig &tableconfig,
                                         QWidget *parent)
    : QWidget{parent}, m_tableConfig{tableconfig} {
  initUI();
};

void DataTableMangerPage::build() {
  m_pTableManager->build();
  m_pDataMapper = new QDataWidgetMapper(this);
  m_pDataMapper->setModel(m_pTableManager->getTableModel());
  m_pDataViewDialog = new DataViewDialog(this);
  m_pDataViewDialog->initUI();
  m_pDataViewDialog->build(m_pTableManager->getTableModel(), m_tableConfig);

  m_pAdvancedQueryView->build(m_tableConfig);
  m_pAdvancedQueryView->refresh(m_pTableManager->getTableModel(),
                                m_tableConfig);

  connect(m_pTableManager,
          &DataTableMangerWithTableConfig::currentSelectedRowChnaged, this,
          &DataTableMangerPage::onTableCurrentRowChanged);

  connect(m_pBtnAdd, &QPushButton::clicked, this,
          &DataTableMangerPage::onBtnAddClicked);
  connect(m_pBtnDelete, &QPushButton::clicked, this,
          &DataTableMangerPage::onBtnDeleteClicked);
  connect(m_pBtnEdit, &QPushButton::clicked, this,
          &DataTableMangerPage::onBtnEditClicked);
  connect(m_pBtnQuery, &QPushButton::clicked, this,
          &DataTableMangerPage::onBtnAdvancedQueryClicked);

  connect(m_pDataViewDialog, &DataViewDialog::accepted, this,
          &DataTableMangerPage::onDialogAccepted);
  connect(m_pDataViewDialog, &DataViewDialog::rejected, this,
          &DataTableMangerPage::onDialogRejected);

  connect(m_pAdvancedQueryView, &AdvancedQueryWidget::filterChange, this,
          &DataTableMangerPage::onAdvancedQueryFilterChanged);
}

void DataTableMangerPage::refresh(const TableConfig &tableConfig) {
  if (tableConfig.version < m_tableConfig.version) {
    return;
  }

  m_tableConfig = tableConfig;

  m_pTableManager->setTableConfig(tableConfig);
  m_pTableManager->refresh();

  m_pDataViewDialog->refresh(m_pTableManager->getTableModel(), tableConfig);
  m_pAdvancedQueryView->refresh(m_pTableManager->getTableModel(), tableConfig);
}

void DataTableMangerPage::initUI() {
  auto mainLayout{new QVBoxLayout};
  setLayout(mainLayout);

  m_pTableManager = new DataTableMangerWithTableConfig(m_tableConfig, this);
  mainLayout->addWidget(m_pTableManager);

  auto operatorLayout{new QHBoxLayout};
  m_pBtnAdd = new QPushButton(tr("添加行")),
  m_pBtnDelete = new QPushButton(tr("删除所选行")),
  m_pBtnEdit = new QPushButton(tr("编辑行")),
  m_pBtnQuery = new QPushButton(tr("开启/关闭查询"));
  operatorLayout->addWidget(m_pBtnAdd);
  operatorLayout->addWidget(m_pBtnDelete);
  operatorLayout->addWidget(m_pBtnEdit);
  operatorLayout->addWidget(m_pBtnQuery);
  mainLayout->addLayout(operatorLayout);

  m_pAdvancedQueryView = new AdvancedQueryWidget(this);
  m_pAdvancedQueryView->initUI();
  m_pAdvancedQueryView->setVisible(false);
  mainLayout->addWidget(m_pAdvancedQueryView);
}

void DataTableMangerPage::setDataMapperCurrentRow(const int row) {
  if (!m_pDataMapper) {
    return;
  }

  m_pDataMapper->setCurrentIndex(row);
}

void DataTableMangerPage::setDataViewDialogDataMapperCurrentRow(const int row) {
  if (!m_pDataViewDialog) {
    return;
  }

  m_pDataViewDialog->setDataMapperCurrentRow(row);
}

void DataTableMangerPage::onTableCurrentRowChanged(
    const QModelIndex &current, const QModelIndex &previous) {
  if (current.row() == previous.row()) {
    return;
  }

  setDataMapperCurrentRow(current.row());
  setDataViewDialogDataMapperCurrentRow(current.row());
}

void DataTableMangerPage::onBtnAddClicked() {
  m_pTableManager->insertRow();
  m_pDataViewDialog->setWindowTitle("添加" + m_tableConfig.tableName + "数据");
  m_pDataViewDialog->exec();
}

void DataTableMangerPage::onBtnDeleteClicked() {
  m_pTableManager->deleteRows();
}

void DataTableMangerPage::onBtnEditClicked() {
  qDebug() << "DataTableMangerPage::onBtnEditClicked()";
  m_pTableManager->editRow();
  m_pDataViewDialog->setWindowTitle("编辑" + m_tableConfig.tableName + "数据");
  m_pDataViewDialog->exec();
}

void DataTableMangerPage::onBtnAdvancedQueryClicked() {
  qDebug() << "DataTableMangerPage::onBtnAdvancedQueryClicked()";
  if (!m_pAdvancedQueryView) {
    return;
  }
  if (!m_pAdvancedQueryView->isVisible()) {
    m_pAdvancedQueryView->setVisible(true);
  } else {
    m_pAdvancedQueryView->setVisible(false);
  }
}

void DataTableMangerPage::onDialogAccepted() {
  qDebug() << "DataTableMangerPage::onDialogAccepted()";
  auto tableModel{m_pTableManager->getTableModel()};
  auto sqlErr{m_pTableManager->submitAll()};
  if (sqlErr.type() == QSqlError::NoError) {
    QMessageBox::information(this, "成功", "操作成功");
    return;
  }

  QMessageBox::critical(this, "失败",
                        "错误信息:" + sqlErr.text() +
                            "\n 错误类型:" + QString::number(sqlErr.type()));
}

void DataTableMangerPage::onDialogRejected() {
  qDebug() << "DataTableMangerPage::onDialogRejected()";
  auto curIndex{m_pTableManager->getSelectionModel()->currentIndex()};
  if (!curIndex.isValid() || m_pTableManager->getCurrentTableActionStatus() !=
                                 DataTableManger::Adding) {
    return;
  }
  auto tableModel{m_pTableManager->getTableModel()};

  tableModel->removeRow(curIndex.row());
}

void DataTableMangerPage::onAdvancedQueryFilterChanged(
    const TableFilterConfig &tableFilterConfig) {
  QStringList fieldList, valueList, conditionList;
  for (int i = 0; i < tableFilterConfig.fieldConfig.size(); i++) {
    if (tableFilterConfig.data[i].isNull()) {
      continue;
    }
    fieldList << tableFilterConfig.fieldConfig[i].field;
    valueList << tableFilterConfig.data[i].toString();
  }
  for (int i = 1; i < fieldList.size(); i++) {
    conditionList << "AND";
  }
  m_pTableManager->setTableFilter(fieldList, valueList, conditionList);
  m_pTableManager->refresh();
  m_pAdvancedQueryView->refresh(m_pTableManager->getTableModel(),
                                m_tableConfig);
}