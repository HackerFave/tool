#ifndef SDL2WIDGET_H
#define SDL2WIDGET_H

#include <SDL2/SDL.h>
#include <QWidget>

class SDLLib
{
public:
    explicit SDLLib();
    ~SDLLib();

};

class SDL2Widget : public QWidget
{
    Q_OBJECT
public:
    explicit SDL2Widget(QWidget *parent = nullptr);
    ~SDL2Widget();

    bool initSDLRenderer(int width, int height);  //须在流媒体线程调用
    void updateTexture(const uint8_t *Yplane, int Ypitch,
                       const uint8_t *Uplane, int Upitch,
                       const uint8_t *Vplane, int Vpitch);
    void showSDL();
    void hideSDL();

protected:

    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);


private:

    SDL_Window      *m_pScreen;
    SDL_Renderer    *m_pRenderer;
    SDL_Texture     *m_pTexture;
    SDL_Rect        m_orgRect,m_showRect;
    int             m_xbk,m_ybk;

    static SDLLib  s_sdl;
};

#endif // SDL2WIDGET_H
