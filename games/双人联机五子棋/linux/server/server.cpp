#include "server.h"
#include <regex>
#include <time.h>
#include <stdlib.h>
#define BUFSIZE 60


Server::Server(uint16_t _port): port(_port)
{
    serverSocket = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);
    if(bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        printf("bind() error");
        exit(0);
    }
    for(int i = 0; i < BOARDWIDTH; i++)
        for(int j = 0; j < BOARDWIDTH; j++)
            board[i][j] = NONE;
}

void Server::MakePacket(const char* type, Message* data, char* packet)
{
    const char* color = data->color == WHITE ? "white" : "black";
    sprintf(packet, "type=%s##msg=%s##%s##pos(%d,%d)", type, data->msg, color, data->pos.x, data->pos.y);
}

bool Server::ParsePacket(char* packet, Message* data)
{
    regex pattern("type=([a-z_]+)##msg=([a-z]*)##([a-z]+)##pos\\((-*\\d+),(-*\\d+)\\)");
    smatch result;
    string str(packet);
    if(regex_match(str, result, pattern))
    {
        if(result[1] == "data_packet")
        {
            data->msg = "";
            data->color = result[3] == "white" ? WHITE : BLACK;
            data->pos = Pos(stoi(result[4], 0, 10), stoi(result[5], 0, 10));
        }
        else
        {
            data->msg = result[2].str().c_str();
            data->color = NONE;
            data->pos = Pos(-1, -1);
        }
        return true;
    }
    return false;
}

string Server::getClientID(SocketAddr* addr)
{
    string cid = string(inet_ntoa((*addr).sin_addr)) + ":" + to_string(ntohs((*addr).sin_port));
    return cid;
}

void Server::Start()
{
    char buf[BUFSIZE];
    SocketAddr client_addr;
    socklen_t addr_len = sizeof(client_addr);
    Message data;
    int color = NONE;
    while(true)
    {
        memset(buf, 0, BUFSIZE);
        memset(&client_addr, 0, addr_len);
        recvfrom(serverSocket, buf, BUFSIZE, 0, (struct sockaddr*)&client_addr, &addr_len);
        if(!ParsePacket(buf, &data))
        {
            printf("parse packet failed\n");
            continue;
        }   
        //不是消息数据包
        if(!strlen(data.msg))
        {
            unordered_map<string, SocketAddr>::iterator it;
            string cid = getClientID(&client_addr);
            if(clients.size() == 2 && clients.count(cid) != 0)
            {
                for(it = clients.begin(); it != clients.end(); it++)
                {
                    if(it->first != cid)
                    {
                        printf("%s ---> %s\n", cid.c_str(), it->first.c_str());
                        char packet[BUFSIZE];
                        MakePacket("data_packet", &data, packet);
                        sendto(serverSocket, packet, BUFSIZE, 0, (struct sockaddr*)&it->second, addr_len);
                        if(board[data.pos.y][data.pos.x] == NONE)
                        {
                            board[data.pos.y][data.pos.x] = data.color;
                            //判断有无五子连棋
                            if(MaxConnection(data.pos.x, data.pos.y, data.color) == 5)
                            {
                                memset(packet, 0, BUFSIZE);
                                Message win = Message("win", clientColor[cid], -1, -1);
                                Message lose = Message("lose", clientColor[it->first], -1, -1);
                                MakePacket("connect_packet", &win, packet);
                                sendto(serverSocket, packet, BUFSIZE, 0, (struct sockaddr*)&clients[cid], addr_len);
                                MakePacket("connect_packet", &lose, packet);
                                sendto(serverSocket, packet, BUFSIZE, 0, (struct sockaddr*)&clients[it->first], addr_len);
                            }
                        }
                    }
                }
            }
        }
        //是消息数据包
        else
        {
            if(!strcmp(data.msg, "connect")) //握手包
            {
                string cid = getClientID(&client_addr);
                if(clients.size() < 2 && clients.count(cid) == 0)
                {  
                    srand((unsigned)time(NULL));
                    if(color == NONE)
                    {
                        color = rand() % 2;
                        clientColor[cid] = 1 - color;
                    }  
                    else 
                        clientColor[cid] = color;
                    clients[cid] = client_addr;
                    Message response("connect", clientColor[cid], -1, -1);
                    char packet[BUFSIZE];
                    MakePacket("connect_packet", &response, packet);
                    sendto(serverSocket, packet, BUFSIZE, 0, (struct sockaddr*)&clients[cid], addr_len);
                    printf("%d client(s) connected\n", int(clients.size()));
                }
            }
            else if(!strcmp(data.msg, "disconnect")) //通知离线
            {
                string cid = getClientID(&client_addr);
                unordered_map<string, SocketAddr>::iterator it = clients.find(cid);
                if(it != clients.end())
                {
                    clients.erase(it);
                    color = clients.size() == 0 ? NONE : clientColor[cid];
                    clientColor.erase(cid);
                    printf("client disconnected\n");
                }                  
            }
        }
    }
}

int Server::MaxConnection(int x, int y, int color)
{
    int c1 = dfs(x, y, color, 0) + dfs(x, y, color, 4) - 1;
    int c2 = dfs(x, y, color, 1) + dfs(x, y, color, 5) - 1;
    int c3 = dfs(x, y, color, 2) + dfs(x, y, color, 6) - 1;
    int c4 = dfs(x, y, color, 4) + dfs(x, y, color, 7) - 1;
    return max(max(max(c1, c2), c3), c4);
}

int Server::dfs(int x, int y, int color, int dir)
{
    if(x == BOARDWIDTH || y == BOARDWIDTH || board[y][x] != color)
        return 0;
    return dfs(x + d[dir][0], y + d[dir][1], color, dir) + 1;
}

Server::~Server()
{
    close(serverSocket);
}