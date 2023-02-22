#ifndef _DATATABLEMANAGERWITTABLECONFIG_H_
#define _DATATABLEMANAGERWITTABLECONFIG_H_

#include "common.h"
#include "dataTableManger.h"

class DataTableMangerWithTableConfig;
class DataTableMangerWithTableConfig : public DataTableManger {
  Q_OBJECT
public:
  explicit DataTableMangerWithTableConfig(
      const DatabaseTableConfig::TableConfig &tableConfig, QWidget *parent);
  ~DataTableMangerWithTableConfig() = default;

private:
  DatabaseTableConfig::TableConfig m_tableConfig;
  int m_lastTableConfigVersion{-1};
  QAction *m_pActionHiddenTableItem{nullptr};
  QMenu *m_pTableItem{nullptr};

signals:
  void currentSelectedRowChnaged(const QModelIndex &current,
                                 const QModelIndex &previous);

public:
  void build() override;
  void refresh() override;

  const DatabaseTableConfig::TableConfig &getTableConfig();
  void setTableConfig(const DatabaseTableConfig::TableConfig &tableConfig);

private:
  void setTableMangerWithTableConfig();
  void refreshTableItemMenu(const QModelIndex &index);

private slots:
  void onCurrentSelectedRowChnaged(const QModelIndex &current,
                                   const QModelIndex &previous);
};

#endif // _DATATABLEMANAGERWITTABLECONFIG_H_