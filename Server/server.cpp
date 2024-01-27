#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <vector>
#include <algorithm>
#define PORT 8080
using namespace std;

enum Network
{
    SOCKET,
    BIND,
    LISTEN,
    ACCEPT
};
const string networkString[] = {
    "SOCKET",
    "BIND",
    "LISTEN",
    "ACCEPT"};
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

void InteractionWithClient(int clientSocket, vector<int> &clients)
{
    cout << "Client Connected" << endl;
    char buffer[1024];
    while (1)
    {
        int bytesRecieved = recv(clientSocket, buffer, sizeof(buffer), 0);
        string messsage(buffer, bytesRecieved);
        cout << "Message Recieved: " << messsage << endl;

        if (bytesRecieved <= 0)
        {
            if (bytesRecieved < 0)
                perror("Error receiving message");

            cout << "Client Disconnected" << endl;
            break;
        }

        for (auto client : clients)
        {
            if (client != clientSocket)
            {
                send(client, messsage.c_str(), messsage.length(), 0);
            }
        }
    }

    auto itr = find(clients.begin(), clients.end(), clientSocket);
    if (itr != clients.end())
    {
        clients.erase(itr);
    }
    close(clientSocket);
}
int main()
{
    vector<int> clients;
    int ret = 0;
    sockaddr_in serverAddress;

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    check(serverSocket, SOCKET);

    serverAddress.sin_family = AF_INET;         // Ipv4
    serverAddress.sin_port = htons(PORT);       // Port number
    serverAddress.sin_addr.s_addr = INADDR_ANY; // IP Address

    ret = bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    check(ret, BIND);

    ret = listen(serverSocket, SOMAXCONN);
    check(ret, LISTEN);

    cout << "================Server Starting on this port: " << PORT << "========================" << endl;

    while (1)
    {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        check(clientSocket, ACCEPT);
        clients.push_back(clientSocket);

        thread t1(InteractionWithClient, clientSocket, ref(clients));
        t1.detach();
    }

    // close(serverSocket);

    return 0;
}