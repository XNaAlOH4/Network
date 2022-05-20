// #include "winsock.h"

// #include <windows.h>
// #include <stdlib.h>
// #include <process.h>

// #define MAX_THREADS 1

// int main(int argc, char** argv);   // Main Thread: 0
// void ServerFunc(void);             // Server input, Thread: 1
// void ShutDown(void);               // Program shutdown

// #define MAX_ITER 50

// int main(int argc, char** argv) {
//     if(argc > 1) {
//         InitWSA();
//         // SOCKET listenSock = SetupServer();
//         struct addrinfo * ptr = ClientScan(argv[1]);
//         for(int i = 0; (i < MAX_ITER) && (ptr != NULL); i++) {
//             printf("Client Connect[%d]\n",i);
//             if(ClientConnect(ptr)) {
//                 printf("is ptr == NULL = %d\n", ptr == NULL);
//                 ptr = ptr->ai_next;
//                 printf("is ptr == NULL = %d\n", ptr == NULL);
//                 continue;
//             }
//             break;
//         }
//         // for(int i = 0; i < 50; i++) {
            
//         //     printf("socket = %d\n",listenSock);
//         // }
        
//     }

//     return 0;
// }

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <string.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
//27015
#define DEFAULT_PORT "80"

// void ServerFunc(void * pID) {
//     WSADATA wsaData;
//     int iResult;

//     SOCKET ListenSocket = INVALID_SOCKET;
//     SOCKET ClientSocket = INVALID_SOCKET;

//     struct addrinfo *result = NULL;
//     struct addrinfo hints;

//     int iSendResult;
//     char recvbuf[DEFAULT_BUFLEN];
//     int recvbuflen = DEFAULT_BUFLEN;
    
//     // Initialize Winsock
//     // iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
//     // if (iResult != 0) {
//     //     printf("WSAStartup failed with error: %d\n", iResult);
//     //     return;
//     // }

//     ZeroMemory(&hints, sizeof(hints));
//     hints.ai_family = AF_INET;
//     hints.ai_socktype = SOCK_STREAM;
//     hints.ai_protocol = IPPROTO_TCP;
//     hints.ai_flags = AI_PASSIVE;

//     // Resolve the server address and port
//     iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
//     if ( iResult != 0 ) {
//         printf("getaddrinfo failed with error: %d\n", iResult);
//         WSACleanup();
//         return;
//     }

//     // Create a SOCKET for connecting to server
//     ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
//     if (ListenSocket == INVALID_SOCKET) {
//         printf("socket failed with error: %ld\n", WSAGetLastError());
//         freeaddrinfo(result);
//         WSACleanup();
//         return;
//     }

//     // Setup the TCP listening socket
//     iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
//     if (iResult == SOCKET_ERROR) {
//         printf("bind failed with error: %d\n", WSAGetLastError());
//         freeaddrinfo(result);
//         closesocket(ListenSocket);
//         WSACleanup();
//         return;
//     }

//     freeaddrinfo(result);

//     iResult = listen(ListenSocket, SOMAXCONN);
//     if (iResult == SOCKET_ERROR) {
//         printf("listen failed with error: %d\n", WSAGetLastError());
//         closesocket(ListenSocket);
//         WSACleanup();
//         return;
//     }

//     // Accept a client socket
//     ClientSocket = accept(ListenSocket, NULL, NULL);
//     if (ClientSocket == INVALID_SOCKET) {
//         printf("accept failed with error: %d\n", WSAGetLastError());
//         closesocket(ListenSocket);
//         WSACleanup();
//         return;
//     }

//     // No longer need server socket
//     closesocket(ListenSocket);

//     // Receive until the peer shuts down the connection
//     do {

//         iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
//         if (iResult > 0) {
//             printf("Server Bytes received: {%d,%s}\n", iResult,recvbuf);

//         // Echo the buffer back to the sender
//             iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
//             if (iSendResult == SOCKET_ERROR) {
//                 printf("send failed with error: %d\n", WSAGetLastError());
//                 closesocket(ClientSocket);
//                 WSACleanup();
//                 return;
//             }
//             printf("Bytes sent: %d\n", iSendResult);
//         }
//         else if (iResult == 0)
//             printf("Server Connection closing...\n");
//         else  {
//             printf("recv failed with error: %d\n", WSAGetLastError());
//             closesocket(ClientSocket);
//             WSACleanup();
//             return;
//         }

