#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QMessageBox>
#include <QKeyEvent>
class messageBox : public QMessageBox
{
public:
    messageBox();
protected:
    void keyPressEvent(QKeyEvent *event);
    bool event(QEvent *e);
};

#endif // MESSAGEBOX_H
