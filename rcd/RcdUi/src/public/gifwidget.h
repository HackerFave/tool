#ifndef GIFWIDGET_H
#define GIFWIDGET_H


/**
 * 1. 可设置要录制屏幕的宽高,支持右下角直接拉动改变.
 * 2. 可设置变宽的宽度
 * 3. 可设置录屏控件的背景颜色
 * 4. 可设置录制的帧数
 * 5. 录制区域可自由拖动选择
 */

#include <QDialog>
#include "gif.h"

class QLineEdit;
class QLabel;

#ifdef quc
class Q_DECL_EXPORT GifWidget : public QDialog
#else
class GifWidget : public QDialog
#endif

{
    Q_OBJECT

    Q_PROPERTY(int _borderWidth READ getBorderWidth WRITE setBorderWidth)
    Q_PROPERTY(QColor _bgColor READ getBgColor WRITE setBgColor)

public:
    enum Record_mode{
        record_start,
        record_stop
    };
    static GifWidget *Instance();
    explicit GifWidget( QString screenMode,QWidget *parent = 0);
public:
    int getBorderWidth()        const;
    QColor getBgColor()         const;
    void setWidthHeight(int width, int height);
    bool createFile(const QString& filename);
    void setGifRect();//获取截屏区域
    void setFps(int fps){//设置帧率
        _fps = fps;
    }
protected:
    bool eventFilter(QObject *watched, QEvent *event);
   // void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

private:
    static QScopedPointer<GifWidget> P_self;
    QWidget *_widgetMain;        //中间部分
    int _fps;                    //帧数 100为1s
    int _borderWidth;            //边框宽度
    QColor _bgColor;             //背景颜色

    int _count;                  //帧数计数
    QString _fileName;           //保存文件名称
    QRect _rectGif;              //截屏区域
    QTimer *_timer;              //截屏定时器

    Gif P_gif;                    //gif类对象
    Gif::GifWriter *P_gifWriter;  //gif写入对象
    QScreen *_screen ;
    int _width;
    int _height;
    QString _screenMode;
public slots:
     void slotRecord(int flag);//记录
private slots:
    void initControl();
    void initForm();
    void saveImage();
    void closeAll();


public Q_SLOTS:
    void setBorderWidth(int borderWidth);
    void setBgColor(const QColor &bgColor);
};

#endif // GIFWIDGET_H
