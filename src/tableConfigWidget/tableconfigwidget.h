#ifndef _TABLECONFIGWIDGET_H_
#define _TABLECONFIGWIDGET_H_

#include "common.h"

#include <QComboBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

/**
 * @brief 编辑字段配置的最小单元 FormLayout[label Combox]
 *
 */
class Item : public QWidget {
public:
  Item(QWidget *parent, const DatabaseTableConfig::FieldConfig &fielConfig);
  ~Item();

  QLabel *label;     // 字段名称
  QComboBox *combox; // 编辑字段状态
  QString field;
  DatabaseTableConfig::FieldType fileType;

public:
  /**
   * @brief 获得字段状态对应的索引
   *
   * @param fieldStatus 字段状态
   * @return int 索引 若状态无效 返回-1
   */
  static int
  FieldStatusToIndex(const DatabaseTableConfig::FieldStatus &fieldStatus);

public:
  /**
   * @brief 获得索引对应的字段状态 不做检查
   *
   * @param index 索引
   * @return FieldStatus 对应的字段状态
   */
  static DatabaseTableConfig::FieldStatus IndexToFieldStatus(const int &index);
};

/**
 * @brief 用户设置某一表格字段是否显示的配置组件
 *
 */
class TableConfigWidget : public QWidget {
  Q_OBJECT
public:
  TableConfigWidget(QWidget *parent = nullptr);
  ~TableConfigWidget();

  /**
   * @brief 获取当前组件所确定的表格配置
   *
   * @return 表格配置结构体的引用
   */
  DatabaseTableConfig::TableConfig &getTableConfig();

  /**
   * @brief 初始化组件界面 仅初始化必要组件
   *
   */
  void initUI();

  /**
   * @brief 根据传人的字段的配置 更新整个组件
   *
   * @param configs 字段的配置
   */
  void refresh(const DatabaseTableConfig::TableConfig &tableConfig);

private:
  QHBoxLayout *m_pHBoxLayout; // 主布局
  QGridLayout *m_pGridLayout; // 采用栅格布局容纳Item
  QLabel *m_pTableLabel;      // 表的名字
  QVector<Item *> m_vecPItem; // Items
  DatabaseTableConfig::TableConfig m_tableConfig; // 表的配置

  /**
   * @brief 向组件中添加字段配置
   *
   * @param fieldConfig 配置
   */
  void addFieldConfigItem(const DatabaseTableConfig::FieldConfig &fieldConfig);
  /**
   * @brief 根据Item重新生成表的配置
   *
   * @return TableConfig&
   */
  DatabaseTableConfig::TableConfig &regenerateTableConfig();
};

#endif // _TABLECONFIGWIDGET_H_