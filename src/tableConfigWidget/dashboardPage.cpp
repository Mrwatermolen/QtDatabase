#include "dashboardPage.h"
#include "tableconfigwidget.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

using namespace DatabaseTableConfig;

DashboardPage::DashboardPage(QWidget *parent) : QWidget(parent) {
  initUI();
}

DashboardPage::~DashboardPage() {
  qDebug() << "DashboardPage::~DashboardPage()";
}

void DashboardPage::initUI() {
  m_pVBoxLayout = new QVBoxLayout(this);

  m_pBtnHboxLayout = new QHBoxLayout();
  m_vecPBtn.append(new QPushButton("确认"));
  m_vecPBtn.emplace_back(new QPushButton("撤回"));
  m_pBtnHboxLayout->addWidget(m_vecPBtn[0]);
  m_pBtnHboxLayout->addWidget(m_vecPBtn[1]);
  m_pVBoxLayout->addLayout(m_pBtnHboxLayout);
}

void DashboardPage::build(const QVector<TableConfig> &tableConfigs) {
  connect(m_vecPBtn[0], &QPushButton::clicked, this,
          &DashboardPage::submitConfigChange);
  connect(m_vecPBtn[1], &QPushButton::clicked, this,
          &DashboardPage::revertConfig);
  for (auto &&i : tableConfigs) {
    addTableConfigWidget(i);
  }
}

void DashboardPage::addTableConfigWidget(const TableConfig &tableConfig) {
  if (tableConfig.version < 0) {
    return;
  }

  auto p = new TableConfigWidget(this);
  p->initUI();
  p->refresh(tableConfig);
  // 插入
  m_pVBoxLayout->insertWidget(m_pVBoxLayout->count() - 1, p);
  // 添加水平分割线
  auto hLine = new QFrame();
  hLine->setFrameShape(QFrame::HLine);
  m_pVBoxLayout->insertWidget(m_pVBoxLayout->count() - 1, hLine);

  m_vecPTableConfigWidget.append(p);
  m_vecTableConfig.append(tableConfig);
}

void DashboardPage::refresh(const QVector<TableConfig> &tableConfigs) {
  if (tableConfigs.size() != m_vecPTableConfigWidget.size()) {
    qDebug() << "DashboardPage::refreshDashboardPage() Error: tableConfig.size:"
             << tableConfigs.size() << "!= m_vecPTableConfigWidget.size()"
             << m_vecPTableConfigWidget.size();
    return;
  }
  for (int i = 0; i < tableConfigs.size(); i++) {
    if (tableConfigs[i].version < m_vecTableConfig[i].version) {
      return;
    }
    m_vecPTableConfigWidget[i]->refresh(tableConfigs[i]);
    m_vecTableConfig[i] = tableConfigs[i];
  }
}

void DashboardPage::submitConfigChange() {
  m_vecTableConfig.clear();
  for (auto &&i : m_vecPTableConfigWidget) {
    m_vecTableConfig.emplace_back(i->getTableConfig());
  }
  emit signalFinishConfig();
  QMessageBox::information(this, "配置提交成功",
                           "配置提交成功 当前1配置版1:" +
                               QString::number(m_vecTableConfig[0].version));
}

void DashboardPage::revertConfig() {
  for (int i = 0; i < m_vecTableConfig.size(); i++) {
    m_vecPTableConfigWidget[i]->refresh(m_vecTableConfig[i]);
  }
  QMessageBox::information(this, "配置撤销成功", "配置撤销成功！！");
}