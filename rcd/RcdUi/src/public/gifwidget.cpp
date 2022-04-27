
#include "gifwidget.h"
#include "qmutex.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#include "qlayout.h"
#include "qpainter.h"
#include "qevent.h"
#include "qstyle.h"
#include "qpixmap.h"
#include "qtimer.h"
#include "qdatetime.h"
#include "qapplication.h"
#include "qdesktopwidget.h"
#include "qdesktopservices.h"
#include "qfiledialog.h"
#include "qurl.h"
#include "qdebug.h"
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include "qscreen.h"
#endif
//智能指针 回收界面实例指针
QScopedPointer<GifWidget> GifWidget::P_self;
GifWidget *GifWidget::Instance()
{
    if (P_self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (P_self.isNull()) {
            P_self.reset(new GifWidget(""));
        }
    }

    return P_self.data();
}

GifWidget::GifWidget(QString screenMode,QWidget *parent) :
    QDialog(parent) ,
    _screenMode(screenMode)
{
    this->initControl();
    this->initForm();
}

bool GifWidget::eventFilter(QObject *watched, QEvent *event)
{
    static QPoint mousePoint;
    static bool mousePressed = false;

    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (mouseEvent->type() == QEvent::MouseButtonPress) {
        if (mouseEvent->button() == Qt::LeftButton) {
            mousePressed = true;
            mousePoint = mouseEvent->globalPos() - this->pos();
            return true;
        }
    } else if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        mousePressed = false;
        return true;
    } else if (mouseEvent->type() == QEvent::MouseMove) {
        if (mousePressed) {
            this->move(mouseEvent->globalPos() - mousePoint);
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}
void GifWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(_bgColor);
    painter.drawRoundedRect(this->rect(), 5, 5);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillRect(_rectGif, Qt::SolidPattern);
}
int GifWidget::getBorderWidth() const
{
    return this->_borderWidth;
}

QColor GifWidget::getBgColor() const
{
    return this->_bgColor;
}

void GifWidget::initControl()
{
    this->setObjectName("GifWidget");
    this->resize(800, 600);
    QScreen *screen=QGuiApplication::primaryScreen ();
    QRect mm=screen->availableGeometry() ;
    setWidthHeight(mm.width(),mm.height());
    this->setSizeGripEnabled(true);
    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    verticalLayout->setObjectName("verticalLayout");
    verticalLayout->setContentsMargins(0, 0, 0, 0);

    _widgetMain = new QWidget(this);
    _widgetMain->setObjectName("widgetMain");
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(_widgetMain->sizePolicy().hasHeightForWidth());
    _widgetMain->setSizePolicy(sizePolicy1);
    verticalLayout->addWidget(_widgetMain);

}

void GifWidget::initForm()
{
    _borderWidth = 0;
    //_bgColor = QColor(34, 163, 169);
    _bgColor = QColor(0, 0, 0);

    _fps = 10;
    P_gifWriter = 0;

    _timer = new QTimer(this);
    _timer->setInterval(1);
    connect(_timer, SIGNAL(timeout()), this, SLOT(saveImage()));

    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->installEventFilter(this);

    QStringList qss;
    qss.append("QLabel{color:#ffffff;}");
    qss.append("#btnClose,#btnIcon{border:none;border-radius:0px;}");
    qss.append("#btnClose:hover{background-color:#ff0000;}");
    qss.append("#btnClose{border-top-right-radius:5px;}");
    qss.append("#labTitle{font:bold 16px;}");
    qss.append("#labStatus{font:15px;}");
    this->setStyleSheet(qss.join(""));
}

