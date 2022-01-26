#include "Window.h"

Window::Window(int _width, int _height):width(_width), height(_height), window(nullptr), renderer(nullptr),
boardTexture(nullptr), startTexture(nullptr), whiteChessTex(nullptr), blackChessTex(nullptr)
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("gobang", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (!window)
		ErrorHandler("Create window failed.");
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
		ErrorHandler("Create renderer failed.");
	boardTexture = LoadImage("chessboard.png");
	startTexture = LoadImage("start.png");
	whiteChessTex = LoadImage("white.png");
	blackChessTex = LoadImage("black.png");
}

void Window::Start()
{
	while(true)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					return;
			}
		}
	}
}

void Window::Update()
{
	for(Widget* w: child)
	{
		w->show();
	}
	SDL_RenderPresent(renderer);
}

void Window::HandleEvent(SDL_Event* e)
{
	for(Widget* w: child)
	{
		w->handleEvent(e);
	}
}

SDL_Texture* Window::LoadImage(string path)
{
	SDL_Surface* surface = IMG_Load(path.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if(!texture)
	{
		printf("Can not open image %s\n", path.c_str());
		exit(1);
	}
	return texture;
}

Window::~Window()
{
	if(boardTexture) SDL_DestroyTexture(boardTexture);
	if(startTexture) SDL_DestroyTexture(startTexture);
	if(whiteChessTex) SDL_DestroyTexture(whiteChessTex);
	if(blackChessTex) SDL_DestroyTexture(blackChessTex);
	for(Widget* w: child)
		delete w;
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void ErrorHandler(const char* message)
{
	printf("%s error: %s\n", message, SDL_GetError());
	exit(1);
}