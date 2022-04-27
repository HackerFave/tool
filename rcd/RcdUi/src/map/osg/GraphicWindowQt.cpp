

#include <osg/DeleteHandler>
#include "GraphicWindowQt.h"
#include <osgViewer/ViewerBase>
#include <QInputEvent>
#include <QDebug>
#include <QThread>
#include <QOpenGLContext>
#include <QWidget>
#include <QSurface>
#include <QWindow>
#include "QtSimpleMap.h"
#include <OpenThreads/Mutex>
#include <OpenThreads/Condition>
#include <OpenThreads/Thread>
#include <OpenThreads/Barrier>
#include <osg/Stats>

class QtKeyboardMap
{

public:
    QtKeyboardMap()
    {
        mKeyMap[Qt::Key_Escape     ] = osgGA::GUIEventAdapter::KEY_Escape;
        mKeyMap[Qt::Key_Delete   ] = osgGA::GUIEventAdapter::KEY_Delete;
        mKeyMap[Qt::Key_Home       ] = osgGA::GUIEventAdapter::KEY_Home;
        mKeyMap[Qt::Key_Enter      ] = osgGA::GUIEventAdapter::KEY_KP_Enter;
        mKeyMap[Qt::Key_End        ] = osgGA::GUIEventAdapter::KEY_End;
        mKeyMap[Qt::Key_Return     ] = osgGA::GUIEventAdapter::KEY_Return;
        mKeyMap[Qt::Key_PageUp     ] = osgGA::GUIEventAdapter::KEY_Page_Up;
        mKeyMap[Qt::Key_PageDown   ] = osgGA::GUIEventAdapter::KEY_Page_Down;
        mKeyMap[Qt::Key_Left       ] = osgGA::GUIEventAdapter::KEY_Left;
        mKeyMap[Qt::Key_Right      ] = osgGA::GUIEventAdapter::KEY_Right;
        mKeyMap[Qt::Key_Up         ] = osgGA::GUIEventAdapter::KEY_Up;
        mKeyMap[Qt::Key_Down       ] = osgGA::GUIEventAdapter::KEY_Down;
        mKeyMap[Qt::Key_Backspace  ] = osgGA::GUIEventAdapter::KEY_BackSpace;
        mKeyMap[Qt::Key_Tab        ] = osgGA::GUIEventAdapter::KEY_Tab;
        mKeyMap[Qt::Key_Space      ] = osgGA::GUIEventAdapter::KEY_Space;
        mKeyMap[Qt::Key_Delete     ] = osgGA::GUIEventAdapter::KEY_Delete;
        mKeyMap[Qt::Key_Alt      ] = osgGA::GUIEventAdapter::KEY_Alt_L;
        mKeyMap[Qt::Key_Shift    ] = osgGA::GUIEventAdapter::KEY_Shift_L;
        mKeyMap[Qt::Key_Control  ] = osgGA::GUIEventAdapter::KEY_Control_L;
        mKeyMap[Qt::Key_Meta     ] = osgGA::GUIEventAdapter::KEY_Meta_L;

        mKeyMap[Qt::Key_F1             ] = osgGA::GUIEventAdapter::KEY_F1;
        mKeyMap[Qt::Key_F2             ] = osgGA::GUIEventAdapter::KEY_F2;
        mKeyMap[Qt::Key_F3             ] = osgGA::GUIEventAdapter::KEY_F3;
        mKeyMap[Qt::Key_F4             ] = osgGA::GUIEventAdapter::KEY_F4;
        mKeyMap[Qt::Key_F5             ] = osgGA::GUIEventAdapter::KEY_F5;
        mKeyMap[Qt::Key_F6             ] = osgGA::GUIEventAdapter::KEY_F6;
        mKeyMap[Qt::Key_F7             ] = osgGA::GUIEventAdapter::KEY_F7;
        mKeyMap[Qt::Key_F8             ] = osgGA::GUIEventAdapter::KEY_F8;
        mKeyMap[Qt::Key_F9             ] = osgGA::GUIEventAdapter::KEY_F9;
        mKeyMap[Qt::Key_F10            ] = osgGA::GUIEventAdapter::KEY_F10;
        mKeyMap[Qt::Key_F11            ] = osgGA::GUIEventAdapter::KEY_F11;
        mKeyMap[Qt::Key_F12            ] = osgGA::GUIEventAdapter::KEY_F12;
        mKeyMap[Qt::Key_F13            ] = osgGA::GUIEventAdapter::KEY_F13;
        mKeyMap[Qt::Key_F14            ] = osgGA::GUIEventAdapter::KEY_F14;
        mKeyMap[Qt::Key_F15            ] = osgGA::GUIEventAdapter::KEY_F15;
        mKeyMap[Qt::Key_F16            ] = osgGA::GUIEventAdapter::KEY_F16;
        mKeyMap[Qt::Key_F17            ] = osgGA::GUIEventAdapter::KEY_F17;
        mKeyMap[Qt::Key_F18            ] = osgGA::GUIEventAdapter::KEY_F18;
        mKeyMap[Qt::Key_F19            ] = osgGA::GUIEventAdapter::KEY_F19;
        mKeyMap[Qt::Key_F20            ] = osgGA::GUIEventAdapter::KEY_F20;

        mKeyMap[Qt::Key_hyphen         ] = '-';
        mKeyMap[Qt::Key_Equal         ] = '=';

        mKeyMap[Qt::Key_division      ] = osgGA::GUIEventAdapter::KEY_KP_Divide;
        mKeyMap[Qt::Key_multiply      ] = osgGA::GUIEventAdapter::KEY_KP_Multiply;
        mKeyMap[Qt::Key_Minus         ] = '-';
        mKeyMap[Qt::Key_Plus          ] = '+';
        //mKeyMap[Qt::Key_H              ] = osgGA::GUIEventAdapter::KEY_KP_Home;
        //mKeyMap[Qt::Key_                    ] = osgGA::GUIEventAdapter::KEY_KP_Up;
        //mKeyMap[92                    ] = osgGA::GUIEventAdapter::KEY_KP_Page_Up;
        //mKeyMap[86                    ] = osgGA::GUIEventAdapter::KEY_KP_Left;
        //mKeyMap[87                    ] = osgGA::GUIEventAdapter::KEY_KP_Begin;
        //mKeyMap[88                    ] = osgGA::GUIEventAdapter::KEY_KP_Right;
        //mKeyMap[83                    ] = osgGA::GUIEventAdapter::KEY_KP_End;
        //mKeyMap[84                    ] = osgGA::GUIEventAdapter::KEY_KP_Down;
        //mKeyMap[85                    ] = osgGA::GUIEventAdapter::KEY_KP_Page_Down;
        mKeyMap[Qt::Key_Insert        ] = osgGA::GUIEventAdapter::KEY_KP_Insert;
        //mKeyMap[Qt::Key_Delete        ] = osgGA::GUIEventAdapter::KEY_KP_Delete;
    }