void GifWidget::saveImage()
{
    if (!P_gifWriter) {
        return;
    }

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    //由于qt4没有RGBA8888,采用最接近RGBA8888的是ARGB32,颜色会有点偏差
    QPixmap pix = QPixmap::grabWindow(0, x() + rectGif.x(), y() + rectGif.y(), rectGif.width(), rectGif.height());
    QImage image = pix.toImage().convertToFormat(QImage::Format_ARGB32);
#else
    QScreen *screen = QApplication::primaryScreen();
    QPixmap pix = screen->grabWindow(0, x() + _rectGif.x(), y() + _rectGif.y(), _rectGif.width(), _rectGif.height());
    //pix.save(QApplication::applicationDirPath()+"/filegif/"+QString("%1.png").arg(QDateTime::currentDateTime().toString()));
    QImage image = pix.toImage().convertToFormat(QImage::Format_RGBA8888);
    //image.save(QApplication::applicationDirPath()+"/filegif/"+QString("%1.png").arg(QDateTime::currentDateTime().toString()));
    //image = image.scaled(image.width(),image.height(),Qt::IgnoreAspectRatio);
#endif
    P_gif.GifWriteFrame(P_gifWriter, image.bits(), _rectGif.width(), _rectGif.height()-30, _fps);
    _count++;
    // _labStatus->setText(QString("正在录制 第 %1 帧").arg(_count));
}

void GifWidget::slotRecord(int openflag)
{
    Record_mode flag = static_cast<Record_mode>(openflag);
    switch (flag) {
    case Record_mode::record_start:
    {
        if (0 != P_gifWriter) {
            delete P_gifWriter;
            P_gifWriter = 0;
        }

        //先弹出文件保存对话框
        _fileName = qApp->applicationDirPath() + "/filegif/" ;
        if(!createFile(_fileName)){
            return;
        }
        _fileName =_fileName +_screenMode+QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss.gif");
        /*fileName = QFileDialog::getSaveFileName(this, "选择保存位置", qApp->applicationDirPath() + "/", "gif图片(*.gif)");
        if (fileName.isEmpty()) {
            return;
        }*/
        _fps = 10;
        P_gifWriter = new Gif::GifWriter;
        bool bOk = P_gif.GifBegin(P_gifWriter, _fileName.toLocal8Bit().data(), _width, _height, _fps);
        if (!bOk) {
            delete P_gifWriter;
            P_gifWriter = 0;
            return;
        }

        _count = 0;
        //延时启动
        _timer->setInterval(1000 / _fps);
        QTimer::singleShot(1000, _timer, SLOT(start()));
    }
        break;
    case Record_mode::record_stop:
    {
        _timer->stop();
        P_gif.GifEnd(P_gifWriter);

        delete P_gifWriter;
        P_gifWriter = 0;
        //QDesktopServices::openUrl(QUrl(_fileName));
    }
        break;
    default:
        break;
    }

}
//关闭
void GifWidget::closeAll()
{
    if (0 != P_gifWriter) {
        delete P_gifWriter;
        P_gifWriter = 0;
    }

    this->close();
}
//设置线宽
void GifWidget::setBorderWidth(int borderWidth)
{
    if (this->_borderWidth != borderWidth) {
        this->_borderWidth = borderWidth;
        this->update();
    }
}
//设置背景色
void GifWidget::setBgColor(const QColor &bgColor)
{
    if (this->_bgColor != bgColor) {
        this->_bgColor = bgColor;
        this->update();
    }
}
//录屏宽高
void GifWidget::setWidthHeight(int width, int height)
{
    _width = width;
    _height = height;
}
//创建文件夹
bool GifWidget::createFile(const QString& filename)
{
    QDir *folder = new QDir;
    bool exist = folder->exists(filename);
    if(exist)
    {
        return true;
    }
    else
    {
        return folder->mkpath(filename);
    }
}
void GifWidget::setGifRect()
{
    //截屏区域
    _rectGif =  QRect(0, _widgetMain->height(), _width - (_borderWidth * 2), _height);
    /*QPainter painter;
    painter.setPen(Qt::NoPen);
    painter.setBrush(_bgColor);
    painter.drawRoundedRect(_rectGif, 5, 5);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.fillRect(_rectGif, Qt::SolidPattern);*/

}
