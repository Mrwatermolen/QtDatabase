#ifndef _DASHBOARDPAGE_H_
#define _DASHBOARDPAGE_H_

#include "common.h"
#include "tableconfigwidget.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class DashboardPage : public QWidget {
  Q_OBJECT
public:
  DashboardPage(QWidget *parent = nullptr);
  ~DashboardPage();

private:
  QString m_configPath; // 配置文件路径
  QVector<DatabaseTableConfig::TableConfig> m_vecTableConfig; // 配置
  QVBoxLayout *m_pVBoxLayout{nullptr};                        // 主布局
  QVector<TableConfigWidget *> m_vecPTableConfigWidget; // 编辑配置的组件
  QHBoxLayout *m_pBtnHboxLayout{nullptr};
  QVector<QPushButton *> m_vecPBtn;

signals:
  /**
   * @brief 确认配置按钮点击后 Dashboard检查通过配置提交后所发射的信号
   *
   */
  void signalFinishConfig();

  /**
   * @brief 将当前的配置写入配置文件中
   *
   * @param configPath
   */
  void writeConfig(const QString &configPath);

public:
  /**
   * @brief 根据传入的配置构建
   *
   * @param tableConfigs
   */
  void build(const QVector<DatabaseTableConfig::TableConfig> &tableConfigs);

  /**
   * @brief 向Dashboard中添加与tableConfig绑定的表格配置组件
   *
   * @param tableConfig
   */
  void
  addTableConfigWidget(const DatabaseTableConfig::TableConfig &tableConfig);

  /**
   * @brief 刷新Dashboard页面 传入相同版本的配置意味着刷新界面
   *
   * @param tableConfig
   */
  void refresh(const QVector<DatabaseTableConfig::TableConfig> &tableConfigs);

  /**
   * @brief 获得Dashborad内存中所存储的配置 不是配置界面所决定的配置
   *
   * @return QVector<TableConfig>&
   */
  auto getTableConfigs() -> QVector<DatabaseTableConfig::TableConfig> & {
    return m_vecTableConfig;
  }

private:
  /**
   * @brief 初始化UI 仅初始化必要组件
   *
   */
  void initUI();

private slots:
  /**
   * @brief 槽函数 提交配置
   *
   */
  void submitConfigChange();
  /**
   * @brief 槽函数 取消用户未提交的修改 重新载入
   *
   */
  void revertConfig();
};

#endif // _DASHBOARDPAGE_H_