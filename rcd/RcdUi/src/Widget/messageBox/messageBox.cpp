#include "messageBox.h"
#include <QDebug>
messageBox::messageBox()
{
    this->setAttribute(Qt::WA_ShowModal);
}

void messageBox::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Up){
        qDebug()<<"Qt::Key_Up"<<"11111111111111";
    }
    else if (event->key()==Qt::Key_Down) {
        qDebug()<<"Qt::Key_Down"<<"22222222222222";
    }
    else if (event->key()==Qt::Key_Escape) {
        qDebug()<<"Qt::Key_Escape"<<"3333333333333333333";
    }
    else if (event->key()==Qt::Key_Return) {
        qDebug()<<"Qt::Key_Return"<<"44444444444444444444";
    }
    else if (event->key()==Qt::Key_Left) {
        qDebug()<<"Qt::Key_Left"<<"555555555555555";
    }
    else if (event->key()==Qt::Key_Right) {
        qDebug()<<"Qt::Key_Right"<<"666666666666666666666";
    }

    switch(event->key())
    {
//    case Qt::Key_Backspace:
//    {
//        QKeyEvent key(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
//        QMessageBox::keyPressEvent(&key);
//        break;
//    }
    case Qt::Key_Up:
    {
        QKeyEvent key(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
        QMessageBox::keyPressEvent(&key);
        break;
    }
    case Qt::Key_Down:
    {
        QKeyEvent key(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
        QMessageBox::keyPressEvent(&key);
        break;
    }
//    case Qt::Key_Escape:
//    {
//        QKeyEvent key(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
//        QMessageBox::keyPressEvent(&key);
//        break;
//    }
//    case Qt::Key_Return:
//    {
//        QKeyEvent key(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
//        QMessageBox::keyPressEvent(&key);
//        break;
//    }
//    case Qt::Key_Left:
//    {
//        QKeyEvent key(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
//        QMessageBox::keyPressEvent(&key);
//        break;
//    }
//    case Qt::Key_Right:
//    {
//        QKeyEvent key(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
//        QMessageBox::keyPressEvent(&key);
//        break;
//    }
    default:
        //QMessageBox::keyPressEvent(event);		// 其他按键处理交给父类
        break;
    }
}
bool messageBox::event(QEvent *e)
{
    switch (e->type()) {

    }
}
