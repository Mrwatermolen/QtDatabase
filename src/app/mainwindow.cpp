#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "common.h"
#include "dataTableManagerWithTableConfig.h"
#include "dataTableMangerpage.h"
#include "navigationdelegate.h"
#include "navigationmodel.h"

// #include <customDataTableManager.h>

#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QListView>
#include <QSplitter>
#include <QSqlError>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QtSql>

using namespace DatabaseTableConfig;
using NavNodePosi = NavigationModel::TreeNode *;

const QString ConfigPath{"/Users/franzero/QtProject/Database/config.json"};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  initUI();
  connectDatabase();
  buildNavigationWidget();
  buildStackPageWidget();
}

MainWindow::~MainWindow() {
  delete ui;
  QSqlDatabase::database().close();
}

void MainWindow::initUI() {
  m_vecTableConfig = readConfig(ConfigPath);
  setWindowTitle("Main");

  auto *splitter = new QSplitter(this);
  splitter->setOpaqueResize(false);
  setCentralWidget(splitter);

  m_pNavListView = new QListView();
  m_pStackPage = new QStackedWidget();
  splitter->addWidget(m_pNavListView);
  splitter->addWidget(m_pStackPage);
  splitter->setStretchFactor(0, 1);
  splitter->setStretchFactor(1, 2);
}

DashboardPage *MainWindow::createDashboardPageWidget() {
  auto *dashPage{new DashboardPage()};
  return dashPage;
}

void MainWindow::buildNavigationWidget() {
  // navModel 一定要设置parent 不然不会自动释放内存
  auto navModel = new NavigationModel(m_pNavListView);
  auto navDelegate = new NavigationDelegate(navModel);
  m_pNavListView->setTabKeyNavigation(false);
  m_pNavListView->setSelectionMode(QAbstractItemView::SingleSelection);
  m_pNavListView->setModel(navModel);
  m_pNavListView->setItemDelegate(navDelegate);
  connect(m_pNavListView, &QListView::clicked, navModel,
          &NavigationModel::Collapse);
  connect(m_pNavListView->selectionModel(),
          &QItemSelectionModel::selectionChanged, this,
          &MainWindow::switchPage);
}

void MainWindow::buildStackPageWidget() {
  m_pDashboardPage = createDashboardPageWidget();
  m_pStackPage->addWidget(m_pDashboardPage);
  buildDashboardWidget();

  auto rawImage{new DataTableMangerPage(m_vecTableConfig[0], this)};
  rawImage->build();
  m_pStackPage->addWidget(rawImage);

  auto ex{new DataTableMangerPage(m_vecTableConfig[1], this)};
  ex->build();
  m_pStackPage->addWidget(ex);
  m_pStackPage->installEventFilter(this);
  m_pStackPage->setAcceptDrops(true);
}

void MainWindow::buildDashboardWidget() {
  m_pDashboardPage->build(m_vecTableConfig);
  connect(m_pDashboardPage, &DashboardPage::signalFinishConfig, this,
          &MainWindow::updateTableConfigs);
}

QVector<TableConfig> MainWindow::readConfig(const QString &configPath) {
  QFile config(configPath);
  if (!config.open(QIODevice::ReadOnly)) {
    qDebug() << "MainWindow::readConfig() config path:" << configPath
             << "Error:" << config.errorString();
    return {};
  }

  QByteArray data(config.readAll());
  config.close();
  QJsonParseError jError;
  // 使用QJsonDocument的fromJson函数读取json串，并将QJsonParseError对象传入获取错误值
  QJsonDocument jDoc = QJsonDocument::fromJson(data, &jError);
  // 判断QJsonParseError对象获取的error是否包含错误
  if (jError.error != QJsonParseError::NoError) {
    qDebug() << "MainWindow::readConfig() Error:" << jError.errorString();
    return {};
  }
  auto jObj = jDoc.object()["tableConfig"].toObject();
  auto tableNames = QStringList() << "RawImage"
                                  << "Experiment"
                                  << "Others";

  // 创建配置
  QVector<TableConfig> tableConfigs;
  qDebug() << jObj.keys();
  for (auto &&table : tableNames) {
    auto jArr = jObj[table].toArray();
    TableConfig config;
    config.tableName = table;
    config.version = 0;

    for (auto &&i : jArr) {
      FieldConfig fieldConfig;
      auto key = i.toObject().keys().first();
      fieldConfig.field = key;
      fieldConfig.name = i[key].toObject()["name"].toString();
      fieldConfig.status = StrToStatus[i[key].toObject()["status"].toString()];
      fieldConfig.type = StrToType[i[key].toObject()["type"].toString()];
      config.fieldConfig.append(fieldConfig);
    }

    tableConfigs.append(config);
  }
  return tableConfigs;
}