//     } while (iResult > 0);

//     // shutdown the connection since we're done
//     iResult = shutdown(ClientSocket, SD_SEND);
//     if (iResult == SOCKET_ERROR) {
//         printf("shutdown failed with error: %d\n", WSAGetLastError());
//         closesocket(ClientSocket);
//         WSACleanup();
//         return;
//     }

//     // cleanup
//     closesocket(ClientSocket);
//     WSACleanup();
// }

int __cdecl main(int argc, char **argv) 
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    const char *sendbuf = "GET / HTTP/1.0\r\nHost: www.google.com/search?q=what+is+ssl\r\n\r\n"; 
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    // HANDLE hThread = (HANDLE)_beginthread(ServerFunc, 0, NULL);

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port

    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    struct in_addr addr = *(struct in_addr*)(result->ai_addr->sa_data + 2 * sizeof(short));

    printf("argv[1] = {%s}\n",argv[1]);
    printf("IP address: {%lu}\n", addr.s_addr);
    printf("Correct IP address: {%lu}\n", gethostbyname(argv[1]));

    int i = 0;
    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {
        printf("Client Connect[%d]\n",i++);
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Client Bytes Sent: {%ld, %s}\n", iResult, sendbuf);

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    FILE * fp = fopen("google.html", "w");

    // Receive until the peer closes the connection
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 ) {
            printf("Client Bytes Received: %d\n", iResult);
            fprintf(fp, "%s", recvbuf);
        }
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());
        memset(recvbuf, 0, iResult);
    } while( iResult > 0 );

    fclose(fp);

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    // WaitForSingleObject(hThread, INFINITE);

    return 0;
}

// sample_multithread_c_program.c
// compile with: /c
//
//  Bounce - Creates a new thread each time the letter 'a' is typed.
//  Each thread bounces a character of a different color around
//  the screen. All threads are terminated when the letter 'Q' is
//  entered.
//

// #include <windows.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdio.h>
// #include <conio.h>
// #include <process.h>

// #define MAX_THREADS  32

// // The function getrandom returns a random number between
// // min and max, which must be in integer range.
// #define getrandom( min, max ) (SHORT)((rand() % (int)(((max) + 1) - \
//                                (min))) + (min))

// int main(void);                    // Thread 1: main
// void KbdFunc(void);                // Keyboard input, thread dispatch
// void BounceProc(void* pMyID);      // Threads 2 to n: display
// void ClearScreen(void);            // Screen clear
// void ShutDown(void);               // Program shutdown
// void WriteTitle(int ThreadNum);    // Display title bar information

// HANDLE  hConsoleOut;                 // Handle to the console
// HANDLE  hRunMutex;                   // "Keep Running" mutex
// HANDLE  hScreenMutex;                // "Screen update" mutex
// int     ThreadNr = 0;                // Number of threads started
// CONSOLE_SCREEN_BUFFER_INFO csbiInfo; // Console information
// COORD   consoleSize;
// BOOL    bTrails = FALSE;

// HANDLE  hThreads[MAX_THREADS] = { NULL }; // Handles for created threads

// int main(void) // Thread One
// {
//     // Get display screen information & clear the screen.
//     hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
//     GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);
//     consoleSize.X = csbiInfo.srWindow.Right;
//     consoleSize.Y = csbiInfo.srWindow.Bottom;
//     ClearScreen();
//     WriteTitle(0);

//     // Create the mutexes and reset thread count.
//     hScreenMutex = CreateMutexW(NULL, FALSE, NULL);  // Cleared
//     hRunMutex = CreateMutexW(NULL, TRUE, NULL);      // Set

//     // Start waiting for keyboard input to dispatch threads or exit.
//     KbdFunc();

//     // All threads done. Clean up handles.
//     if (hScreenMutex) CloseHandle(hScreenMutex);
//     if (hRunMutex) CloseHandle(hRunMutex);
//     if (hConsoleOut) CloseHandle(hConsoleOut);
// }

// void ShutDown(void) // Shut down threads
// {
//     // Tell all threads to die
//     ReleaseMutex(hRunMutex);

//     while (ThreadNr > 0)
//     {
//         // Wait for each thread to complete
//         WaitForSingleObject(hThreads[--ThreadNr], INFINITE);
//     }

//     // Clean up display when done
//     WaitForSingleObject(hScreenMutex, INFINITE);
//     ClearScreen();
// }

