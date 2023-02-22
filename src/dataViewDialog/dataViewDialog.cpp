#include "dataViewDialog.h"
#include "common.h"

#include <QDateTimeEdit>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>

using namespace DatabaseTableConfig;

DataViewDialog::DataViewDialog(QWidget *parent) : QDialog(parent) {}

void DataViewDialog::initUI() {
  setLayout(new QVBoxLayout);
  m_pContainer = new QWidget(this);
  m_pContainer->setLayout(new QGridLayout);
  m_pBtnBox = new QDialogButtonBox(
      {QDialogButtonBox::Close, QDialogButtonBox::Ok}, this);
  this->layout()->addWidget(m_pContainer);
  this->layout()->addWidget(m_pBtnBox);

  m_pDataMapper = new QDataWidgetMapper(this);

  connect(m_pBtnBox, &QDialogButtonBox::accepted, this,
          &DataViewDialog::onBtnOkClicked);
  connect(m_pBtnBox, &QDialogButtonBox::rejected, this,
          &DataViewDialog::onBtnCloseClicked);
}

void DataViewDialog::build(QSqlTableModel *tableModel,
                           const TableConfig &tableConfig) {
  if (tableConfig.version <= m_tableConfig.version) {
    return;
  }

  m_pDataMapper->clearMapping();
  m_pDataMapper->setModel(tableModel);
  int maxColumn = 4, counter = 0;

  for (auto &&i : tableConfig.fieldConfig) {
    if (i.type == FieldType::Others || i.status == FieldStatus::Disable) {
      continue;
    }
    auto layout = dynamic_cast<QGridLayout *>(m_pContainer->layout());

    auto label = new QLabel(i.name);
    layout->addWidget(label, counter / maxColumn, counter % maxColumn);
    counter++;

    if (i.type == FieldType::DateTime) {
      auto dateTime = new QDateTimeEdit;
      layout->addWidget(dateTime, counter / maxColumn, counter % maxColumn);
      dateTime->clear();
      counter++;
      m_pDataMapper->addMapping(dateTime, tableModel->fieldIndex(i.field));
      continue;
    }

    if (i.type == FieldType::Text) {
      auto textEdit = new QPlainTextEdit;
      textEdit->clear();
      layout->addWidget(textEdit, counter / maxColumn, counter % maxColumn);
      counter++;
      m_pDataMapper->addMapping(textEdit, tableModel->fieldIndex(i.field));
      continue;
    }

    auto lineEdit = new QLineEdit;
    lineEdit->clear();
    layout->addWidget(lineEdit, counter / maxColumn, counter % maxColumn);
    counter++;
    m_pDataMapper->addMapping(lineEdit, tableModel->fieldIndex(i.field));
  }
  m_tableConfig = tableConfig;
}

void DataViewDialog::refresh(QSqlTableModel *tableModel,
                             const TableConfig &tableConfig) {
  if (tableConfig.version <= m_tableConfig.version) {
    return;
  }

  if (m_pContainer) {
    delete m_pContainer;
    m_pContainer = nullptr;
    m_pContainer = new QWidget(this);
    m_pContainer->setLayout(new QGridLayout);
  }
  this->layout()->addWidget(m_pContainer);
  this->layout()->removeWidget(m_pBtnBox);
  this->layout()->addWidget(m_pBtnBox);
  build(tableModel, tableConfig);
}

bool DataViewDialog::validData() { return true; }

void DataViewDialog::setDataMapperCurrentRow(const int row) {
  if (!m_pDataMapper) {
    return;
  }

  m_pDataMapper->setCurrentIndex(row);
}

void DataViewDialog::closeEvent(QCloseEvent *e) {
  qDebug() << "DataViewDialog::closeEvent()";
  reject();
}

void DataViewDialog::onBtnOkClicked() {
  qDebug() << "DataViewDialog::onBtnOkClicked()";
  if (!validData()) {
    reject();
  }
  accept();
}

void DataViewDialog::onBtnCloseClicked() {
  qDebug() << "DataViewDialog::onBtnCloseClicked()";
  reject();
}