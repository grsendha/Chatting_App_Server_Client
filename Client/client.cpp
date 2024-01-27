#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#define PORT 8080
using namespace std;

enum Network
{
    SOCKET,
    SEND,
    LISTEN,
    ACCEPT,
    CONNECT
};
const string networkString[] = {
    "SOCKET",
    "SEND",
    "LISTEN",
    "ACCEPT",
    "CONNECT",
};
void check(int checker, Network network)
{
    if (checker != -1)
    {
        cout << networkString[network] << " Created Successfully" << endl;
    }
    else
    {
        cout << networkString[network] << " Failed" << endl;
        exit(EXIT_FAILURE);
    }
}

void sendMessageToServer(int clientSideSocket)
{
    cout << "Enter your name: ";
    string name;
    getline(cin, name);
    string message;
    while (1)
    {
        cout << "Enter message: ";
        getline(cin, message);
        string msg = name + " : " + message;
        int byteSend = send(clientSideSocket, msg.c_str(), msg.length(), 0);
        if (byteSend == -1)
        {
            cout << "Error sending message" << endl;
            break;
        }
    }
    close(clientSideSocket);
}

void recieveMessageFromServer(int clientSideSocket)
{
    char buffer[4096];
    int recieveLength;
    string msg = "";
    while (1)
    {
        recieveLength = recv(clientSideSocket, buffer, sizeof(buffer), 0);
        if (recieveLength <= 0)
        {
            cout << "Disconnected from the server" << endl;
            break;
        }
        msg = string(buffer, recieveLength);
        cout << endl
             << msg << endl;
    }
    close(clientSideSocket);
}
int main()
{
    int ret = 0;
    sockaddr_in serverAddress;
    int clientSideSocket = socket(AF_INET, SOCK_STREAM, 0);
    check(clientSideSocket, SOCKET);

    serverAddress.sin_family = AF_INET;   // Ipv4
    serverAddress.sin_port = htons(PORT); // Port number
    inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr));

    ret = connect(clientSideSocket, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(sockaddr_in));
    if (ret == -1)
    {
        perror("Error connecting to the server");
        close(clientSideSocket);
        return 1;
    }
    cout << "----------------Server Connected-------------------" << endl;

    thread sendThread(sendMessageToServer, clientSideSocket);
    thread recieveThread(recieveMessageFromServer, clientSideSocket);

    sendThread.join();
    recieveThread.join();
}