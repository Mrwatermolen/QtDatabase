#include "navigationmodel.h"

NavigationModel::NavigationModel(QObject *parent) : QAbstractListModel(parent) {
  initModel();
}

NavigationModel::~NavigationModel() {
  qDebug() << "NavigationModel::~NavigationModel()";
  // 释放内存
  for (auto &i : m_vecTreeNode) {
    if (i->children.empty()) {
      delete i;
      continue;
    }

    for (auto &j : i->children) {
      delete j;
    }
    delete i;
  }

  m_vecListNode.clear();
  m_vecTreeNode.clear();
}

int NavigationModel::rowCount(const QModelIndex &parent) const {
  return m_vecListNode.size();
}

QVariant NavigationModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) {
    return {};
  }

  if (m_vecListNode.size() < index.row() || index.row() < 0) {
    return {};
  }

  if (role == Qt::DisplayRole) {
    return m_vecListNode[index.row()].labelText;
  }

  if (role == Qt::UserRole) {
    return QVariant::fromValue((void *)m_vecListNode[index.row()].pTreeNode);
  }

  return {};
}

const QVector<NavigationModel::TreeNode *> &
NavigationModel::getNavNodePosiVec() {
  return m_vecTreeNode;
}

void NavigationModel::Collapse(const QModelIndex &index) {
  TreeNode *pTreeNode =
      m_vecListNode[index.row()].pTreeNode; // 获取当前点击节点

  if (pTreeNode->children.empty()) {
    // 如果该节点没有子节点 则返回
    return;
  }

  pTreeNode->collapse = !pTreeNode->collapse;
  if (!pTreeNode->collapse) {
    // 未展开
    beginRemoveRows(QModelIndex(), index.row() + 1, pTreeNode->children.size());
    endRemoveRows();
  } else {
    // 展开
    beginInsertRows(QModelIndex(), index.row() + 1, pTreeNode->children.size());
    endInsertRows();
  }
  refreshList();
}

void NavigationModel::initModel() {
  int stackPage = 0;
  auto *pTreeNode = new TreeNode("Dashboard", TopLevel, stackPage++, false,
                                     QList<TreeNode *>());
  m_vecTreeNode.push_back(pTreeNode);

  pTreeNode =
      new TreeNode("Database", TopLevel, -1, false, QList<TreeNode *>());
  pTreeNode->children.push_back(new TreeNode("Raw Image", SubLevel, stackPage++,
                                             false, QList<TreeNode *>()));
  pTreeNode->children.push_back(new TreeNode(
      "Experiment", SubLevel, stackPage++, false, QList<TreeNode *>()));
  pTreeNode->children.push_back(new TreeNode("Others", SubLevel, stackPage++,
                                             false, QList<TreeNode *>()));
  m_vecTreeNode.push_back(pTreeNode);

  pTreeNode =
      new TreeNode("About", TopLevel, stackPage++, false, QList<TreeNode *>());
  m_vecTreeNode.push_back(pTreeNode);

  refreshList(); //刷新界面标题栏展示数据
  beginInsertRows(QModelIndex(), 0, m_vecListNode.size()); //插入所有节点
  endInsertRows();                                         //结束插入
}

void NavigationModel::refreshList() {
  m_vecListNode.clear();
  for (auto & it : m_vecTreeNode) {
    //一级节点
    ListNode node;
    node.labelText = it->labelText;
    node.pTreeNode = it;
    m_vecListNode.push_back(node);

    if (!it->collapse) {
      //如果一级节点未展开，则插入下一一级节点
      continue;
    }

    for (auto & child : it->children) {
      ListNode node;
      node.labelText = child->labelText;
      node.pTreeNode = child;
      m_vecListNode.push_back(node);
    }
  }
}