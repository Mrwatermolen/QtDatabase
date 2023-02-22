#ifndef _MEMERYFREETESTWIDGET_H_
#define _MEMERYFREETESTWIDGET_H_

#include <QWidget>

/**
 * @brief 测试一下会不会释放内存
 *
 */
class MemeryFreeTestWidget : public QWidget {
  public:
  MemeryFreeTestWidget(QString name, QWidget *parent = nullptr)
      : m_name(std::move(name)), QWidget(parent) {
    qDebug() << "Memery free test start. Name" << m_name;
  }
  ~MemeryFreeTestWidget() {
    qDebug() << "Memery free test End. Name" << m_name;
  }
  QString m_name;
};

#endif // _MEMERYFREETESTWIDGET_H_