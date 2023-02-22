#ifndef _DATAVIEWDIALOG_H_
#define _DATAVIEWDIALOG_H_

#include "common.h"

#include <QDataWidgetMapper>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLayout>
#include <QSqlTableModel>

class DataViewDialog : public QDialog {
  Q_OBJECT
public:
  DataViewDialog(QWidget *parent = nullptr);
  ~DataViewDialog() = default;

  QWidget *m_pContainer;
  QVBoxLayout *m_pContainerLayout;
  QDialogButtonBox *m_pBtnBox;

public:
  void initUI();
  void build(QSqlTableModel *tableModel,
             const DatabaseTableConfig::TableConfig &tableConfig);
  void refresh(QSqlTableModel *tableModel,
               const DatabaseTableConfig::TableConfig &tableConfig);
  bool validData();

  void setDataMapperCurrentRow(const int row);

private:
  DatabaseTableConfig::TableConfig m_tableConfig;
  QDataWidgetMapper *m_pDataMapper;

  void closeEvent(QCloseEvent *e) override;

private slots:
  void onBtnOkClicked();
  void onBtnCloseClicked();

signals:
};

#endif // _DATAVIEWDIALOG_H_