#include "Label.h"
#include <SDL_ttf.h>


Label::Label(Window* window):Widget(60, 30)
{
    window->child.push_back(this);
    renderer = window->renderer;
    WidgetArea.x = 0;
    WidgetArea.y = 0;
    WidgetArea.w = width;
    WidgetArea.h = height;
    Text = "label";
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    SDL_Color fontColor = {0, 0, 0, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, Text.c_str(), fontColor);
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    textSurface = nullptr;
}

Label::Label(Window* window, const char* text, int _x, int _y, int _w, int _h):Widget(_w, _h)
{
    window->child.push_back(this);
    renderer = window->renderer;
    WidgetArea.x = _x;
    WidgetArea.y = _y;
    WidgetArea.w = width;
    WidgetArea.h = height;
    Text = text;
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    SDL_Color fontColor = {0, 0, 0, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, Text.c_str(), fontColor);
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    textSurface = nullptr;
}

Label::~Label()
{

}