#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include "Widget.h"
using namespace std;

class Window
{
public:
	int width;
	int height;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* boardTexture;
	SDL_Texture* startTexture;
	SDL_Texture* whiteChessTex;
	SDL_Texture* blackChessTex;
	SDL_Event event;
	vector<Widget*> child;
	Window(int _width, int _height);
	~Window();
	virtual void Start();
	virtual void Update();
	virtual void HandleEvent(SDL_Event* e);
	SDL_Texture* LoadImage(string path);
};

void ErrorHandler(const char* message);

#endif
