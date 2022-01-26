#include "Widget.h"

Widget::Widget(int w, int h): display(true), Enabled(true), width(w), height(h)
{
    bg = {190, 190, 190, 255};
}

void Widget::setGeometry(int _w, int _h)
{
    width = _w;
    height = _h;
}

void Widget::move(int _x, int _y)
{
    WidgetArea.x = _x;
    WidgetArea.y = _y;
}

void Widget::hide(bool hidden)
{
    display = !hidden;
}

void Widget::show()
{
    if(!display)
        return;
    if(bg.a)
    {
        SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
        SDL_RenderFillRect(renderer, &WidgetArea);
    }
    SDL_Rect textRect = {WidgetArea.x, WidgetArea.y,
                        width, height};
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);
}

void Widget::setText(const char* text)
{
    Text = text;
    TTF_Font* font = TTF_OpenFont(FONT, 100);
    SDL_Color fontColor = {0, 0, 0, 0xFF};
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, Text, fontColor);
    if(!textSurface)
    {
        printf("Can not open font %s\n", FONT);
        exit(1);
    }
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
}

void Widget::setBackgroundColor(SDL_Color color)
{
    bg = color;
}

void Widget::setEnabled(bool isEnabled)
{
    Enabled = isEnabled;
}

Widget::~Widget()
{
    if(texture) 
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}