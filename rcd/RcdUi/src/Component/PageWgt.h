#ifndef PageWgt_H
#define PageWgt_H

#include <QWidget>
#include <QPaintEvent>
#include <QTimer>

class PageWgt : public QWidget
{
    Q_OBJECT

public:

    explicit PageWgt(QWidget *parent = nullptr);
    void     setMaxPage(quint8 value);
    void     setPage(quint8 value);

protected:

    virtual void paintEvent(QPaintEvent *) override;

private slots:

    void timerHandle();

private:

    QTimer         m_timer;

    quint8         m_currentPage{0};
    quint8         m_maxPage{5};

};

#endif // PageWgt_H