    ~QtKeyboardMap()
    {
    }

    int remapKey(QKeyEvent* event)
    {
        KeyMap::iterator itr = mKeyMap.find(event->key());
        if (itr == mKeyMap.end())
        {
            return int(*(event->text().toLatin1().data()));
        }
        else
            return itr->second;
    }

    private:
    typedef std::map<unsigned int, int> KeyMap;
    KeyMap mKeyMap;
};

static QtKeyboardMap s_QtKeyboardMap;


/// The object responsible for the scene re-rendering.
class HeartBeat : public QObject {
public:
int _timerId;
osg::Timer _lastFrameStartTime;
osg::observer_ptr< osgViewer::ViewerBase > _viewer;

HeartBeat();
virtual ~HeartBeat();
void init( osgViewer::ViewerBase *viewer );
void stopTimer();
void timerEvent( QTimerEvent *event );
};

static HeartBeat heartBeat;

GLWidget::GLWidget( QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags f, bool forwardKeyEvents )
: QGLWidget(parent, shareWidget, f),
_gw( NULL ),
_forwardKeyEvents( forwardKeyEvents )
{
}

GLWidget::GLWidget( QGLContext* context, QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags f,
                    bool forwardKeyEvents )
: QGLWidget(context, parent, shareWidget, f),
_gw( NULL ),
_forwardKeyEvents( forwardKeyEvents )
{
}

GLWidget::GLWidget( const QGLFormat& format, QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags f,
                    bool forwardKeyEvents )
: QGLWidget(format, parent, shareWidget, f),
_gw( NULL ),
_forwardKeyEvents( forwardKeyEvents )
{
}

GLWidget::~GLWidget()
{
    // close MyGraphicWindowQt and remove the reference to us
    if( _gw )
    {
		//delete _gw;
        _gw->close();
        _gw->m_pGLWidget = NULL;
        _gw = NULL;
    }
}

