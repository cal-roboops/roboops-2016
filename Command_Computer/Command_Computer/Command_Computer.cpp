// Command_Computer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Command_Computer.h"

#define MAX_LOADSTRING 100


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

												// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance1(HINSTANCE, int);
BOOL				InitInstance2(HINSTANCE, int);
BOOL                InitInstance3(HINSTANCE, int);
BOOL                InitInstance4(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_COMMAND_COMPUTER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance1(hInstance, nCmdShow))
	{
		return FALSE;
	}

	if (!InitInstance2(hInstance, nCmdShow))
	{
		return FALSE;
	}

	if (!InitInstance3(hInstance, nCmdShow))
	{
		return FALSE;
	}

	if (!InitInstance4(hInstance, nCmdShow))
	{
		return FALSE;
	}


	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COMMAND_COMPUTER));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW window;

	window.cbSize = sizeof(WNDCLASSEX);

	window.style = CS_HREDRAW | CS_VREDRAW;
	window.lpfnWndProc = WndProc;
	window.cbClsExtra = 0;
	window.cbWndExtra = 0;
	window.hInstance = hInstance;
	window.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COMMAND_COMPUTER));
	window.hCursor = LoadCursor(nullptr, IDC_ARROW);
	window.hbrBackground = CreateSolidBrush(RGB(150, 0, 0));
	window.lpszMenuName = MAKEINTRESOURCEW(IDC_COMMAND_COMPUTER);
	window.lpszClassName = szWindowClass;
	window.hIconSm = LoadIcon(window.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&window);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance1(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd1 = CreateWindowW(szWindowClass, L"ARM", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd1)
	{
		return FALSE;
	}

	ShowWindow(hWnd1, SW_MAXIMIZE);
	MoveWindow(hWnd1, 0, 0, 645, 350, true);
	UpdateWindow(hWnd1);

	return TRUE;
}


BOOL InitInstance2(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd2 = CreateWindowW(szWindowClass, L"STATE VARIABLES", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd2)
	{
		return FALSE;  
	}

	ShowWindow(hWnd2, SW_MAXIMIZE);
	MoveWindow(hWnd2, 635, 0, 645, 350, true);
	UpdateWindow(hWnd2);

	return TRUE;
}

BOOL InitInstance3(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd3 = CreateWindowW(szWindowClass, L"CAMERA 1", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd3)
	{
		return FALSE;
	}

	ShowWindow(hWnd3, SW_MAXIMIZE);
	MoveWindow(hWnd3, 0, 350, 645, 350, true);
	UpdateWindow(hWnd3);

	return TRUE;
}

BOOL InitInstance4(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd4 = CreateWindowW(szWindowClass, L"CAMERA 2", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd4)
	{
		return FALSE;
	}

	ShowWindow(hWnd4, SW_MAXIMIZE);
	MoveWindow(hWnd4, 635, 350, 645, 350, true);
	UpdateWindow(hWnd4);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//BOOL MoveWindow(HWND hWnd, int X, int Y, int nWidth, int nHeight, BOOL bRepaint)
//
//In_ HWND hWnd1,
//In_ int X;
//In_ int Y;
//In_ int nWidth;
//In_ int nHeight;
//In_ BOOL bRepaint;
//eturn true;
//}