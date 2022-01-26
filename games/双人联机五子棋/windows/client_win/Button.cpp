#include "Button.h"
#include <SDL_ttf.h>

Button::Button(Window* window):Widget(60, 30)
{
    window->child.push_back(this);
    renderer = window->renderer;
    WidgetArea.x = 0;
    WidgetArea.y = 0;
    WidgetArea.w = width;
    WidgetArea.h = height;
    Text = "button";
    callback = nullptr;
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    SDL_Color fontColor = {0, 0, 0, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, Text.c_str(), fontColor);
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    textSurface = nullptr;
}

Button::Button(Window* window, const char* text, int _x, int _y, int _w, int _h):Widget(_w, _h)
{
    window->child.push_back(this);
    renderer = window->renderer;
    WidgetArea.x = _x;
    WidgetArea.y = _y;
    WidgetArea.w = width;
    WidgetArea.h = height;
    Text = text;
    callback = nullptr;
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    SDL_Color fontColor = {0, 0, 0, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, Text.c_str(), fontColor);
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    textSurface = nullptr;
}

void Button::handleEvent(SDL_Event* e)
{
    if(!Enabled || (*e).type != SDL_MOUSEBUTTONUP)
        return;
    int ex = (*e).button.x;
    int ey = (*e).button.y;
    if(ex >= WidgetArea.x && ex <= WidgetArea.x + WidgetArea.w && ey >= WidgetArea.y && ey <= WidgetArea.y + WidgetArea.h)
    {
        if(callback)
            callback(e);
    }   
}

void Button::setEventListener(function<void(SDL_Event* e)> callback)
{
    this->callback = callback;
}

Button::~Button()
{
    
}