void GLWidget::processDeferredEvents()
{
    QQueue<QEvent::Type> deferredEventQueueCopy;
    {
        QMutexLocker lock(&_deferredEventQueueMutex);
        deferredEventQueueCopy = _deferredEventQueue;
        _eventCompressor.clear();
        _deferredEventQueue.clear();
    }

    while (!deferredEventQueueCopy.isEmpty())
    {
        QEvent event(deferredEventQueueCopy.dequeue());
        QGLWidget::event(&event);
    }
}

bool GLWidget::event( QEvent* event )
{

    // QEvent::Hide
    //
    // workaround "Qt-workaround" that does glFinish before hiding the widget
    // (the Qt workaround was seen at least in Qt 4.6.3 and 4.7.0)
    //
    // Qt makes the context current, performs glFinish, and releases the context.
    // This makes the problem in OSG multithreaded environment as the context
    // is active in another thread, thus it can not be made current for the purpose
    // of glFinish in this thread.

    // QEvent::ParentChange
    //
    // Reparenting GLWidget may create a new underlying window and a new GL context.
    // Qt will then call doneCurrent on the GL context about to be deleted. The thread
    // where old GL context was current has no longer current context to render to and
    // we cannot make new GL context current in this thread.

    // We workaround above problems by deferring execution of problematic event requests.
    // These events has to be enqueue and executed later in a main GUI thread (GUI operations
    // outside the main thread are not allowed) just before makeCurrent is called from the
    // right thread. The good place for doing that is right after swap in a swapBuffersImplementation.

    if (event->type() == QEvent::Hide)
    {
        // enqueue only the last of QEvent::Hide and QEvent::Show
        enqueueDeferredEvent(QEvent::Hide, QEvent::Show);
        return true;
    }
    else if (event->type() == QEvent::Show)
    {
        // enqueue only the last of QEvent::Show or QEvent::Hide
        enqueueDeferredEvent(QEvent::Show, QEvent::Hide);
        return true;
    }
    else if (event->type() == QEvent::ParentChange)
    {
        // enqueue only the last QEvent::ParentChange
        enqueueDeferredEvent(QEvent::ParentChange);
        return true;
    }

    // perform regular event handling
    return QGLWidget::event( event );
}

void GLWidget::setKeyboardModifiers( QInputEvent* event )
{
    int modkey = event->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier);
    unsigned int mask = 0;
    if ( modkey & Qt::ShiftModifier ) mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
    if ( modkey & Qt::ControlModifier ) mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
    if ( modkey & Qt::AltModifier ) mask |= osgGA::GUIEventAdapter::MODKEY_ALT;
    _gw->getEventQueue()->getCurrentEventState()->setModKeyMask( mask );
}

void GLWidget::resizeEvent( QResizeEvent* event )
{
    const QSize& size = event->size();
	//todo后续需要判断这里到底对不对;
	//QPoint pos = ((QWidget*)this->parent())->pos();
    
    _gw->getEventQueue()->windowResize(x(), y(), size.width(), size.height() );
	_gw->resized(x(), y(), size.width(), size.height());
    _gw->requestRedraw();
}

void GLWidget::moveEvent( QMoveEvent* event )
{
    const QPoint& pos = event->pos();
    _gw->resized( pos.x(), pos.y(), width(), height() );
    _gw->getEventQueue()->windowResize( pos.x(), pos.y(), width(), height() );
}

void GLWidget::glDraw()
{
    _gw->requestRedraw();
}

void GLWidget::keyPressEvent( QKeyEvent* event )
{
	return inherited::keyPressEvent(event);
    setKeyboardModifiers( event );
    int value = s_QtKeyboardMap.remapKey( event );
    _gw->getEventQueue()->keyPress( value );

    // this passes the event to the regular Qt key event processing,
    // among others, it closes popup windows on ESC and forwards the event to the parent widgets
    if( _forwardKeyEvents )
        inherited::keyPressEvent( event );
}

void GLWidget::keyReleaseEvent( QKeyEvent* event )
{
	return inherited::keyReleaseEvent(event);
    if( event->isAutoRepeat() )
    {
        event->ignore();
    }
    else
    {
        setKeyboardModifiers( event );
        int value = s_QtKeyboardMap.remapKey( event );
        _gw->getEventQueue()->keyRelease( value );
    }

    // this passes the event to the regular Qt key event processing,
    // among others, it closes popup windows on ESC and forwards the event to the parent widgets
    if( _forwardKeyEvents )
        inherited::keyReleaseEvent( event );
}