void MainWindow::writeTableConfigToJson(const QString &configPath) {
  for (auto &&tableConfig : m_vecTableConfig) {
    auto jObj{QJsonObject()};
    for (auto &&i : tableConfig.fieldConfig) {
      QString field(i.field);
      // auto JObjStatus{QJsonObject{std::make_pair("status", i.status)}}
    }
  }
  QFile config(configPath);
  if (!config.open(QIODevice::ReadOnly)) {
    qDebug() << "MainWindow::writeTableConfigToJson() config path:"
             << configPath << "Error:" << config.errorString();
    return;
  }

  QByteArray data(config.readAll());
  config.close();
  QJsonParseError jError;
  // 使用QJsonDocument的fromJson函数读取json串，并将QJsonParseError对象传入获取错误值
  QJsonDocument jDoc = QJsonDocument::fromJson(data, &jError);
  // 判断QJsonParseError对象获取的error是否包含错误
  if (jError.error != QJsonParseError::NoError) {
    qDebug() << "MainWindow::readConfig() Error:" << jError.errorString();
    return;
  }
  auto jObj = jDoc.object();
  auto rObj{jObj["tableConfig"].toObject()["RawImage"]};
  qDebug() << rObj.toArray()[0].toObject()["id"].toObject()["name"];
  rObj.toArray()[0].toObject()["id"].toObject()["name"] = "编号吗";
  qDebug() << rObj.toArray()[0].toObject()["id"].toObject()["name"];
  jDoc.object()["tableConfig"].toObject()["RawImage"] = rObj;
  if (!config.open(QIODevice::WriteOnly)) {
    return;
  }
  jDoc.setObject(jObj);
  config.seek(0);
  config.write(jDoc.toJson());
  config.flush();
  config.close();
}

void MainWindow::connectDatabase() {
  auto db = QSqlDatabase::addDatabase("QMYSQL");
  db.setHostName("localhost");
  db.setDatabaseName("QtSqlTest");
  db.setUserName("root");
  db.setPassword("qwer1234");
  bool ok = db.open();
  if (!ok) {
    qDebug() << "MainWindow::connectDatabase() Open Database fail."
             << db.lastError().text();
    return;
  }
  qDebug() << "MainWindow::connectDatabase() Open Database OK."
           << db.lastError().text();
}

void MainWindow::getFullTableColumn() {
  if (!QSqlDatabase::database().isOpen()) {
    return;
  }

  QSqlQuery query;
  query.exec("SHOW FULL COLUMNS FROM Experiment");
  qDebug() << query.lastError();
  while (query.next()) {
    qDebug() << query.value(0) << query.value(1) << query.value(2);
  }
}

void MainWindow::switchPage(const QItemSelection &selection) {
  if (selection.indexes().isEmpty()) {
    qDebug() << "MainWindow::switchPage() selection.indexes().isEmpty()";
    return;
  }
  auto modelIndex = selection.indexes().first();
  if (!modelIndex.isValid()) {
    return;
  }

  NavNodePosi pTreeNode =
      static_cast<NavNodePosi>(modelIndex.data(Qt::UserRole).value<void *>());
  auto pageIndex = pTreeNode->index;
  if (pageIndex == -1 || m_pStackPage->count() <= pageIndex) {
    return;
  }
  qDebug() << "MainWindow::switchPage() switch to " << pTreeNode->index;

  switch (pTreeNode->index) {
  case 0: {
    // 进入Dashboard
    auto dashboardPage = static_cast<DashboardPage *>(m_pStackPage->widget(0));
    dashboardPage->refresh(m_vecTableConfig);
    break;
  }
  case 1: {
    auto index = pTreeNode->index;
    auto p = dynamic_cast<DataTableMangerWithTableConfig *>(
        m_pStackPage->widget(index));
    // p->setTableConfig(m_vecTableConfig[index - 1]);
    // p->refresh();
    break;
  }
  case 2: {
    auto index = pTreeNode->index;
    auto p = dynamic_cast<DataTableMangerPage *>(m_pStackPage->widget(index));
    p->refresh(m_vecTableConfig[index - 1]);
  }
  case 4: {
    // 其他界面
    break;
  }
  default: {
  }
  }

  m_pStackPage->setCurrentIndex(pTreeNode->index);
}

void MainWindow::updateTableConfigs() {
  auto configs = m_pDashboardPage->getTableConfigs();
  for (int i = 0; i < configs.size(); i++) {
    if (m_vecTableConfig.size() == i) {
      m_vecTableConfig.append(configs[i]);
      continue;
    }
    if (configs[i].version <= m_vecTableConfig[i].version) {
      continue;
    }
    qDebug() << "MainWindow::updateTableConfigs()"
             << m_vecTableConfig[i].tableName
             << "Ver:" << m_vecTableConfig[i].version << "to"
             << configs[i].version;
    m_vecTableConfig[i] = configs[i];
  }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
  if (watched == m_pStackPage) {
    if (event->type() == QEvent::DragEnter) {
      auto dragEvent{dynamic_cast<QDragEnterEvent *>(event)};
      if (dragEvent->mimeData()->hasUrls()) {
        // 接受本地文件的拖拽事件
        dragEvent->acceptProposedAction();
      }
    } else if (event->type() == QEvent::Drop) {
      auto dropEvent{dynamic_cast<QDropEvent *>(event)};
      QList<QUrl> urls = dropEvent->mimeData()->urls();
      if (urls.isEmpty()) {
        return true;
      }
      QString path = urls.first().toLocalFile();
      qDebug() << path;
    }
  }
  return QWidget::eventFilter(watched, event);
}

void MainWindow::testImage(const QByteArray& data) {
  auto l{new QLabel(this)};
  qDebug() << data.size();
  auto image{QImage()};
  image.loadFromData(data);
  l->setPixmap(QPixmap::fromImage(image));
  setCentralWidget(l);
}