#ifndef _ADVANCEDQUERYWIDGET_H_
#define _ADVANCEDQUERYWIDGET_H_

#include "common.h"

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QListView>
#include <QSqlTableModel>
#include <QWidget>
#include <QScrollArea>

class AdvancedQueryWidget : public QWidget {
  Q_OBJECT
public:
  AdvancedQueryWidget(QWidget *parent = nullptr);
  ~AdvancedQueryWidget() = default;

  void initUI();

  void build(const DatabaseTableConfig::TableConfig &tableConfig);

  /**
   * @brief 刷新combobox中的内容
   *
   * @param sqlModel
   * @param tableConfig
   */
  void refresh(QSqlTableModel *sqlModel,
               const DatabaseTableConfig::TableConfig &tableConfig);

  /**
   * @brief 获得当前的filter
   *
   * @return TableFilterConfig
   */
  DatabaseTableConfig::TableFilterConfig getCurrentTableFilterConfig();
signals:
  // 可以传引用吗
  void filterChange(DatabaseTableConfig::TableFilterConfig);

private:
  struct FetchData {
    DatabaseTableConfig::FieldConfig fieldConfig{};
    QComboBox *pCombo{};
    FetchData(DatabaseTableConfig::FieldConfig fc, QComboBox *c)
        : fieldConfig(std::move(fc)), pCombo(c){};
  };
  DatabaseTableConfig::TableConfig m_tableConfig{};
  QVBoxLayout *m_pMainLayout{};
  QScrollArea *m_pScrollArea{};
  QWidget *m_pContainer{};
  QVector<FetchData> m_vecFetchData{};

  /**
   * @brief 生成对应字段的筛选列表 去除重复
   *
   * @param sqlModel
   * @param column 字段在TableModel的列数
   * @return QStringList
   */
  QStringList generateComboboxItem(QSqlTableModel *sqlModel, int column);
};

#endif // _ADVANCEDQUERYWIDGET_H_