// void KbdFunc(void) // Dispatch and count threads.
// {
//     int         KeyInfo;

//     do
//     {
//         KeyInfo = _getch();
//         if (tolower(KeyInfo) == 'a' &&
//             ThreadNr < MAX_THREADS)
//         {
//             ++ThreadNr;
//             hThreads[ThreadNr] = 
//                 (HANDLE)_beginthread(BounceProc, 0, (void*)(uintptr_t)ThreadNr);
//             WriteTitle(ThreadNr);
//         }

//         if (tolower(KeyInfo) == 't')
//         {
//             bTrails = !bTrails;
//         }
//     } while (tolower(KeyInfo) != 'q');

//     ShutDown();
// }

// void BounceProc(void* pMyID)
// {
//     wchar_t MyCell, OldCell;
//     WORD    MyAttrib, OldAttrib = 0;
//     wchar_t BlankCell = 0x20;
//     COORD   Coords, Delta;
//     COORD   Old = { 0,0 };
//     DWORD   Dummy;
//     int MyID = (int)(uintptr_t)pMyID;

//     // Generate update increments and initial
//     // display coordinates.
//     srand(MyID * 3);

//     Coords.X = getrandom(0, consoleSize.X - 1);
//     Coords.Y = getrandom(0, consoleSize.Y - 1);
//     Delta.X = getrandom(-3, 3);
//     Delta.Y = getrandom(-3, 3);

//     // Set up character & generate color
//     // attribute from thread number.
//     if (MyID > 16)
//         MyCell = (wchar_t)(0x60 + MyID - 16); // lower case
//     else
//         MyCell = (wchar_t)(0x40 + MyID);      // upper case
//     MyAttrib = MyID & 0x0f;   // force black background

//     do
//     {
//         // Wait for display to be available, then lock it.
//         WaitForSingleObject(hScreenMutex, INFINITE);

//         if (!bTrails)
//         {
//             // If we still occupy the old screen position, blank it out.
//             ReadConsoleOutputCharacterW(hConsoleOut, &OldCell, 1,
//                 Old, &Dummy);
//             ReadConsoleOutputAttribute(hConsoleOut, &OldAttrib, 1,
//                 Old, &Dummy);
//             if ((OldCell == MyCell) && (OldAttrib == MyAttrib))
//                 WriteConsoleOutputCharacterW(hConsoleOut, &BlankCell, 1,
//                     Old, &Dummy);
//         }

//         // Draw new character, then clear screen lock
//         WriteConsoleOutputCharacterW(hConsoleOut, &MyCell, 1,
//             Coords, &Dummy);
//         WriteConsoleOutputAttribute(hConsoleOut, &MyAttrib, 1,
//             Coords, &Dummy);
//         ReleaseMutex(hScreenMutex);

//         // Increment the coordinates for next placement of the block.
//         Old.X = Coords.X;
//         Old.Y = Coords.Y;
//         Coords.X += Delta.X;
//         Coords.Y += Delta.Y;

//         // If we are about to go off the screen, reverse direction
//         if (Coords.X < 0 || Coords.X >= consoleSize.X)
//         {
//             Delta.X = -Delta.X;
//             Beep(400, 50);
//         }
//         if (Coords.Y < 0 || Coords.Y > consoleSize.Y)
//         {
//             Delta.Y = -Delta.Y;
//             Beep(600, 50);
//         }
//     }
//     // Repeat while RunMutex is still taken.
//     while (WaitForSingleObject(hRunMutex, 75L) == WAIT_TIMEOUT);
// }

// void WriteTitle(int ThreadNum)
// {
//     enum
//     {
//         sizeOfNThreadMsg = 120
//     };
//     wchar_t    NThreadMsg[sizeOfNThreadMsg] = { L"" };

//     swprintf_s(NThreadMsg, sizeOfNThreadMsg,
//         L"Threads running: %02d.  Press 'A' "
//         L"to start a thread, 'T' to toggle "
//         L"trails, 'Q' to quit.", ThreadNum);
//     SetConsoleTitleW(NThreadMsg);
// }

// void ClearScreen(void)
// {
//     DWORD    dummy = 0;
//     COORD    Home = { 0, 0 };
//     FillConsoleOutputCharacterW(hConsoleOut, L' ',
//         consoleSize.X * consoleSize.Y,
//         Home, &dummy);
// }