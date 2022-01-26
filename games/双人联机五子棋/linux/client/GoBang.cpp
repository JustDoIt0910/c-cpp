#include "GoBang.h"
#include <math.h>


GoBang::GoBang(int _width, int _height):Window(_width, _height), status(STATUS_START),
UDPSocket(nullptr)
{
    for(int x = 0; x < BOARD_WIDTH; x++)
        for(int y = 0; y < BOARD_WIDTH; y++)
        {
            ChessBoard[y][x].color = NONE;
            ChessBoard[y][x].pos = Pos(40 + 37 * x, 40 + 37 * y);
        }        
    UDPSocket = new Socket("47.102.131.241", 9000);
    UDPSocket->Recv([=](){
        if(UDPSocket->readyRead)
        {
            if(!strlen(UDPSocket->recvMessage.msg)) //不是握手数据包
            {
                printf("recv packet: color: %d pos(%d, %d)\n", 
                UDPSocket->recvMessage.color, UDPSocket->recvMessage.pos.x, UDPSocket->recvMessage.pos.y);
                Pos pos = UDPSocket->recvMessage.pos;
                ChessMove(pos.x, pos.y, UDPSocket->recvMessage.color);
                isMyTurn = true;
            }
            else //握手数据包
            {
                //收到服务器回应，进入游戏
                if(!strcmp(UDPSocket->recvMessage.msg, "connect"))
                {
                    btnStart->hide(true);
                    btnStart->setEnabled(false);
                    btnQuit->hide(true);
                    btnQuit->setEnabled(false);
                    status = STATUS_PLAYING;
                    self = UDPSocket->recvMessage.color;
                    isMyTurn = (self == BLACK);
                }
                //win
                else if(!strcmp(UDPSocket->recvMessage.msg, "win"))
                {
                    printf("win\n");
                    sleep(1);
                    winLabel->hide(false);
                    status = STATUS_WIN;
                }
                else if(!strcmp(UDPSocket->recvMessage.msg, "lose"))
                {                    
                    printf("lose\n");
                    sleep(1);
                    loseLabel->hide(false);
                    status = STATUS_LOSE;
                }
            }
        }
    });
    btnStart = new Button(this, "Start", 80, 450, 80, 40);
    btnStart->setBackgroundColor({205, 133, 63, 255});
    btnStart->setEventListener([=](SDL_Event* e){
        cout << "start" << endl;
        sleep(1);
        UDPSocket->Send("connect");
    });

    btnQuit = new Button(this, "Quit", 250, 450, 80, 40);
    btnQuit->setBackgroundColor({205, 133, 63, 255});
    btnQuit->setEventListener([=](SDL_Event* e){
        cout << "quit" << endl;
        status = QUIT;
    });

    winLabel = new Label(this, "You win!", 210, 5, 200, 80);
    winLabel->setBackgroundColor({0, 0, 0, 0});
    loseLabel = new Label(this, "You lose!", 210, 5, 200, 80);
    loseLabel->setBackgroundColor({0, 0, 0, 0});
    winLabel->hide(true);
    loseLabel->hide(true);
}

void GoBang::Update()
{
    if(status == STATUS_PLAYING)
    {
        SDL_RenderCopy(renderer, boardTexture, nullptr, nullptr);
        for(int y = 0; y < BOARD_WIDTH; y++)
            for(int x = 0; x < BOARD_WIDTH; x++)
            {
                if(ChessBoard[y][x].color == BLACK)
                {
                    SDL_Rect r = {ChessBoard[y][x].pos.x - 10, ChessBoard[y][x].pos.y - 10,
                                20, 20};
                    SDL_RenderCopy(renderer, blackChessTex, nullptr, &r);
                }
                else if(ChessBoard[y][x].color == WHITE)
                {
                    SDL_Rect r = {ChessBoard[y][x].pos.x - 10, ChessBoard[y][x].pos.y - 10,
                                20, 20};
                    SDL_RenderCopy(renderer, whiteChessTex, nullptr, &r);
                }

            }
    }
    else if(status == STATUS_START)
    {
        SDL_RenderCopy(renderer, startTexture, nullptr, nullptr);
    }
    Window::Update();
}

void GoBang::ChessMove(int x, int y, ChessColor c)
{
    ChessBoard[y][x].color = c;
}

Pos GoBang::CoordinateScale(Pos pos)
{
    float tempX = (pos.x - 40) / 37.2;
    float tempY = (pos.y - 40) / 37.2;
    int bx = tempX > 0 ? (int)(tempX + 0.5) : 0;
    int by = tempY > 0 ? (int)(tempY + 0.5) : 0;
    //没有点击在棋盘交点
    if(abs((40 + 37 * bx) - pos.x) > 10 || abs((40 + 37 * by) - pos.y) > 10
      || bx > 14 || by > 14)
        bx = by = -1;
    return Pos(bx, by);
}

void GoBang::Start()
{
    while(true)
	{
        if(status == QUIT)
            return;
		while(SDL_PollEvent(&event))
		{
            if(event.type == SDL_QUIT)
                return;
            HandleEvent(&event);   
		}
        Update();
	}
}

void GoBang::HandleEvent(SDL_Event* e)
{
    if((*e).type == SDL_MOUSEBUTTONUP)
        if(status == STATUS_PLAYING && isMyTurn)
        {
            Pos BoardPos = CoordinateScale(Pos(event.button.x, event.button.y));
            if(BoardPos.x != -1 && ChessBoard[BoardPos.y][BoardPos.x].color == NONE)
            {
                ChessMove(BoardPos.x, BoardPos.y, self);
                Message* message = new Message("", self, BoardPos.x, BoardPos.y);
                UDPSocket->Send(message);
                isMyTurn = false;
            }
        }    
    Window::HandleEvent(e);
}

GoBang::~GoBang()
{
    UDPSocket->Send("disconnect");
    delete UDPSocket;
    UDPSocket = nullptr;
}