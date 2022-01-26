#include "Widget.h"

Widget::Widget(int w, int h): display(true), Enabled(true), width(w), height(h)
{
    imgTexture = nullptr;
    bg = {190, 190, 190, 255};
    TTF_Init();
    font = TTF_OpenFont(FONT, 50);
    if (!font)
    {
        printf("Can not open font %s\n", FONT);
        exit(1);
    }
    fontColor = { 0, 0, 0, 0xFF };
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
    SDL_Rect textRect = { WidgetArea.x, WidgetArea.y,
                        width, height };
    if(bg.a)
    {
        if (!imgTexture)
        {
            SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
            SDL_RenderFillRect(renderer, &WidgetArea);
        }
        else
            SDL_RenderCopy(renderer, imgTexture, nullptr, &textRect);
    }
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);
}

void Widget::setText(string text)
{
    Text = text;
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, Text.c_str(), fontColor);
    if(!textSurface)
    {
        printf("Can not open font %s\n", FONT);
        exit(1);
    }
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    textSurface = nullptr;
}

void Widget::setBackgroundColor(SDL_Color color)
{
    bg = color;
}

void Widget::setBackgroundImage(string path)
{
    imgTexture = LoadImage(path);
}

SDL_Texture* Widget::LoadImage(string path)
{
    SDL_Surface* s = IMG_Load(path.c_str());
    SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
    SDL_FreeSurface(s);
    if (!t)
    {
        printf("Can not open image %s\n", path.c_str());
        exit(1);
    }
    return t;
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
    if (imgTexture)
    {
        SDL_DestroyTexture(imgTexture);
        imgTexture = nullptr;
    }
}