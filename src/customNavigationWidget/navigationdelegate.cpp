#include "navigationdelegate.h"
#include "navigationmodel.h"

NavigationDelegate::NavigationDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

NavigationDelegate::~NavigationDelegate() {
  qDebug() << "NavigationDelegate::~NavigationDelegate()";
}

QSize NavigationDelegate::sizeHint(const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const {
  return {80, 60};
}

/**
 * @brief 重写绘制函数
 *
 * @param painter
 * @param option
 * @param index
 */
void NavigationDelegate::paint(QPainter *painter,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index) const {
  NavigationModel::TreeNode *pTreeNode =
      static_cast<NavigationModel::TreeNode *>(
          index.data(Qt::UserRole).value<void *>()); // 获取单元格中的数据

  painter->setRenderHints(QPainter::Antialiasing); // 反走样

  // 绘制单元格边框 略小于单元格大小
  QRect rect = QRect(option.rect.left() + 3, option.rect.top() + 3,
                     option.rect.width() - 6, option.rect.height() - 6);

  // 绘制不同状态时的单元格的样式
  if (option.state.testFlag(QStyle::State_Selected)) {
    // 被选中
    painter->setPen(QPen(Qt::blue));
    painter->setBrush(QColor(229, 241, 255));
    painter->drawRoundedRect(rect, 1, 1);
  } else if (option.state.testFlag(QStyle::State_MouseOver)) {
    painter->setPen(QPen(Qt::green));
    painter->setBrush(Qt::NoBrush);
    painter->drawRoundedRect(rect, 1, 1);
  } else {
    painter->setPen(QPen(Qt::gray));
    painter->setBrush(Qt::NoBrush);
    painter->drawRoundedRect(rect, 1, 1);
  }

  // //根据绘制时提供的信息进行背景色绘制
  // if (option.state & QStyle::State_Selected) {
  //   painter->fillRect(option.rect, QColor(133, 153, 216));
  // } else if (option.state & QStyle::State_MouseOver) {
  //   painter->fillRect(option.rect, QColor(209, 216, 240));
  // } else {
  //   if (pTreeNode->level == NavigationModel::TopLevel)
  //     painter->fillRect(option.rect, QColor(247, 249, 255));
  //   else
  //     painter->fillRect(option.rect, QColor(239, 241, 250));
  // }

  // 数据绘制
  // 层级偏移
  int offset = 20 * (pTreeNode->level - 1);
  QRect labelTextRect =
      QRect(rect.left(), rect.top(), rect.width() - offset, rect.height());
  labelTextRect.setX(labelTextRect.x() + offset);
  // 绘制文字
  painter->drawText(labelTextRect, Qt::AlignLeft | Qt::AlignVCenter,
                    index.data(Qt::DisplayRole).toString());
}