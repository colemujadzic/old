#include <iostream>
#include <sstream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <fstream>
#include <vector>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <streambuf>
#include <stdint.h>
using namespace std;

int main(int argc, char *argv[])
{
    string hostNameString, portNumberString, userName;
    printf("Enter a server host name: ");
    while(getline(cin, hostNameString) && (hostNameString.compare("127.0.0.1") != 0 && hostNameString.compare("localhost") != 0)) {
      printf("That entry is NOT valid \n");
      printf("Enter a server host name: ");
    }
    printf("Enter server port number: ");
    while(getline(cin, portNumberString) && portNumberString.size() != 4) {
      printf("Please enter a valid port number! \n");
      printf("Enter server port number: ");
    }
    int portNumber = stoi(portNumberString); //Converts datatype of the port number from string to int

    // Create socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    // Fill in server IP address
    struct sockaddr_in server;
    int serverAddrLen;
    bzero( &server, sizeof( server ) );

    const char* hostName = hostNameString.c_str();
    if (argc > 1)
        hostName = argv[1];

    // Resolve server address (convert from symbolic name to IP number)
    struct hostent *host = gethostbyname(hostName);
    if (host == NULL)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    server.sin_family = AF_INET;
    if (argc >= 3)
        portNumber = (short) atoi(argv[2]);
    server.sin_port = htons(portNumber);

    // Write resolved IP address of a server to the address structure
    memmove(&(server.sin_addr.s_addr), host->h_addr_list[0], 4);

    // Connect to the remote server
    int res = connect(clientSocket, (struct sockaddr*) &server, sizeof(server));
    if (res < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    std::cout << "Connected. Reading a server message" << std::endl;

    printf("Enter a user name: ");
    while(getline(cin, userName) && userName.compare("Terminate") == 0) {
      printf("The process will now terminate \n");
      return 0;
    }
    const char *userNameChar = userName.c_str();
    char buffer[4096];
    strcpy(buffer, userNameChar);
    send(clientSocket, buffer, strlen(buffer), 0);

    char buffer2[4096];
    res = read(clientSocket, buffer2, 4095);
    if (res < 0) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }
    buffer2[res] = 0;

    cout << "The public key for user " << userName << " is " << buffer2 << endl;

    close(clientSocket);
    return 0;
}
