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

int main(int argc, char **argv)
{
    int client_socket;
    sockaddr_in server_addr;
    char buff[BUFF_SIZE + 5];
    //string text_buff;
    char text_buff[BUFF_SIZE + 5];
    char name[20];
    int port;
    char server_address[30];

    cout << "input server address and port" << endl;
    cin >> server_address >> port;
    cout << "input your name: ";
    cin >> name;
    fgetc(stdin);

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        cout << "Can't create socket" << endl;
        return -1;
    }

    // Connect to server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port); //4000?
    server_addr.sin_addr.s_addr = inet_addr(server_address);
    if (connect(client_socket, (sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        cout << "Connection fail!" << endl;
        return -2;
    }

    User user;
    user.set_name();
    while (1)
    {
        user.set_msg();
        if(strcmp(user.get_msg(),"exit")==0) break;
        write(client_socket, user, sizeof(User) + 1); // transmit including NULL which is +1
        read(client_socket, buff, BUFF_SIZE);
        cout << buff << endl;
    }
    close(client_socket);

    return 0;
}