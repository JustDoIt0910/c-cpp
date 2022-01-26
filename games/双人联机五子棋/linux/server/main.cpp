#include "server.h"

int main(int argc, char* argv[])
{
    Server server(9000);
    server.Start();
    return 0;
}