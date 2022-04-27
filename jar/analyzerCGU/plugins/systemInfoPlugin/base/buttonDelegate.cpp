#include "buttonDelegate.h"
#include <QApplication>
#include <QMouseEvent>
#include <QDialog>
#include <QPainter>
#include <QStyleOption>
#include <QDesktopWidget>

buttonDelegate::buttonDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void buttonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionButton* button = m_btns.value(index);
    if (!button) {
        button = new QStyleOptionButton();
        button->rect = option.rect.adjusted(20, 2, -20, -2);
        button->text = "kill";
        button->state |= QStyle::State_Enabled;

        (const_cast<buttonDelegate *>(this))->m_btns.insert(index, button);
    }
    painter->save();

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());

    }
    painter->restore();
    QApplication::style()->drawControl(QStyle::CE_PushButton, button, painter);


}

bool buttonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress) {

        QMouseEvent* e =(QMouseEvent*)event;

        if (option.rect.adjusted(4, 4, -4, -4).contains(e->x(), e->y()) && m_btns.contains(index)) {
            m_btns.value(index)->state |= QStyle::State_Sunken;
        }
    }
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* e =(QMouseEvent*)event;

        if (option.rect.adjusted(4, 4, -4, -4).contains(e->x(), e->y()) && m_btns.contains(index)) {
            m_btns.value(index)->state &= (~QStyle::State_Sunken);

//            QDialog *d = new QDialog();

//            d->setGeometry(0, 0, 200, 200);
//            d->move(QApplication::desktop()->screenGeometry().center() - d->rect().center());
//            d->show();
            emit signalBtnDelegate(index);
        }
    }
}
