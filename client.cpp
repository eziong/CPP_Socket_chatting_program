#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
using namespace std;

#define BUFF_SIZE 1024

int main(int argc, char **argv){
    int client_socket;
    sockaddr_in server_addr;
    char buff[BUFF_SIZE+5];

    // Create socket
    client_socket = socket(AF_INET,SOCK_STREAM,0);
    if(client_socket == -1){
        cout << "Can't create socket" << endl;
        return -1;
    }

    // Connect to server
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        cout << "Connection fail!" << endl;
        return -2;
    }
    write(client_socket, argv[1],strlen(argv[1])+1); // transmit including NULL which is +1
    read(client_socket,buff,BUFF_SIZE);
    cout << buff << endl;
    close(client_socket);

    return 0;
}