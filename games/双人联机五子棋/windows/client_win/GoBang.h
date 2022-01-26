#ifndef __GOBANG_H__
#define __GOBANG_H__
#include <SDL.h>
#include "Window.h"
#include "Network.h"
#include "DataTypes.h"
#include "Button.h"
#include "Label.h"

#define BOARD_WIDTH 15


class GoBang:public Window
{
public:
	Status status;
	ChessColor self;
	ChessColor opponent;
	bool isMyTurn;
	Chess ChessBoard[BOARD_WIDTH][BOARD_WIDTH];
	Socket* UDPSocket;
	Button* btnStart;
	Button* btnQuit;
	Label* winLabel;
	Label* loseLabel;
	Label* side;
	Label* blackChess;
	Label* whiteChess;

	void Start();
	void Update();
	void HandleEvent(SDL_Event* e);
	Pos CoordinateScale(Pos pos);
	void ChessMove(int x, int y, ChessColor c);
	GoBang(int _width, int _height);
	~GoBang();
private:
	Label* ColorToLabel[2];
};

#endif
