#ifndef _DATATABLEMANGERPAGE_H_
#define _DATATABLEMANGERPAGE_H_

#include "advancedquerywidget.h"
#include "common.h"
#include "dataTableManagerWithTableConfig.h"
#include "dataViewDialog.h"

#include <QDataWidgetMapper>
#include <QPushButton>

class DataTableMangerPage;

class DataTableMangerPage : public QWidget {
  Q_OBJECT
public:
  explicit DataTableMangerPage(
      const DatabaseTableConfig::TableConfig &tableconfig,
      QWidget *parent = nullptr);
  ~DataTableMangerPage() = default;

private:
  DatabaseTableConfig::TableConfig m_tableConfig{
      DatabaseTableConfig::TableConfig()};
  DataTableMangerWithTableConfig *m_pTableManager{nullptr};
  QDataWidgetMapper *m_pDataMapper{nullptr};
  DataViewDialog *m_pDataViewDialog{nullptr};
  AdvancedQueryWidget *m_pAdvancedQueryView{nullptr};
  QPushButton *m_pBtnAdd{nullptr}, *m_pBtnDelete{nullptr}, *m_pBtnEdit{nullptr},
      *m_pBtnQuery{nullptr};

public:
  void build();
  void refresh(const DatabaseTableConfig::TableConfig &tableConfig);

private:
  void initUI();
  void setDataMapperCurrentRow(const int row);
  void setDataViewDialogDataMapperCurrentRow(const int row);

private slots:
  void onTableCurrentRowChanged(const QModelIndex &current,
                                const QModelIndex &previous);

  void onBtnAddClicked();
  void onBtnDeleteClicked();
  void onBtnEditClicked();
  void onBtnAdvancedQueryClicked();

  void onDialogAccepted();
  void onDialogRejected();

  void onAdvancedQueryFilterChanged(
      const DatabaseTableConfig::TableFilterConfig &tableFilterConfig);

  //void dragEnterEvent(QDragEnterEvent *event) override;
  //void dropEvent(QDropEvent *event) override;
};

#endif // _DATATABLEMANGERPAGE_H_