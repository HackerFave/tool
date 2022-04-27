#ifndef TitleWgt_H
#define TitleWgt_H

#include <QWidget>
#include <QPaintEvent>
#include <QTimer>

class TitleWgt : public QWidget
{
    Q_OBJECT

public:

    explicit TitleWgt(QWidget *parent = nullptr);
    void     setTitleString(QString &str);

protected:

    virtual void paintEvent(QPaintEvent *) override;

private:
    QPixmap        m_titleImage;

    QString        m_titleStr;

};

#endif // TitleWgt_H
