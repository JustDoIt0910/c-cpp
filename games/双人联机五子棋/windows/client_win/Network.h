#ifndef __NETWORK_H__
#define __NETWORK_H__
#include "DataTypes.h"
#include <iostream>
#include <functional>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#define BUFSIZE 60
using namespace std;

struct Message{
    char msg[BUFSIZE];
    ChessColor color;
    Pos pos;
    Message(const char* _msg, ChessColor c, int x, int y): color(c){
        pos = Pos(x, y);
        strcpy(msg, _msg);
    }
    Message(){}
};

class Socket{
public:
    SOCKET _socket;
    bool readyRead;
    Message recvMessage;
    Socket(const char* remoteIP, int port);
    void Send(Message* message);
    void Send(const char* message);
    void Recv(function<void(void)> callback);
    void MakePacket(const char* type, Message* data, char* packet);
    bool Unpack(char* packet);
    ~Socket();
private:
    HANDLE recvtid;
    WSADATA wsaData;
    SOCKADDR_IN RemoteAddress;
};

struct Args{
    Socket* instance;
    function<void(void)> callback;
};
unsigned WINAPI RecvThread(void*);

#endif