void GLWidget::mousePressEvent( QMouseEvent* event )
{
    int button = 0;
    switch ( event->button() )
    {
        case Qt::LeftButton: button = 1; break;
        case Qt::MidButton: button = 2; break;
        case Qt::RightButton: button = 3; break;
        case Qt::NoButton: button = 0; break;
        default: button = 0; break;
    }
    setKeyboardModifiers( event );
    _gw->getEventQueue()->mouseButtonPress( event->x(), event->y(), button );
}

void GLWidget::mouseReleaseEvent( QMouseEvent* event )
{
    int button = 0;
    switch ( event->button() )
    {
        case Qt::LeftButton: button = 1; break;
        case Qt::MidButton: button = 2; break;
        case Qt::RightButton: button = 3; break;
        case Qt::NoButton: button = 0; break;
        default: button = 0; break;
    }
    setKeyboardModifiers( event );
    _gw->getEventQueue()->mouseButtonRelease( event->x(), event->y(), button );
}

void GLWidget::mouseDoubleClickEvent( QMouseEvent* event )
{
    int button = 0;
    switch ( event->button() )
    {
        case Qt::LeftButton: button = 1; break;
        case Qt::MidButton: button = 2; break;
        case Qt::RightButton: button = 3; break;
        case Qt::NoButton: button = 0; break;
        default: button = 0; break;
    }
    setKeyboardModifiers( event );
    _gw->getEventQueue()->mouseDoubleButtonPress( event->x(), event->y(), button );
}

void GLWidget::mouseMoveEvent( QMouseEvent* event )
{
    setKeyboardModifiers( event );
    _gw->getEventQueue()->mouseMotion( event->x(), event->y() );
}

void GLWidget::wheelEvent( QWheelEvent* event )
{
    setKeyboardModifiers( event );
    _gw->getEventQueue()->mouseScroll(
        event->orientation() == Qt::Vertical ?
            (event->delta()>0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN) :
            (event->delta()>0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT) );
}



GraphicWindowQt::GraphicWindowQt( osg::GraphicsContext::Traits* traits, QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags f )
:   m_bRealized(false)
{

    m_pGLWidget = NULL;
    _traits = traits;
    init( parent, shareWidget, f );
}

GraphicWindowQt::GraphicWindowQt( GLWidget* widget )
:   m_bRealized(false)
{
    m_pGLWidget = widget;
    _traits = m_pGLWidget ? createTraits( m_pGLWidget ) : new osg::GraphicsContext::Traits;
    init( NULL, NULL, 0 );
}

GraphicWindowQt::~GraphicWindowQt()
{
    close();

    // remove reference from GLWidget
    if ( m_pGLWidget )
        m_pGLWidget->_gw = NULL;
}

bool GraphicWindowQt::init( QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags f )
{
    // update _widget and parent by WindowData
    WindowData* windowData = _traits.get() ? dynamic_cast<WindowData*>(_traits->inheritedWindowData.get()) : 0;
    if ( !m_pGLWidget )
        m_pGLWidget = windowData ? windowData->_widget : NULL;
    if ( !parent )
        parent = windowData ? windowData->_parent : NULL;

    // create widget if it does not exist
    m_bOwnWidget = m_pGLWidget == NULL;
    if ( !m_pGLWidget )
    {
        // shareWidget
        if ( !shareWidget ) {
            GraphicWindowQt* sharedContextQt = dynamic_cast<GraphicWindowQt*>(_traits->sharedContext.get());
            if ( sharedContextQt )
                shareWidget = sharedContextQt->getGLWidget();
        }

        // WindowFlags
        Qt::WindowFlags flags = f | Qt::Window | Qt::CustomizeWindowHint;
        if ( _traits->windowDecoration )
            flags |= Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowSystemMenuHint
#if (QT_VERSION_CHECK(4, 5, 0) <= QT_VERSION)
                | Qt::WindowCloseButtonHint
#endif
                ;

        // create widget
        m_pGLWidget = new GLWidget( traits2qglFormat( _traits.get() ), parent, shareWidget, flags );
        //m_pQTWidget=new QWidget;
    }

    // set widget name and position
    // (do not set it when we inherited the widget)
    if ( m_bOwnWidget )
    {
        m_pGLWidget->setWindowTitle( _traits->windowName.c_str() );
        m_pGLWidget->move( _traits->x, _traits->y );
        if ( !_traits->supportsResize ) m_pGLWidget->setFixedSize( _traits->width, _traits->height );
        else m_pGLWidget->resize( _traits->width, _traits->height );
    }

    // initialize widget properties
    m_pGLWidget->setAutoBufferSwap( false );
    m_pGLWidget->setMouseTracking( true );
    m_pGLWidget->setFocusPolicy( Qt::WheelFocus );
    m_pGLWidget->setGraphicsWindow( this );
    m_pGLWidget->update();

    useCursor( _traits->useCursor );

    // initialize State
    setState( new osg::State );
    getState()->setGraphicsContext(this);

    // initialize contextID
    if ( _traits.valid() && _traits->sharedContext.valid() )
    {
        getState()->setContextID( _traits->sharedContext->getState()->getContextID() );
        incrementContextIDUsageCount( getState()->getContextID() );
    }
    else
    {
        getState()->setContextID( osg::GraphicsContext::createNewContextID() );
    }

    // make sure the event queue has the correct window rectangle size and input range
    getEventQueue()->syncWindowRectangleWithGraphicsContext();

    return true;
}

