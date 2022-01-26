#include "Network.h"
#include "string.h"
#include <regex>

Socket::Socket(const char* remoteIP, int port):readyRead(false)
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        printf("WSAStartup() falied\n");
    _socket = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&RemoteAddress, 0, sizeof(RemoteAddress));
    RemoteAddress.sin_family = AF_INET;
    RemoteAddress.sin_addr.s_addr = inet_addr(remoteIP);
    RemoteAddress.sin_port = htons(port);
}

void Socket::Send(Message* message)
{
    char packet[BUFSIZE];
    MakePacket("data_packet", message, packet);
    //printf("Send packet: %s\n", packet);
    sendto(_socket, packet, strlen(packet), 0, (SOCKADDR*)&RemoteAddress, sizeof(RemoteAddress));
}

void Socket::Send(const char* message)
{
    char packet[BUFSIZE];
    Message data(message, NONE, -1, -1);
    MakePacket("connect_packet", &data, packet);
    sendto(_socket, packet, strlen(packet), 0, (SOCKADDR*)&RemoteAddress, sizeof(RemoteAddress));
}

void Socket::Recv(function<void(void)> callback)
{
    Args *args = new Args;
    args->instance = this;
    args->callback = callback;
    unsigned threadID;
    recvtid = (HANDLE)_beginthreadex(NULL, 0, RecvThread, (void*)args, 0, &threadID);
}

void Socket::MakePacket(const char* type, Message* data, char* packet)
{
    const char* color = data->color == WHITE ? "white" : "black";
    sprintf(packet, "type=%s##msg=%s##%s##pos(%d,%d)", type, data->msg, color, data->pos.x, data->pos.y);
}

bool Socket::Unpack(char* packet)
{
    regex pattern("type=([a-z_]+)##msg=([a-z]*)##([a-z]+)##pos\\((-*\\d+),(-*\\d+)\\)");
    smatch result;
    string str(packet);
    if(regex_match(str, result, pattern))
    {
        if(result[1] == "data_packet")
        {
            strcpy(recvMessage.msg, "");
            recvMessage.color = result[3] == "white" ? WHITE: BLACK;
            recvMessage.pos = Pos(stoi(string(result[4]), 0, 10), stoi(string(result[5]), 0, 10));
        }
        else
        {
            strcpy(recvMessage.msg, result.str(2).c_str());
            recvMessage.color = result[3] == "white" ? WHITE: BLACK;
            recvMessage.pos = Pos(-1, -1);
        }
        readyRead = true;
        return true;
    }
    readyRead = false;
    return false;
}

unsigned WINAPI RecvThread(void* _args)
{
    Args *args = (Args*)(_args);
    SOCKADDR_IN from_addr;
    int addr_size = sizeof(from_addr);
    while(true)
    {
        char buf[BUFSIZE];
        memset(buf, 0, sizeof(buf)); 
        recvfrom(args->instance->_socket, buf, sizeof(buf), 0, (SOCKADDR*)&from_addr, &addr_size); 
        if(!args->instance->Unpack(buf))
            continue;
        args->callback();
        args->instance->readyRead = false;
    } 
    delete args;
    return 0;
}

Socket::~Socket()
{
    closesocket(_socket);
    WSACleanup();
}