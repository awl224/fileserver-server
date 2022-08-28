#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <hashmap.hpp>

int main(int argc, char **argv)
{

    // flag for hostname
    bool hostFlag = false;
    string hostString;
    char *hostChars;
    string portNoString;
    int portNo;
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][2] == 'h' && argv[i][3] == 'o' && argv[i][4] == 's' && argv[i][5] == 't' && argv[i][6] == 'n' && argv[i][7] == 'a' && argv[i][8] == 'm' && argv[i][9] == 'e')
        {
            hostFlag = true;
            i++;
            int z = 0;
            while (argv[i][z] != ':')
            {
                hostString += argv[i][z];
                z++;
            }
            z++;
            while (argv[i][z] != NULL)
            {
                portNoString += argv[i][z];
                z++;
            }
        }
        else
        {
            printf("ERROR: Unknown Flag\n");
            return 1;
        }
    }
    if (hostString.empty())
    {
        printf("No hostname or port number provided, will listen on: 'localhost:8081'.\n");
        hostString = "localhost";
        portNoString = "8081";
    }
    hostChars = (char *)hostString.c_str();
    portNo = stoi(portNoString);
    // UNIX SOCKET AND PORT
    // create socket
    int sockfd, newsockfd, clilen;
    int n;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("ERROR opening socket\n");
        exit(1);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portNo);
    // bind to socket
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding\n");
        exit(1);
    }
    printf("Server listening on: %s:%s\n", hostChars, portNoString.c_str());
    // listen for connection
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);

    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        exit(1);
    }
    printf("-------------------------------------------------\n");
    printf("New connection: %s:%d\n", hostChars, newsockfd);

    // loop connection
    // empty hash map
    HashMap map = HashMap();
    bool requestFlag = false;
    bool sendFlag = false;
    std::vector<unsigned char> encryptedMessage;
    char buffer[1000];
    while (true)
    {
        // initial message
        // read message into buffer
        int loopBytes = 0;
        ssize_t readBytes = 1000;
        std::vector<unsigned char> encryptedMessage;
        bzero(buffer, 1001);
        while (readBytes == 1000)
        {
            readBytes = recv(newsockfd, buffer, sizeof(buffer), 0);
            if (readBytes > 0)
            {
                loopBytes += readBytes;
                for (int i = 0; i < readBytes; i++)
                {
                    encryptedMessage.push_back(buffer[i]);
                }
                bzero(buffer, sizeof(buffer));
            }
        }
        if (loopBytes > 0)
        {
            printf("Reading %d bytes.\n", encryptedMessage.size());
            // decrypt message
            std::vector<unsigned char> decryptedMessage = pack109::encrypt(encryptedMessage);
            // deserialize into file or request
            if (decryptedMessage[3] == 0x04)
            { //**FILE CASE** - store in hash map, key = filename, value = File struct
                File receivedFile = pack109::deserialize_file(decryptedMessage);
                printf("Received File: \"%s\".\n", receivedFile.name.c_str());
                bool contained = map.insert(receivedFile.name, receivedFile);
                if (contained)
                { // replacing file case
                    printf("File replaced.\n");
                }
                else
                { // new file case
                    printf("New File sored.\n");
                }
            }
            else if (decryptedMessage[3] == 0x07)
            { //**REQUEST CASE** - search for file, if found - serialize to file and encrypt to send, if not found - do nothing
                Request receivedRequest = pack109::deserialize_request(decryptedMessage);
                printf("Received Request: \"%s\".\n", receivedRequest.name.c_str());
                // search for file
                if (map.contains(receivedRequest.name))
                { // Server has the file in the hash map, now send to client    
                    File toSendFile = map.get(receivedRequest.name);
                    std::vector<unsigned char> serializedSendFile = pack109::serialize(toSendFile);
                    std::vector<unsigned char> encryptedSendFile = pack109::encrypt(serializedSendFile);
                    unsigned char sendBuffer[encryptedSendFile.size()];
                    bzero(sendBuffer, encryptedSendFile.size()+1);
                    for (int i = 0; i < encryptedSendFile.size(); i++)
                    {
                        sendBuffer[i] = encryptedSendFile[i];
                    }
                    printf("Sending \"%s\" -- %d bytes.\n", toSendFile.name.c_str(), encryptedSendFile.size());
                    n = write(newsockfd, sendBuffer, encryptedSendFile.size());
                    if (n < 0)
                    {
                        perror("ERROR writing to socket\n");
                        printf("Done.\n");
                        exit(1);
                    }
                    printf("Message sent.\n");
                }
                else
                {
                    // no file found in hash map.
                    //printf("map.contains(%s) is: %d\n",receivedRequest.name.c_str(),map.contains(receivedRequest.name));
                    printf("File not found: \"%s\".\n", receivedRequest.name.c_str());
                }
            }
            else
            {
                printf("neither file nor request\n");
            }
        }
        // continue loop
    }
    return 0;
}