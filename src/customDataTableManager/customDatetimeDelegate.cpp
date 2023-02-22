#include "customDatetimeDelegate.h"

#include <QCalendarWidget>
#include <memory>

customDatetimeDelegate::customDatetimeDelegate(QWidget *parent)
    : QStyledItemDelegate(parent) {}

customDatetimeDelegate::~customDatetimeDelegate() = default;

QWidget *customDatetimeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const {
  auto calender{new QCalendarWidget};
  return calender;
}

void customDatetimeDelegate::setEditorData(QWidget *editor,
                                           const QModelIndex &index) const {
  if (!index.isValid()) {
    return;
  }

  QDate data{index.data().toDate()};
  auto calender{dynamic_cast<QCalendarWidget *>(editor)};
  calender->setSelectedDate(data);
}

void customDatetimeDelegate::setModelData(QWidget *editor,
                                          QAbstractItemModel *model,
                                          const QModelIndex &index) const {
  auto calender{dynamic_cast<QCalendarWidget *>(editor)};
  auto data{calender->selectedDate()};
  model->setData(index, data, Qt::EditRole);
}
