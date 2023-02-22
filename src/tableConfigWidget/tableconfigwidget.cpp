#include "tableconfigwidget.h"
#include "common.h"

using namespace DatabaseTableConfig;

const QStringList FieldStatusTextList = QStringList() << "表"
                                                      << "右键菜单"
                                                      << "不研究";
const int MaxColumnNum = 3; // 栅格布局最大列数

Item::Item(QWidget *parent, const FieldConfig &fielConfig)
    : QWidget(parent), label(new QLabel(fielConfig.name, this)),
      field(fielConfig.field), fileType(fielConfig.type) {
  auto form = new QFormLayout(this);
  combox = new QComboBox(this);
  combox->addItems(FieldStatusTextList);
  combox->setCurrentIndex(FieldStatusToIndex(fielConfig.status));
  form->addRow(label, combox);
}

Item::~Item() = default;

int Item::FieldStatusToIndex(
    const DatabaseTableConfig::FieldStatus &fieldStatus) {
  switch (fieldStatus) {
  case FieldStatus::InTableView: {
    return 0;
  }
  case FieldStatus::InMenu: {
    return 1;
  }
  case FieldStatus::Disable: {
    return 2;
  }
  default:
    return -1;
  }
}

FieldStatus Item::IndexToFieldStatus(const int &index) {
  switch (index) {
  case 0: {
    return FieldStatus::InTableView;
  }
  case 1: {
    return FieldStatus::InMenu;
  }
  case 2: {
    return FieldStatus::Disable;
  }
  default:
    return FieldStatus::Disable;
  }
}

TableConfigWidget::TableConfigWidget(QWidget *parent) : QWidget(parent) {}

TableConfigWidget::~TableConfigWidget() {
  qDebug() << "TableConfigWidget::~TableConfigWidget()";
  for (auto &i : m_vecPItem) {
    delete i;
    i = nullptr;
  }
  m_vecPItem.clear();
  if (m_pTableLabel) {
    delete m_pTableLabel;
    m_pTableLabel = nullptr;
  }
  if (m_pGridLayout) {
    delete m_pGridLayout;
    m_pGridLayout = nullptr;
  }
  if (m_pHBoxLayout) {
    delete m_pHBoxLayout;
    m_pHBoxLayout = nullptr;
  }
}

TableConfig &TableConfigWidget::getTableConfig() {
  regenerateTableConfig();
  return m_tableConfig;
}

void TableConfigWidget::addFieldConfigItem(const DatabaseTableConfig::FieldConfig &fieldConfig) {
  m_vecPItem.append(new Item(this, fieldConfig));
  int n = m_vecPItem.size() - 1;
  m_pGridLayout->addWidget(m_vecPItem.back(), n / MaxColumnNum,
                           n % MaxColumnNum);
}

void TableConfigWidget::initUI() {
  m_pHBoxLayout = new QHBoxLayout(this);
  m_pTableLabel = new QLabel();
  m_pGridLayout = new QGridLayout();
  m_pHBoxLayout->addWidget(m_pTableLabel);
  m_pHBoxLayout->addLayout(m_pGridLayout);
  m_pHBoxLayout->setStretch(0, 1);
  m_pHBoxLayout->setStretch(1, 6);
  m_tableConfig = TableConfig();
}

TableConfig &TableConfigWidget::regenerateTableConfig() {
  auto guiTableConfig = TableConfig();
  guiTableConfig.version = m_tableConfig.version + 1;
  guiTableConfig.tableName = m_tableConfig.tableName;
  // 从配置界面中重新生成配置
  for (auto &&i : m_vecPItem) {
    qDebug() << i->field << i->label->text();
    FieldConfig fieldConfig;
    fieldConfig.field = i->field;
    fieldConfig.type = i->fileType;
    fieldConfig.name = i->label->text();
    fieldConfig.status = Item::IndexToFieldStatus(i->combox->currentIndex());
    guiTableConfig.fieldConfig.append(fieldConfig);
  }
  // 比较配置
  auto size = std::min(m_tableConfig.fieldConfig.size(),
                       guiTableConfig.fieldConfig.size());
  if (size != guiTableConfig.fieldConfig.size()) {
    m_tableConfig = guiTableConfig;
    return m_tableConfig;
  }
  for (int i = 0; i < size; i++) {
    if (m_tableConfig.fieldConfig[i] != guiTableConfig.fieldConfig[i]) {
      qDebug() << "TableConfigWidget::regenerateTableConfig()"
               << m_tableConfig.fieldConfig[i].name << " "
               << guiTableConfig.fieldConfig[i].name;
      m_tableConfig = guiTableConfig;
      break;
    }
  }
  return m_tableConfig;
}

void TableConfigWidget::refresh(const TableConfig &tableConfig) {
  if (tableConfig.version < m_tableConfig.version) {
    // 不能等于 因为这是刷新 传人相同版本的配置意味着刷新
    return;
  }
  m_pTableLabel->setText(tableConfig.tableName);
  for (auto &i : m_vecPItem) {
    delete i;
    i = nullptr;
  }
  m_vecPItem.clear();

  for (auto &i : tableConfig.fieldConfig) {
    addFieldConfigItem(i);
  }
  m_tableConfig = tableConfig;
}
