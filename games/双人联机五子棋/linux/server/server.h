#ifndef __SERVER_H__
#define __SERVER_H__
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unordered_map>
#include <string>
#define BLACK 0
#define WHITE 1
#define NONE 2
#define BOARDWIDTH 15
using namespace std;


struct Pos{
	int x, y;
	Pos(){}
	Pos(int _x, int _y):x(_x), y(_y){}
};

struct Message{
    const char* msg;
    int color;
    Pos pos;
    Message(const char* _msg,int c, int x, int y):msg(_msg), color(c){
        pos = Pos(x, y);
    }
    Message(){}
};

typedef struct sockaddr_in SocketAddr;

class Server
{
public:
    Server(uint16_t _port);
    ~Server();
    void Start();
private:
    uint16_t port;
    int serverSocket;
    SocketAddr serverAddr;
    string getClientID(SocketAddr* addr);
    void MakePacket(const char* type, Message* data, char* packet);
    bool ParsePacket(char* packet, Message* data);
    unordered_map<string, SocketAddr> clients;
    unordered_map<string, int> clientColor;
    int board[BOARDWIDTH][BOARDWIDTH];
    int direction;
    int d[8][2] = {{0, -1}, {-1, -1}, {-1, 0}, {-1, 1},
                    {0, 1}, {1, 1}, {1, 0}, {1, -1}};
    int dfs(int x, int y, int color, int dir);
    int MaxConnection(int x, int y, int color);
};

#endif