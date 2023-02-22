#ifndef _customDatetimeDelegate_H_
#define _customDatetimeDelegate_H_

#include "common.h"

#include <QStyledItemDelegate>

class customDatetimeDelegate;

class customDatetimeDelegate : public QStyledItemDelegate {
  Q_OBJECT
public:
  customDatetimeDelegate(QWidget *parent = nullptr);
  ~customDatetimeDelegate();

  // 自定义代理必须实现的函数 是吗

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model,
                    const QModelIndex &index) const override;
};

#endif // _customDatetimeDelegate_H_