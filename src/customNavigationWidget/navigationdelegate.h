#ifndef NAVIGATIONDELEGATE
#define NAVIGATIONDELEGATE

#include <QPainter>
#include <QStyledItemDelegate>

/**
 * @brief item数据显示的样式控制和自定义编辑
 *
 */
class NavigationDelegate : public QStyledItemDelegate {
  Q_OBJECT
public:
  NavigationDelegate(QObject *parent);
  ~NavigationDelegate();

  // 返回单元格大小，此函数无需手动调用
  QSize sizeHint(const QStyleOptionViewItem &option,
                 const QModelIndex &index) const override;
  // 重写重画函数
  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const override;
};

#endif