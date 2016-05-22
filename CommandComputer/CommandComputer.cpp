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
int mode;										// Mode state variable (only changes if buttons pressed)

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void				send_command();

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
    if (!InitInstance (hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COMMANDCOMPUTER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
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
ATOM MyRegisterClass(HINSTANCE hInstance) {
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd) {
      return FALSE;
   }
   
   // Set mode to initial value of MODE0
   mode = MODE0;
   // Create the Joystick for control input
   sJoy = new SaitekJoystick(hWnd);

   // Create the client for communication
   cc = new Client(ipv4, port);
   // Send Setup Command to RaspPi
   cc->client_send("Setup!");
   ZeroMemory(cc->recvbuf, sizeof(cc->recvbuf));
   while (strstr(cc->recvbuf, "Setup complete!") == NULL) {
	   cc->client_receive();
   }
   // Send Unfold Command to RaspPi
   cc->client_send("Unfold!");
   ZeroMemory(cc->recvbuf, sizeof(cc->recvbuf));
   while (strstr(cc->recvbuf, "Unfolding Complete!") == NULL) {
	   cc->client_receive();
   }
   // Wait for Rover Ready
   ZeroMemory(cc->recvbuf, sizeof(cc->recvbuf));
   while (strstr(cc->recvbuf, "Rover Ready!") == NULL) {
	   cc->client_receive();
   }

   // Set a timer to go off 3 times a second. At every timer message
   // the input device will be read
   SetTimer(hWnd, 0, 1000/3, nullptr);

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId) {
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
			// Update the joystick
			if (FAILED(sJoy->UpdateInputState())) {
				KillTimer(hWnd, 0);
				MessageBox(nullptr, TEXT("Error Reading Input State. The sample will now exit."), TEXT("DirectInput Sample"), MB_ICONERROR | MB_OK); 
				EndDialog(hWnd, TRUE);
			}

			// Compile and send the command
			send_command();
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
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Command compiler and sender
void send_command() {
	// Grab joystick values
	long x = sJoy->js.lX;
	long y = sJoy->js.lY;
	long z = sJoy->js.lZ;
	long rx = sJoy->js.lRx;
	long ry = sJoy->js.lRy;
	long rz = sJoy->js.lRz;
	long s0 = sJoy->js.rglSlider[0];
	long pov0 = sJoy->js.rgdwPOV[0];

	// Map button pushes from 0 and 128 (0x80) to 0 and 1
	byte b00 = sJoy->js.rgbButtons[0] && 0x80;
	byte b01 = sJoy->js.rgbButtons[1] && 0x80;
	byte b04 = sJoy->js.rgbButtons[4] && 0x80;
	byte b14 = sJoy->js.rgbButtons[14] && 0x80;
	byte b15 = sJoy->js.rgbButtons[15] && 0x80;
	byte b16 = sJoy->js.rgbButtons[16] && 0x80;
	byte b17 = sJoy->js.rgbButtons[17] && 0x80;
	byte b18 = sJoy->js.rgbButtons[18] && 0x80;
	byte b23 = sJoy->js.rgbButtons[23] && 0x80;
	byte b24 = sJoy->js.rgbButtons[24] && 0x80;
	byte b25 = sJoy->js.rgbButtons[25] && 0x80;

	// Initialize Eight Motor Controllers to 0
	long mc_0 = RC_FB_ZERO;
	long mc_1 = RC_FB_ZERO;
	long mc_2 = RC_FB_ZERO;
	long mc_3 = RC_FB_ZERO;
	long mc_4 = RC_FB_ZERO;
	long mc_5 = RC_FB_ZERO;
	long mc_6 = RC_FB_ZERO;
	long mc_7 = RC_FB_ZERO;

	// Initialize Eight PWM Outputs to Center
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

	// Send endMsg and exit if kill button pressed
	if (b01) {
		sprintf(cc->msgbuf, endMsg);
		cc->client_send(cc->msgbuf);
		exit(0);
	}

	// Update the mode
	if (b23 || b24) {
		mode = MODE0;
	} else if (b25) {
		mode = MODE2;
	}

	// Setup command syntax
	if ((mode == MODE0) || (mode == MODE1)) {
		// Map the commands into the appropriate range
		if (((abs(y) + 10) >= abs(rz)) || ((abs(x) + 10) >= abs(rz))) {
			if (abs(y) >= abs(x)) {
				// Keep drive servos straight to go forward/backward
				s_0 = SERVO_CENTER;
				s_1 = SERVO_CENTER;
			} else {
				// Turn drive servos 90 degres to go sideways
				s_0 = SERVO_90_Degrees;
				s_1 = SERVO_90_Degrees;
				// Use negative x instead of y
				y = -x;
			}

			// Map motor speed
			if (y < -10 || y > 10) {
				// Drive Forward/Right or Backward/Left
				mc_0 = RC_FB_MAX * ((float) -y / 1000);
			} else {
				// Stop
				mc_0 = RC_FB_ZERO;
			}

			// Set Motor 1 equal to Motor 0
			mc_1 = mc_0;
		} else {
			// Turn drive servos to 45 degrees and spin
			s_0 = SERVO_45_Degrees;
			s_1 = SERVO_45_Degrees + 350;

			// Map motor speed
			if (rz < -10 || rz > 10) {
				// Spin Left/Right
				mc_0 = RC_FB_MAX * ((float) -rz / 1000);
				mc_1 = RC_FB_MAX * ((float) rz / 1000);
			} else {
				// Stop
				mc_0 = RC_FB_ZERO;
				mc_1 = RC_FB_ZERO;
			}
		}
	} else if (mode == MODE2) {
		// Base Swivel
		if (b16) {
			mc_0 = 1;
		} else if (b18) {
			mc_0 = -1;
		}

		// Shoulder Extend
		if (b15) {
			mc_1 = 1;
		} else if (b17) {
			mc_1 = -1;
		}

		// Elbow Spin
		if (b15) {
			mc_2 = 1;
		} else if (b17) {
			mc_2 = -1;
		}

		// Forearm Extend
		if (pov0 == 0) {
			mc_3 = 1;
		} else if (pov0 == 1800) {
			mc_3 = -1;
		}

		// Claw Close
		if (b00) {
			mc_4 = 1;
		} else if (b04) {
			mc_4 = -1;
		}
	}

	// Save Compiled Command to the clients MSGBUF
	// mode, mc_0, mc_1, mc_2, mc_3, mc_4, mc_5, mc_6, mc_7, servo0, servo1, servo2, servo3, servo4, servo5, servo6, servo7
	sprintf(cc->msgbuf, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
		mode,
		mc_0, mc_1, mc_2, mc_3, mc_4, mc_5, mc_6, mc_7,
		s_0, s_1, s_2, s_3, s_4, s_5, s_6, s_7);

	// Test Saitek
	//sprintf(cc->msgbuf, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", mode, x, y, z, rx, ry, rz, s0, m1, m2, m3, m4, m5, m6);

	// Test Arm
	//long base_swivel = (1000 + ry) / 20;
	//long shoulder_rl = (1000 + s0) / 20;
	//long elbow = (1000 + rx) / 20;
	//long forearm_extend = (1000 - z) / 20;
	//sprintf(cc->msgbuf, "%d,%d,%d,%d,%d,%d", MODE2, base_swivel, shoulder_rl, elbow, forearm_extend);

	// Send the compiled message
	cc->client_send(cc->msgbuf);
}