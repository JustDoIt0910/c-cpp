#ifndef __WIDGET_H__
#define __WIDGET_H__
#include <functional>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
using namespace std;
#define FONT  "C:\\Windows\\Fonts\\Arial.ttf"

class Widget
{
public:
    string Text;
    SDL_Color bg;
    int width;
    int height;
    Widget(int w, int h);
    ~Widget();
    void show();
    void hide(bool hidden);
    void move(int _x, int _y);
    void setText(string text);
    void setGeometry(int _w, int _h);
    void setBackgroundColor(SDL_Color color);
    void setBackgroundImage(string path);
    void setEnabled(bool isEnabled);
    virtual void setEventListener(function<void(SDL_Event* e)> callback){};
    virtual void handleEvent(SDL_Event* e){};
protected:
    bool Enabled;
    bool display;
    SDL_Rect WidgetArea;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    TTF_Font* font;
    SDL_Color fontColor;
    SDL_Texture* imgTexture;
    SDL_Texture* LoadImage(string path);
    function<void(SDL_Event* e)> callback;
};

#endif