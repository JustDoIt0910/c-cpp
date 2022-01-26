#ifndef __BUTTON_H__
#define __BUTTON_H__
#include <string>
#include <SDL.h>
#include <functional>
#include "DataTypes.h"
#include "Window.h"
#include "Widget.h"
using namespace std;


class Button:public Widget
{
public:
    Button(Window* window);
    Button(Window* window, const char* text, int _x, int _y, int _w, int _h);
    void handleEvent(SDL_Event *e);
    void setEventListener(function<void(SDL_Event* e)> callback);
    ~Button();
};

#endif