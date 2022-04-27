#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include <QPixmap>

class TGFrame;

class CameraWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CameraWidget(QWidget *parent = nullptr);
    void     startPlay(QString &url);
    void     stopPlay();
    void     setShadowType(int);

public slots:

    void asyncUpdateUi(QPixmap img);
    void reconnect();

signals:

    void sglResize(int w,int h);

protected:

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    QPixmap        m_videoImage;                         //记录当前的图像
    TGFrame*       m_pFrame;
    QString        m_playUrl;
    bool           m_isPlay;
    bool           m_isWaitingReconn;
};

#endif // CAMERAWIDGET_H