QGLFormat GraphicWindowQt::traits2qglFormat( const osg::GraphicsContext::Traits* traits )
{
    QGLFormat format( QGLFormat::defaultFormat() );

    format.setAlphaBufferSize( traits->alpha );
    format.setRedBufferSize( traits->red );
    format.setGreenBufferSize( traits->green );
    format.setBlueBufferSize( traits->blue );
    format.setDepthBufferSize( traits->depth );
    format.setStencilBufferSize( traits->stencil );
    format.setSampleBuffers( traits->sampleBuffers );
    format.setSamples( traits->samples );

    format.setAlpha( traits->alpha>0 );
    format.setDepth( traits->depth>0 );
    format.setStencil( traits->stencil>0 );
    format.setDoubleBuffer( traits->doubleBuffer );
    format.setSwapInterval( traits->vsync ? 1 : 0 );
    format.setStereo( traits->quadBufferStereo ? 1 : 0 );

    return format;
}

void GraphicWindowQt::qglFormat2traits( const QGLFormat& format, osg::GraphicsContext::Traits* traits )
{
    traits->red = format.redBufferSize();
    traits->green = format.greenBufferSize();
    traits->blue = format.blueBufferSize();
    traits->alpha = format.alpha() ? format.alphaBufferSize() : 0;
    traits->depth = format.depth() ? format.depthBufferSize() : 0;
    traits->stencil = format.stencil() ? format.stencilBufferSize() : 0;

    traits->sampleBuffers = format.sampleBuffers() ? 1 : 0;
    traits->samples = format.samples();

    traits->quadBufferStereo = format.stereo();
    traits->doubleBuffer = format.doubleBuffer();

    traits->vsync = format.swapInterval() >= 1;
}

osg::GraphicsContext::Traits* GraphicWindowQt::createTraits( const QGLWidget* widget )
{
    osg::GraphicsContext::Traits *traits = new osg::GraphicsContext::Traits;

    qglFormat2traits( widget->format(), traits );

    QRect r = widget->geometry();
    traits->x = r.x();
    traits->y = r.y();
    traits->width = r.width();
    traits->height = r.height();

    traits->windowName = widget->windowTitle().toLocal8Bit().data();
    Qt::WindowFlags f = widget->windowFlags();
    traits->windowDecoration = ( f & Qt::WindowTitleHint ) &&
                            ( f & Qt::WindowMinMaxButtonsHint ) &&
                            ( f & Qt::WindowSystemMenuHint );
    QSizePolicy sp = widget->sizePolicy();
    traits->supportsResize = sp.horizontalPolicy() != QSizePolicy::Fixed ||
                            sp.verticalPolicy() != QSizePolicy::Fixed;

    return traits;
}

bool GraphicWindowQt::setWindowRectangleImplementation( int x, int y, int width, int height )
{
    if ( m_pGLWidget == NULL )
        return false;

    m_pGLWidget->setGeometry( x, y, width, height );
    return true;
}

void GraphicWindowQt::getWindowRectangle( int& x, int& y, int& width, int& height )
{
    if ( m_pGLWidget )
    {
        const QRect& geom = m_pGLWidget->geometry();
        x = geom.x();
        y = geom.y();
        width = geom.width();
        height = geom.height();
    }
}

bool GraphicWindowQt::setWindowDecorationImplementation( bool windowDecoration )
{
    Qt::WindowFlags flags = Qt::Window|Qt::CustomizeWindowHint;//|Qt::WindowStaysOnTopHint;
    if ( windowDecoration )
        flags |= Qt::WindowTitleHint|Qt::WindowMinMaxButtonsHint|Qt::WindowSystemMenuHint;
    _traits->windowDecoration = windowDecoration;

    if ( m_pGLWidget )
    {
        m_pGLWidget->setWindowFlags( flags );

        return true;
    }

    return false;
}

