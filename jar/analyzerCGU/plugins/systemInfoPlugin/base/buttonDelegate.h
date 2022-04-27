#ifndef BUTTONDELEGATE_H
#define BUTTONDELEGATE_H

#include <QItemDelegate>
class buttonDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit buttonDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
   void signalBtnDelegate(const QModelIndex &index);
public slots:

private:
    QMap<QModelIndex, QStyleOptionButton*> m_btns;
};

#endif // BUTTONDELEGATE_H
