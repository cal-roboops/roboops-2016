//
// Saitek_X52.h
// CPP Project
//
// Created by Mitchell Oleson on 4/4/2016
//
// Written for Windows
//

#define STRICT
#define DIRECTINPUT_VERSION 0x0800
#define _CRT_SECURE_NO_DEPRECATE
#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <commctrl.h>
#include <basetsd.h>

#pragma warning(push)
#pragma warning(disable:6000 28251)
#include <dinput.h>
#pragma warning(pop)

#include <dinputd.h>
#include <assert.h>
#include <oleauto.h>
#include <shellapi.h>
#include "resource.h"
#include <wbemidl.h>

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=nullptr; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=nullptr; } }


// Saitek_X52 Controller Class
class Saitek_X52 {
	private:
		LPDIRECTINPUT8 g_pDI = nullptr;
		LPDIRECTINPUTDEVICE8 g_pJoystick = nullptr;

		INT_PTR CALLBACK MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );
		BOOL CALLBACK    EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
		BOOL CALLBACK    EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );
		HRESULT InitDirectInput(HWND hDlg);
		VOID FreeDirectInput();
		HRESULT UpdateInputState(HWND hDlg);

		// Stuff to filter out XInput devices
		HRESULT SetupForIsXInputDevice();
		bool IsXInputDevice(const GUID* pGuidProductFromDirectInput );
		void CleanupForIsXInputDevice();

		struct XINPUT_DEVICE_NODE {
		    DWORD dwVidPid;
		    XINPUT_DEVICE_NODE* pNext;
		};

		struct DI_ENUM_CONTEXT {
		    DIJOYCONFIG* pPreferredJoyCfg;
		    bool bPreferredJoyCfgValid;
		};

		bool g_bFilterOutXinputDevices = false;
		XINPUT_DEVICE_NODE* g_pXInputDeviceList = nullptr;

	public:
		// Public Variables
		// Public Methods
		Saitek_X52( _In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int );
		~Saitek_X52();
		void update()
};