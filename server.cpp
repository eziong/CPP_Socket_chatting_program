#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
using namespace std;

#define BUFF_SIZE 1024

class User{
    char name[20];
    char msg[BUFF_SIZE+5];
    public:
    void set_name(){
        cout << "input your name: ";
        scanf("%s",name);
    }
    char* get_name(){
        return name;
    }
    void set_msg(){
        cout << name << "'s message: ";
        fgets(msg, BUFF_SIZE + 5, stdin);
        msg[strlen(msg) - 1] = '\0';
    }
    char* get_msg(){
        return msg;
    }
};

int main()
{

    sockaddr_in server_addr;
    sockaddr_in client_addr;

    char buff_rcv[BUFF_SIZE + 5];
    char buff_snd[BUFF_SIZE + 5];
    User rcv_user;
    User snd_user;

    int server_socket;
    int client_socket;
    socklen_t client_addr_size;

    //Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        cout << "Can't create server socket!" << endl;
        return -1;
    }

    //Bind the socket to IP/port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4000);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //inet_pton(AF_INET,INADDR_ANY,&server_addr.sin_addr);
    if (bind(server_socket, (sockaddr *)&server_addr, sizeof(server_addr)))
    {
        cout << "Can't bind to IP/port!" << endl;
        return -2;
    }

    //Mark the socket for listening in
    if (listen(server_socket, 5))
    { //can listen 5 people at the same time
        cout << "Can't listen!" << endl;
        return -3;
    }

    //Accept a call
    while (1)
    {
        int clients = system("netstat -na|grep :4000");
        client_addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (sockaddr *)&client_addr, &client_addr_size);
        if (client_socket == -1)
        {
            cout << "Client connection fail" << endl;
            return -4;
        }
        read(client_socket, rcv_user, sizeof(User));
        cout << rcv_user.get_name << ": " << rcv_user.get_msg() << endl;
        sprintf(buff_snd, "got the message\n");
        write(client_socket, buff_snd, strlen(buff_snd) + 1); // including NULL which is +1
    }
    close(client_socket);
}