#include "Sdl2widget.h"
#include <QDebug>

SDLLib::SDLLib()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
        qDebug("Could not initialize SDL - %s\n", SDL_GetError());
    }
}

SDLLib::~SDLLib()
{
    SDL_Quit();
}

SDLLib SDL2Widget::s_sdl;

SDL2Widget::SDL2Widget(QWidget *parent) : QWidget(parent),
        m_pScreen(NULL),
        m_pRenderer(NULL),
        m_pTexture(NULL),
        m_xbk(0),
        m_ybk(0)
{

//    this->setAttribute(Qt::WA_PaintOnScreen);
    this->setAttribute(Qt::WA_OpaquePaintEvent);
//    this->setAttribute(Qt::WA_TranslucentBackground);

    m_pScreen = SDL_CreateWindowFrom((void *)this->winId());
    if(m_pScreen == NULL) {
        qDebug("Could not create window - %s\n", SDL_GetError());
    }
}

SDL2Widget::~SDL2Widget()
{
    if(m_pTexture != NULL){
        SDL_DestroyTexture(m_pTexture);
    }
    if(m_pRenderer != NULL){
        SDL_DestroyRenderer(m_pRenderer);
    }
    if(m_pScreen != NULL){
        SDL_DestroyWindow(m_pScreen);
    }
}

void SDL2Widget::showEvent(QShowEvent *)
{
    this->showSDL();
}

void SDL2Widget::hideEvent(QHideEvent *)
{
    this->hideSDL();
}

void SDL2Widget::showSDL()
{
    SDL_ShowWindow(m_pScreen);
}

void SDL2Widget::hideSDL()
{
    SDL_HideWindow(m_pScreen);
}

bool SDL2Widget::initSDLRenderer(int width, int height)
{
#if true  //二次打开是否重建渲染？
    if(m_pTexture != NULL){
        SDL_DestroyTexture(m_pTexture);
    }
    if(m_pRenderer != NULL){
        SDL_DestroyRenderer(m_pRenderer);
    }

    m_pRenderer = SDL_CreateRenderer(m_pScreen, -1, SDL_RENDERER_ACCELERATED);
    if(m_pRenderer == NULL) {
        qDebug("Could not create renderer - %s\n", SDL_GetError());
        return false;
    }

    m_pTexture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, width, height);
    if(m_pTexture == NULL) {
        qDebug("Could not create texture - %s\n", SDL_GetError());
        return false;
    }
#else
    if(m_pRenderer == NULL){
        m_pRenderer = SDL_CreateRenderer(m_pScreen, -1, SDL_RENDERER_ACCELERATED);
        if(m_pRenderer == NULL) {
            qDebug("Could not create renderer - %s\n", SDL_GetError());
            return false;
        }
    }
    if(m_pTexture == NULL){
        m_pTexture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, width, height);
        if(m_pTexture == NULL) {
            qDebug("Could not create texture - %s\n", SDL_GetError());
            return false;
        }
    }
#endif

    m_orgRect.x = 0;
    m_orgRect.y = 0;
    m_orgRect.w = width;
    m_orgRect.h = height;
    m_showRect = m_orgRect;

    return true;
}

void SDL2Widget::updateTexture(const uint8_t *Yplane, int Ypitch,
                               const uint8_t *Uplane, int Upitch,
                               const uint8_t *Vplane, int Vpitch)
{
    if(m_xbk != this->x() || m_ybk != this->y() ||
       m_showRect.w != this->width() || m_showRect.h != this->height()){

        m_xbk = this->x();
        m_ybk = this->y();
        m_showRect.w = this->width();
        m_showRect.h = this->height();

        SDL_SetWindowPosition(m_pScreen,m_xbk,m_ybk);
        SDL_SetWindowSize(m_pScreen,m_showRect.w,m_showRect.h);
    }
    SDL_UpdateYUVTexture(m_pTexture, &m_orgRect,
                         Yplane, Ypitch,
                         Uplane, Upitch,
                         Vplane, Vpitch);
    SDL_RenderClear(m_pRenderer);
    SDL_RenderCopy(m_pRenderer, m_pTexture, &m_orgRect, &m_showRect);
    SDL_RenderPresent(m_pRenderer);
    //SDL_Delay(10);
}