bool GraphicWindowQt::getWindowDecoration() const
{
    return _traits->windowDecoration;
}

void GraphicWindowQt::grabFocus()
{
    if ( m_pGLWidget )
        m_pGLWidget->setFocus( Qt::ActiveWindowFocusReason );
}

void GraphicWindowQt::grabFocusIfPointerInWindow()
{
    if ( m_pGLWidget->underMouse() )
        m_pGLWidget->setFocus( Qt::ActiveWindowFocusReason );
}

void GraphicWindowQt::raiseWindow()
{
    if ( m_pGLWidget )
        m_pGLWidget->raise();
}

void GraphicWindowQt::setWindowName( const std::string& name )
{
    if ( m_pGLWidget )
        m_pGLWidget->setWindowTitle( name.c_str() );
}

std::string GraphicWindowQt::getWindowName()
{
    return m_pGLWidget ? m_pGLWidget->windowTitle().toStdString() : "";
}

void GraphicWindowQt::useCursor( bool cursorOn )
{
    if ( m_pGLWidget )
    {
        _traits->useCursor = cursorOn;
        if ( !cursorOn ) m_pGLWidget->setCursor( Qt::BlankCursor );
        else m_pGLWidget->setCursor( m_currentCursor );
    }
}

void GraphicWindowQt::setCursor( MouseCursor cursor )
{
    if ( cursor==InheritCursor && m_pGLWidget )
    {
        m_pGLWidget->unsetCursor();
    }

    switch ( cursor )
    {
    case NoCursor: m_currentCursor = Qt::BlankCursor; break;
    case RightArrowCursor: case LeftArrowCursor: m_currentCursor = Qt::ArrowCursor; break;
    case InfoCursor: m_currentCursor = Qt::SizeAllCursor; break;
    case DestroyCursor: m_currentCursor = Qt::ForbiddenCursor; break;
    case HelpCursor: m_currentCursor = Qt::WhatsThisCursor; break;
    case CycleCursor: m_currentCursor = Qt::ForbiddenCursor; break;
    case SprayCursor: m_currentCursor = Qt::SizeAllCursor; break;
    case WaitCursor: m_currentCursor = Qt::WaitCursor; break;
    case TextCursor: m_currentCursor = Qt::IBeamCursor; break;
    case CrosshairCursor: m_currentCursor = Qt::CrossCursor; break;
    case HandCursor: m_currentCursor = Qt::OpenHandCursor; break;
    case UpDownCursor: m_currentCursor = Qt::SizeVerCursor; break;
    case LeftRightCursor: m_currentCursor = Qt::SizeHorCursor; break;
    case TopSideCursor: case BottomSideCursor: m_currentCursor = Qt::UpArrowCursor; break;
    case LeftSideCursor: case RightSideCursor: m_currentCursor = Qt::SizeHorCursor; break;
    case TopLeftCorner: m_currentCursor = Qt::SizeBDiagCursor; break;
    case TopRightCorner: m_currentCursor = Qt::SizeFDiagCursor; break;
    case BottomRightCorner: m_currentCursor = Qt::SizeBDiagCursor; break;
    case BottomLeftCorner: m_currentCursor = Qt::SizeFDiagCursor; break;
    default: break;
    };
    if ( m_pGLWidget ) m_pGLWidget->setCursor( m_currentCursor );
}

bool GraphicWindowQt::valid() const
{
    return m_pGLWidget && m_pGLWidget->isValid();
}

bool GraphicWindowQt::realizeImplementation()
{
    // save the current context
    // note: this will save only Qt-based contexts
    const QGLContext *savedContext = QGLContext::currentContext();

    // initialize GL context for the widget
    if ( !valid() )
        m_pGLWidget->glInit();

    // make current
    m_bRealized = true;
    bool result = makeCurrent();
    m_bRealized = false;

    // fail if we do not have current context
    if ( !result )
    {
        if ( savedContext )
            const_cast< QGLContext* >( savedContext )->makeCurrent();

        OSG_WARN << "Window realize: Can make context current." << std::endl;
        return false;
    }

    m_bRealized = true;

    // make sure the event queue has the correct window rectangle size and input range
	getEventQueue()->syncWindowRectangleWithGraphicsContext();


    // make this window's context not current
    // note: this must be done as we will probably make the context current from another thread
    //       and it is not allowed to have one context current in two threads
    if( !releaseContext() )
        OSG_WARN << "Window realize: Can not release context." << std::endl;

    // restore previous context
    if ( savedContext )
        const_cast< QGLContext* >( savedContext )->makeCurrent();

    return true;
}

