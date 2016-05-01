// CommandComputer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CommandComputer.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

SaitekJoystick* sJoy;							// Global Joystick Object
Client* cc;										// Global Client Object

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void				compile_message();

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
    LoadStringW(hInstance, IDC_COMMANDCOMPUTER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COMMANDCOMPUTER));

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

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COMMANDCOMPUTER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_COMMANDCOMPUTER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   
   // Create the client for communication
   cc = new Client(ipv4, port);
   // Send Setup Command to RaspPi
   cc->client_send("Setup!");
   // Create the Joystick for control input
   sJoy = new SaitekJoystick(hWnd);
   // Send Unfold Command to RaspPi
   cc->client_send("Unfold!");
   cc->client_receive();

   // Set a timer to go off 30 times a second. At every timer message
   // the input device will be read
   SetTimer(hWnd, 0, 1000, nullptr);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
	case WM_TIMER:
		{
			if (FAILED(sJoy->UpdateInputState())) {
				KillTimer(hWnd, 0);
				MessageBox(nullptr, TEXT("Error Reading Input State. The sample will now exit."), TEXT("DirectInput Sample"), MB_ICONERROR | MB_OK); 
				EndDialog(hWnd, TRUE);
			}
			compile_message();
			cc->client_send(cc->msgbuf);
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

// Message compiler for rover send
void compile_message() {
	// Grab all values that we could possibly need
	long x = sJoy->js.lX;
	long y = sJoy->js.lY;
	long z = sJoy->js.lZ;
	long ry = sJoy->js.lRy;
	long rz = sJoy->js.lRz;
	long s0 = sJoy->js.rglSlider[0];
	byte m1 = 0x80; // sJoy->js.rgbButtons[];
	byte m2 = 0x80; // sJoy->js.rgbButtons[];
	byte m3 = 0x80; // sJoy->js.rgbButtons[];

	// Mode
	long mode = MODE0;
	// Four Motor Controllers
	long mc_0 = RC_COMBINEDFB_ZERO;
	long mc_1 = RC_COMBINEDFB_ZERO;
	long mc_2 = RC_COMBINEDFB_ZERO;
	long mc_3 = RC_COMBINEDFB_ZERO;
	// Eight PWM Outputs
	long s_0 = SERVO_CENTER;
	long s_1 = SERVO_CENTER;
	long s_2 = SERVO_CENTER;
	long s_3 = SERVO_CENTER;
	long s_4 = SERVO_CENTER;
	long s_5 = SERVO_CENTER;
	long s_6 = SERVO_CENTER;
	long s_7 = SERVO_CENTER;

	// Reset memory before writing updated command
	ZeroMemory(cc->msgbuf, sizeof(cc->msgbuf));

	if ((m1 & 0x80) || (m2 & 0x80)) {
		// Set Mode to Tank
		mode = MODE0;

		// Map the commands into the appropriate range
		if ((abs(y) >= abs(rz)) || (abs(x) >= abs(rz))) {
			if (abs(y) >= abs(x)) {
				// Keep drive servos straight to go forward/backward
				s_0 = SERVO_CENTER;
				s_1 = SERVO_CENTER;
				s_2 = SERVO_CENTER;
				s_3 = SERVO_CENTER;
			} else {
				// Turn drive servos 90 degres to go sideways
				s_0 = SERVO_90_Degrees;
				s_1 = SERVO_90_Degrees;
				s_2 = SERVO_90_Degrees;
				s_3 = SERVO_90_Degrees;
				// Use negative x instead of y
				y = -x;
			}

			// Map motor speed
			if (y != 0) {
				// Drive Forward/Right or Backward/Left
				mc_0 = 64 - (64 * y / 1000);
			} else {
				// Stop
				mc_0 = RC_COMBINEDFB_ZERO;
			}

			// Set Motor 1 equal to Motor 0
			mc_1 = mc_0;
		} else {
			// Turn drive servos to 45 degrees and spin
			s_0 = SERVO_45_Degrees;
			s_1 = SERVO_45_Degrees;
			s_2 = SERVO_45_Degrees;
			s_3 = SERVO_45_Degrees;

			// Map motor speed
			if (rz != 0) {
				// Spin Left/Right
				mc_0 = 64 - (64 * rz / 1000);
				mc_1 = 64 + (64 * rz / 1000);
			} else {
				// Stop
				mc_0 = RC_COMBINEDFB_ZERO;
				mc_1 = RC_COMBINEDFB_ZERO;
			}
		}
	} else if (m3 & 0x80) {
		// Set Mode to Arm
		mode = MODE2;


	}

	// Ensure that mc_0, mc_1, mc_2 and mc_3 are within the viable range
	if (mc_0 > RC_HIGH) {
		mc_0 = RC_HIGH;
	} else if (mc_0 < RC_LOW) {
		mc_0 = RC_LOW;
	}

	if (mc_1 > RC_HIGH) {
		mc_1 = RC_HIGH;
	} else if (mc_1 < RC_LOW) {
		mc_1 = RC_LOW;
	}

	if (mc_2 > RC_HIGH) {
		mc_2 = RC_HIGH;
	} else if (mc_2 < RC_LOW) {
		mc_2 = RC_LOW;
	}

	if (mc_3 > RC_HIGH) {
		mc_3 = RC_HIGH;
	} else if (mc_3 < RC_LOW) {
		mc_3 = RC_LOW;
	}

	// Save Compiled Command to the clients MSGBUF
	// mode, roboclaw0, roboclaw1, roboclaw2, servo0, servo1, servo2, servo3, servo4, servo5, servo6, servo7
	sprintf(cc->msgbuf, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", mode, mc_0, mc_1, mc_2, mc_3, s_0, s_1, s_2, s_3, s_4, s_5, s_6, s_7);

	// Test
	//sprintf(cc->msgbuf, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", mode, x, y, z, ry, rz, s0, m1, m2, m3, 0, 0, 0);
}