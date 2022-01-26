#ifndef __NETWORK_H__
#define __NETWORK_H__
#include "DataTypes.h"
#include <iostream>
#include <functional>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#define BUFSIZE 60
using namespace std;

struct Message{
    const char* msg;
    ChessColor color;
    Pos pos;
    Message(const char* _msg, ChessColor c, int x, int y):msg(_msg), color(c){
        pos = Pos(x, y);
    }
    Message(){}
};

class Socket{
public:
    int _socket;
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
    pthread_t recvtid;
    struct sockaddr_in RemoteAddress;
};

struct Args{
    Socket* instance;
    function<void(void)> callback;
};
void* RecvThread(void*);

#endif