bool GraphicWindowQt::isRealizedImplementation() const
{
    return m_bRealized;
}

void GraphicWindowQt::closeImplementation()
{
    if ( m_pGLWidget )
        m_pGLWidget->close();
    m_bRealized = false;
}

void GraphicWindowQt::runOperations()
{
    // While in graphics thread this is last chance to do something useful before
    // graphics thread will execute its operations.
    if (m_pGLWidget->getNumDeferredEvents() > 0)
        m_pGLWidget->processDeferredEvents();

    if (QGLContext::currentContext() != m_pGLWidget->context())
        m_pGLWidget->makeCurrent();

    GraphicsWindow::runOperations();
}

bool GraphicWindowQt::makeCurrentImplementation()
{
    if (m_pGLWidget->getNumDeferredEvents() > 0)
        m_pGLWidget->processDeferredEvents();

    QOpenGLContext* qglcx = m_pGLWidget->context()->contextHandle();
    if (!qglcx)
    {
        return false;
    }
    if (qglcx->thread() != QThread::currentThread()){
        if(!qglcx->thread()) return true;//窗口关闭时
        //这是在另一个线程中做得，需要让主线程来movetothread，需要用信号槽机制告诉主线程
        m_pMoveThread=new MoveThread;
        connect(m_pMoveThread,SIGNAL(moveThread(QThread*)),this,SLOT(onMoveOpenglContextToNewThread(QThread*)),Qt::BlockingQueuedConnection);
        emit m_pMoveThread->moveThread(QThread::currentThread());
        qglcx->makeCurrent(m_pGLWidget->windowHandle());
    }else{
        m_pGLWidget->makeCurrent();
    }
    return true;
}

bool GraphicWindowQt::releaseContextImplementation()
{
    m_pGLWidget->doneCurrent();
    return true;
}

void GraphicWindowQt::swapBuffersImplementation()
{
    //每次切换时需要确认makecurrent
    QOpenGLContext* qglcx = m_pGLWidget->context()->contextHandle();
    qglcx->makeCurrent(m_pGLWidget->windowHandle());

    m_pGLWidget->swapBuffers();

    // FIXME: the processDeferredEvents should really be executed in a GUI (main) thread context but
    // I couln't find any reliable way to do this. For now, lets hope non of *GUI thread only operations* will
    // be executed in a QGLWidget::event handler. On the other hand, calling GUI only operations in the
    // QGLWidget event handler is an indication of a Qt bug.
    if (m_pGLWidget->getNumDeferredEvents() > 0)
        m_pGLWidget->processDeferredEvents();

    // We need to call makeCurrent here to restore our previously current context
    // which may be changed by the processDeferredEvents function.
    if (QGLContext::currentContext() != m_pGLWidget->context())
        m_pGLWidget->makeCurrent();
}

void GraphicWindowQt::requestWarpPointer( float x, float y )
{
    if ( m_pGLWidget )
        QCursor::setPos( m_pGLWidget->mapToGlobal(QPoint((int)x,(int)y)) );
}

void GraphicWindowQt::setMainWindow(QtSimpleMap *mainWindow)
{
    m_pSceneWidget=mainWindow;
}

void GraphicWindowQt::onMoveOpenglContextToNewThread(QThread *newThread)
{
    QOpenGLContext* qglcx = m_pGLWidget->context()->contextHandle();
    if (qglcx->thread() != newThread){
        qglcx->doneCurrent();
        qglcx->moveToThread(newThread);
    }
    m_pSceneWidget->onStartTimer();
}




class QtWindowingSystem : public osg::GraphicsContext::WindowingSystemInterface
{
public:

    QtWindowingSystem()
    {
        OSG_INFO << "QtWindowingSystemInterface()" << std::endl;
    }

    ~QtWindowingSystem()
    {
        if (osg::Referenced::getDeleteHandler())
        {
            osg::Referenced::getDeleteHandler()->setNumFramesToRetainObjects(0);
            osg::Referenced::getDeleteHandler()->flushAll();
        }
    }

    // Access the Qt windowing system through this singleton class.
    static QtWindowingSystem* getInterface()
    {
        static QtWindowingSystem* qtInterface = new QtWindowingSystem;
        return qtInterface;
    }

