#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
//#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <streambuf>
#include <sstream>
#include <stdlib.h>
#include <stdint.h>
using namespace std;

string idArray[1024]; // char array that can hold 1024 chars for 1st column
string publicKeyArray[1014]; // char array that can hold 1024 chars for 2nd column

int main(int argc, char *argv[])
{
    ifstream infile;
    string fileName, returnedPublicKey, requestedID;
    int num = 0; //num has to start at 0

    printf("Enter a file name: ");
    getline(cin, fileName);
    infile.open(fileName.c_str()); //text file
    if(infile.fail()) // checks to see if file opended
    {
      cout << "Error! The text file failed to open!" << endl;
      cout << "Please restart the program and try again!" << endl;
      return 1; // ends the program if the file did not open
    }

    while(!infile.eof()) // reads file to end of *file*, not line
    {
      infile >> idArray[num] >> publicKeyArray[num]; // read each line and store into array
      ++num; // go to the next number
    }
    infile.close();

    printf("Enter server port number: ");
    string portNumberString;
    while(getline(cin, portNumberString) && portNumberString.size() != 4) {
      printf("Please enter a valid port number! \n");
      printf("Enter server port number: ");
    }
    printf("Waiting for requests...\n");

    int portNumber = stoi(portNumberString); //Converts datatype of the port number from string to int

    // Create a socket
    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    // Fill in the address structure containing self address
    struct sockaddr_in myaddr;
    memset(&myaddr, 0, sizeof(struct sockaddr_in));
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(portNumber);        // Port to listen
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind a socket to the address
    int res = bind(listenSocket, (struct sockaddr*) &myaddr, sizeof(myaddr));
    if (res < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    // Set the "LINGER" timeout to zero, to close the listen socket
    // immediately at program termination.
    struct linger linger_opt = { 1, 0 }; // Linger active, timeout 0
    setsockopt(listenSocket, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt));

    // Now, listen for a connection
    res = listen(listenSocket, 1);    // "1" is the maximal length of the queue
    if (res < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    // Accept a connection (the "accept" command waits for a connection with
    // no timeout limit...)
    struct sockaddr_in peeraddr;
    socklen_t peeraddr_len;
    int acceptedSocket = accept(listenSocket, (struct sockaddr*) &peeraddr, &peeraddr_len);
    if (acceptedSocket < 0)
    {
        cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    // A connection is accepted. The new socket "s1" is created
    // for data input/output. The peeraddr structure is filled in with
    // the address of connected entity, print it.

    res = close(listenSocket);    // Closes the listen socket

    char buffer[4096]; //Creates a buffer with enough space for the data we want to accept
    res = read(acceptedSocket, buffer, 4095);
    if (res < 0) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }
    buffer[res] = 0;

    if(find(idArray, idArray + 5, buffer) == idArray + 5)
    {
      returnedPublicKey = "";
    }
    else {
      int index = distance(idArray, find(idArray, idArray + (sizeof(idArray)/sizeof(*idArray)), buffer));
      returnedPublicKey = publicKeyArray[index];
    }

    const char *returnedPublicKeyChar = returnedPublicKey.c_str();
    char buffer2[4096];
    strcpy(buffer2, returnedPublicKeyChar);
    send(acceptedSocket, buffer2, strlen(buffer2), 0);
    cout << "User " << buffer << " requested their public key" << endl;
    cout << "Public key was sent to client, the program will now terminate" << endl;

    close(acceptedSocket);
    return 0;
}
