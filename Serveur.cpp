//Empeche d'avoir certains avertissements liés à l'utilisation de fonctions risquées
#define _CRT_SECURE_NO_WARNINGS

//On se lie à la bibliothèque ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

//Include que nous allons utiliser
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>

//Définie le port
//#define PORT 80
#define PORT 14843
#define WM_TOTO (WM_USER + 1) 
#define WM_TATA (WM_USER + 2) 

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = 0;
    wcex.hCursor = 0;
    wcex.hbrBackground = 0;
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = L"WNDCLASS";
    wcex.hIconSm = 0;

    return RegisterClassExW(&wcex);
}


HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd = CreateWindowW(L"WNDCLASS", L"", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    return hWnd;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_TOTO:
        {
            int a = 0;
            // connexion des cleitn 
            break;
        }
        case WM_TATA:
        {
            // message des clients
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int main() {
    int iResult;

    HINSTANCE hInstance = GetModuleHandle(0);
    MyRegisterClass(hInstance);
    HWND hWnd = InitInstance(hInstance, 0);

    // initialisation de winsock
    WSADATA wsaData;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    // Création de la socket
    SOCKET hsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (hsocket == INVALID_SOCKET) {
        printf("socket failed\n");
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT); 
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Liaison du socket
    if (bind(hsocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(hsocket);
        WSACleanup();
        return 1;
    }

    // Ecoute du socket
    if (listen(hsocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed\n");
        closesocket(hsocket);
        WSACleanup();
        return 1;
    }


    if (WSAAsyncSelect(hsocket, hWnd, WM_TOTO, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR) {
        printf("WSAAsyncSelect failed\n");
        closesocket(hsocket);
        WSACleanup();
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
    }
    /*
    // Acceptation des connexions entrantes
    SOCKET clientSocket = accept(hsocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        printf("Accept failed\n");
        closesocket(hsocket);
        WSACleanup();
        return 1;
    }

    printf("Connecter\n");

    // Réception et affichage de la réponse
    char buffer[4096];
    int valread;
    while ((valread = recv(hsocket, buffer, sizeof(buffer) - 1, 0)) > 0) {        
        buffer[valread] = '\0';
        printf("%s", buffer);
    }

    closesocket(hsocket);*/

    WSACleanup();

    return 0;
}