    // Return the number of screens present in the system
    virtual unsigned int getNumScreens( const osg::GraphicsContext::ScreenIdentifier& /*si*/ )
    {
        OSG_WARN << "osgQt: getNumScreens() not implemented yet." << std::endl;
        return 0;
    }

    // Return the resolution of specified screen
    // (0,0) is returned if screen is unknown
    virtual void getScreenSettings( const osg::GraphicsContext::ScreenIdentifier& /*si*/, osg::GraphicsContext::ScreenSettings & /*resolution*/ )
    {
        OSG_WARN << "osgQt: getScreenSettings() not implemented yet." << std::endl;
    }

    // Set the resolution for given screen
    virtual bool setScreenSettings( const osg::GraphicsContext::ScreenIdentifier& /*si*/, const osg::GraphicsContext::ScreenSettings & /*resolution*/ )
    {
        OSG_WARN << "osgQt: setScreenSettings() not implemented yet." << std::endl;
        return false;
    }

    // Enumerates available resolutions
    virtual void enumerateScreenSettings( const osg::GraphicsContext::ScreenIdentifier& /*screenIdentifier*/, osg::GraphicsContext::ScreenSettingsList & /*resolution*/ )
    {
        OSG_WARN << "osgQt: enumerateScreenSettings() not implemented yet." << std::endl;
    }

    // Create a graphics context with given traits
    virtual osg::GraphicsContext* createGraphicsContext( osg::GraphicsContext::Traits* traits )
    {
        if (traits->pbuffer)
        {
            OSG_WARN << "osgQt: createGraphicsContext - pbuffer not implemented yet." << std::endl;
            return NULL;
        }
        else
        {
            osg::ref_ptr< GraphicWindowQt > window = new GraphicWindowQt( traits );
            if (window->valid()) return window.release();
            else return NULL;
        }
    }

private:

    // No implementation for these
    QtWindowingSystem( const QtWindowingSystem& );
    QtWindowingSystem& operator=( const QtWindowingSystem& );
};


// declare C entry point for static compilation.
extern "C" void graphicswindow_Qt(void)
{
    osg::GraphicsContext::getWindowingSystemInterfaces()->addWindowingSystemInterface(QtWindowingSystem::getInterface());
}


void initQtWindowingSystem()
{
    graphicswindow_Qt();
}



void setViewer( osgViewer::ViewerBase *viewer )
{
    heartBeat.init( viewer );
}


/// Constructor. Must be called from main thread.
HeartBeat::HeartBeat() : _timerId( 0 )
{
}


/// Destructor. Must be called from main thread.
HeartBeat::~HeartBeat()
{
    stopTimer();
}


void HeartBeat::stopTimer()
{
    if ( _timerId != 0 )
    {
        killTimer( _timerId );
        _timerId = 0;
    }
}


/// Initializes the loop for viewer. Must be called from main thread.
void HeartBeat::init( osgViewer::ViewerBase *viewer )
{
    if( _viewer == viewer )
        return;

    stopTimer();

    _viewer = viewer;

    if( viewer )
    {
        _timerId = startTimer( 0 );
        _lastFrameStartTime.setStartTick( 0 );
    }
}


void HeartBeat::timerEvent( QTimerEvent */*event*/ )
{
    osg::ref_ptr< osgViewer::ViewerBase > viewer;
    if( !_viewer.lock( viewer ) )
    {
        // viewer has been deleted -> stop timer
        stopTimer();
        return;
    }

    // limit the frame rate
    if( viewer->getRunMaxFrameRate() > 0.0)
    {
        double dt = _lastFrameStartTime.time_s();
        double minFrameTime = 1.0 / viewer->getRunMaxFrameRate();
        if (dt < minFrameTime)
            OpenThreads::Thread::microSleep(static_cast<unsigned int>(1000000.0*(minFrameTime-dt)));
    }
    else
    {
        // avoid excessive CPU loading when no frame is required in ON_DEMAND mode
        if( viewer->getRunFrameScheme() == osgViewer::ViewerBase::ON_DEMAND )
        {
            double dt = _lastFrameStartTime.time_s();
            if (dt < 0.01)
                OpenThreads::Thread::microSleep(static_cast<unsigned int>(1000000.0*(0.01-dt)));
        }

        // record start frame time
        _lastFrameStartTime.setStartTick();

        // make frame
        if( viewer->getRunFrameScheme() == osgViewer::ViewerBase::ON_DEMAND )
        {
            if( viewer->checkNeedToDoFrame() )
            {
                viewer->frame();
            }
        }
        else
        {
            viewer->frame();
        }
    }
}
