#ifndef __WIDGET_H__
#define __WIDGET_H__
#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
using namespace std;
#define FONT  "/usr/share/fonts/truetype/freefont/FreeMono.ttf"

class Widget
{
public:
    const char* Text;
    SDL_Color bg;
    int width;
    int height;
    Widget(int w, int h);
    ~Widget();
    void show();
    void hide(bool hidden);
    void move(int _x, int _y);
    void setText(const char* text);
    void setGeometry(int _w, int _h);
    void setBackgroundColor(SDL_Color color);
    void setEnabled(bool isEnabled);
    virtual void setEventListener(function<void(SDL_Event* e)> callback){};
    virtual void handleEvent(SDL_Event* e){};
protected:
    bool Enabled;
    bool display;
    SDL_Rect WidgetArea;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    function<void(SDL_Event* e)> callback;
};

#endif