#ifndef __LABEL_H__
#define __LABEL_H__
#include <string>
#include <SDL2/SDL.h>
#include "DataTypes.h"
#include "Window.h"
#include "Widget.h"
using namespace std;


class Label:public Widget
{
public:
    Label(Window* window);
    Label(Window* window, const char* text, int _x, int _y, int _w, int _h);
    ~Label();
};

#endif