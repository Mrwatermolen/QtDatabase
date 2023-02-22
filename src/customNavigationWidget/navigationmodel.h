#ifndef NAVIGATIONMODEL_H
#define NAVIGATIONMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVector>

/**
 * @brief 仅仅支持二级列表
 *
 */
class NavigationModel : public QAbstractListModel {
  Q_OBJECT
public:
  enum NavLevel { TopLevel = 1, SubLevel, SubSunLevel };

  /**
   * @brief 树形结构的节点
   *
   */
  struct TreeNode {
    QString labelText;          // 字符
    NavLevel level;             // 层级
    int index;                  // 对应的StackPage
    bool collapse;              // 是否折叠
    QList<TreeNode *> children; // 子列表

    TreeNode(QString labelText, NavLevel level, int index, bool collapse,
             QList<TreeNode *> children)
        : labelText(std::move(labelText)), level(level), index(index),
          collapse(collapse), children(std::move(children)) {}

    /**
     * @brief 树形节点的dfs遍历
     *
     * @tparam VST 访问函数
     * @param x 起点
     * @param visit
     */
    template <typename VST> void dfs(TreeNode *x, VST &visit) const {
      if (!x) {
        return;
      }

      visit(x);
      for (auto &i : x->children) {
        dfs(i, visit);
      }
    }
  };

  /**
   * @brief 负责将数据加载到ListView中
   *
   */
  struct ListNode {
    QString labelText;
    TreeNode *pTreeNode;
  };

  explicit NavigationModel(QObject *parent = nullptr);
  ~NavigationModel();

  /**
   * @brief 重载rowCount 返回行数
   *
   * @param parent 父级，默认值为自己
   * @return int 行数
   */
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  /**
   * @brief
   *
   * @param index
   * @param role
   * @return QVariant
   */
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  /**
   * @brief Get the Nav Node Posi Vec object
   *
   * @return const QVector<TreeNode *>&
   */
  const QVector<TreeNode *> &getNavNodePosiVec();

public slots:
  /**
   * @brief 处理展开/收缩状态 TODO:实现多级展开
   *
   * @param index 处理的项目
   */
  void Collapse(const QModelIndex &index);

private:
  QVector<TreeNode *> m_vecTreeNode; // 树形结构的存储
  QVector<ListNode> m_vecListNode;   // 树形结构展开为List存储

  /**
   * @brief 初始化结构
   *
   */
  void initModel();

  /**
   * @brief 将树形结构加载到列表中 TODO:实现多级展开
   *
   */
  void refreshList();
};

#endif