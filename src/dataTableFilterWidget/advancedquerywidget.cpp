#include "advancedquerywidget.h"
#include "common.h"

#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>

using namespace DatabaseTableConfig;

const int MaxColumnNum = 6;

AdvancedQueryWidget::AdvancedQueryWidget(QWidget *parent) : QWidget(parent) {}

void AdvancedQueryWidget::initUI() {
  m_pMainLayout = new QVBoxLayout{this};
  m_pMainLayout->addWidget(new QPushButton("重置所有"));
  m_pScrollArea = new QScrollArea{this};
  m_pContainer = new QWidget;
  m_pContainer->setLayout(new QGridLayout);
  m_pMainLayout->addWidget(m_pScrollArea);
}

void AdvancedQueryWidget::build(const TableConfig &tableConfig) {
  int row{0}, column{0};
  m_pScrollArea->setWidgetResizable(true); // 允许自动调整子组件的大小
  m_pScrollArea->setWidget(m_pContainer);
  auto gridlayout{dynamic_cast<QGridLayout *>(m_pContainer->layout())};
  gridlayout->setColumnStretch(1, 1);
  gridlayout->setColumnStretch(4, 1);
  for (auto &&i : tableConfig.fieldConfig) {
    if (i.status == FieldStatus::Disable) {
      continue;
    }
    if (i.type == FieldType::Others || i.type == FieldType::Text) {
      continue;
    }

    auto label = new QLabel(i.name);
    auto combo = new QComboBox(m_pContainer);
    auto pushButton = new QPushButton("重置");
    combo->setDuplicatesEnabled(false);
    combo->setCurrentIndex(-1);
    m_vecFetchData.append(FetchData(i, combo));

    gridlayout->addWidget(label, row, (column++ % MaxColumnNum));
    gridlayout->addWidget(combo, row, (column++ % MaxColumnNum));
    gridlayout->addWidget(pushButton, row, (column++ % MaxColumnNum));
    if (MaxColumnNum <= column) {
      row++;
      column = 0;
    }

    connect(combo, &QComboBox::activated, this, [this]() {
      emit this->filterChange(this->getCurrentTableFilterConfig());
    });
    connect(pushButton, &QPushButton::clicked, this, [this, combo]() {
      combo->setCurrentIndex(-1);
      emit this->filterChange(this->getCurrentTableFilterConfig());
    });
  }
  m_tableConfig = tableConfig;
  // TODO

}

void AdvancedQueryWidget::refresh(QSqlTableModel *sqlTableModel,
                                  const TableConfig &tableConfig) {
  if (tableConfig.version < 0 || tableConfig.version < m_tableConfig.version) {
    return;
  }

  if (m_tableConfig.version < tableConfig.version) {
    m_vecFetchData.clear();
    if (m_pContainer) {
      delete m_pContainer;
    }
    m_pContainer = new QWidget;
    m_pContainer->setLayout(new QGridLayout);
    build(tableConfig);
    //return;
  }

  // 从table model中获得
  QList<QStringList> newItems;
  for (auto &&i : tableConfig.fieldConfig) {
    if (i.status == FieldStatus::Disable || i.type == FieldType::Others ||
        i.type == FieldType::Text) {
      continue;
    }
    auto items =
        generateComboboxItem(sqlTableModel, sqlTableModel->fieldIndex(i.field));
    newItems.append(items);
  }

  QStringList preComboText;
  for (auto &&i : m_vecFetchData) {
    preComboText << i.pCombo->currentText();
  }

  int counter = 0;
  for (auto &&i : tableConfig.fieldConfig) {
    if (i.status == FieldStatus::Disable) {
      continue;
    }
    if (i.type == FieldType::Others || i.type == FieldType::Text) {
      continue;
    }

    // 还原未改变的项
    int currentIndex = -1;
    if (!preComboText.empty() && !preComboText[counter].isEmpty()) {
      for (int j = 0; j < newItems[counter].size(); j++) {
        if (newItems[counter][j] != preComboText[counter]) {
          continue;
        }
        currentIndex = j;
        break;
      }
    }

    auto combo = m_vecFetchData[counter].pCombo;
    combo->clear();
    combo->addItems(newItems[counter++]);
    combo->setCurrentIndex(currentIndex);
  }
}

TableFilterConfig AdvancedQueryWidget::getCurrentTableFilterConfig() {
  TableFilterConfig tFC;
  for (auto &&i : this->m_vecFetchData) {
    tFC.fieldConfig.append(i.fieldConfig);
    tFC.data.append(i.pCombo->currentData(Qt::DisplayRole));
  }
  return tFC;
}

QStringList AdvancedQueryWidget::generateComboboxItem(QSqlTableModel *sqlModel,
                                                      int column) {
  QStringList strList;
  QSet<QString> set;
  int row = sqlModel->rowCount();
  for (int i = 0; i < row; i++) {
    auto index = sqlModel->index(i, column);
    if (!index.isValid()) {
      continue;
    }
    auto s = index.data().toString();
    if (s.isEmpty() || set.contains(s)) {
      continue;
    }
    set.insert(s);
    strList << s;
  }
  return strList;
}