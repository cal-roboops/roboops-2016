//
// SaitekJoystick.h
// CPP Project
// 
// Created by Mitchell Oleson on 4/16/2016
//
// Made for Windows
//

//#define STRICT
//#define DIRECTINPUT_VERSION 0x0800
//#define _CRT_SECURE_NO_DEPRECATE
//#ifndef _WIN32_DCOM
//#define _WIN32_DCOM
//#endif

#define DEFAULT_BUFLEN 512
#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
//#include <commctrl.h>
//#include <basetsd.h>

#pragma warning(push)
#pragma warning(disable:6000 28251)
#include <dinput.h>
#pragma warning(pop)

#include <dinputd.h>
//#include <assert.h>
//#include <oleauto.h>
//#include <shellapi.h>

// Stuff to filter out XInput devices
#include <wbemidl.h>
#include "../CommandComputer/Resource.h"

// Constants and Globals
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=nullptr; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=nullptr; } }

// Global Structs
struct XINPUT_DEVICE_NODE {
	DWORD dwVidPid;
	XINPUT_DEVICE_NODE* pNext;
};

struct DI_ENUM_CONTEXT {
	DIJOYCONFIG* pPreferredJoyCfg;
	bool bPreferredJoyCfgValid;
};

// Global Variables
LPDIRECTINPUT8 g_pDI = nullptr;
LPDIRECTINPUTDEVICE8 g_pJoystick = nullptr;

// Global Methods
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);


class SaitekJoystick {
	private:
		// Private Variables
		bool g_bFilterOutXinputDevices = false;
		XINPUT_DEVICE_NODE* g_pXInputDeviceList = nullptr;
		DIDEVCAPS capabilities;

		// Private Methods

		
	public:
		// Public Variables
		DIJOYSTATE2* js;

		// Public Methods
		SaitekJoystick();
		~SaitekJoystick();
		HRESULT UpdateInputState();
};