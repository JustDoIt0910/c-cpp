#ifndef __DATATYPES_H__
#define __DATATYPES_H__

enum Status{
	STATUS_START,
	STATUS_PLAYING,
	STATUS_WIN,
	STATUS_LOSE,
	QUIT
};

enum ChessColor{
	BLACK,
	WHITE,
	NONE
};

struct Pos{
	int x, y;
	Pos(){}
	Pos(int _x, int _y):x(_x), y(_y){}
};

struct Chess{
	ChessColor color;
	Pos pos;
};

#endif