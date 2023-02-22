#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
#include "dashboardPage.h"

#include <QItemSelection>
#include <QListView>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QStackedWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  QListView *m_pNavListView;
  QStackedWidget *m_pStackPage;
  DashboardPage *m_pDashboardPage;
  QVector<DatabaseTableConfig::TableConfig> m_vecTableConfig;

private:
  void initUI();

  DashboardPage *createDashboardPageWidget();

  void buildNavigationWidget();
  void buildStackPageWidget();
  void buildDashboardWidget();

  /**
   * @brief 读取配置文件 为Dashboard生成配置
   *
   * @param configPath 配置文件路径 仅接受JSON文件
   * @return QVector<DatabaseTableConfig::TableConfig> 文件对应的配置
   */
  QVector<DatabaseTableConfig::TableConfig>
  readConfig(const QString &configPath);
  void writeTableConfigToJson(const QString &configPath);

  void connectDatabase();
  void getFullTableColumn();

  void switchPageToDashBoardPage();
  void switchPageToDatabaseTablePage(const int pageIndex);

private slots:
  /**
   * @brief 槽函数 处理页面切换
   *
   * @param selection
   */
  void switchPage(const QItemSelection &selection);

  /**
   * @brief 槽函数 更新表格配置
   *
   */
  void updateTableConfigs();

  bool eventFilter(QObject *watched, QEvent *event) override;

  void testImage(const QByteArray& data);
};
#endif // MAINWINDOW_H
