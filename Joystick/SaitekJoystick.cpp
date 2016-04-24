//
// SaitekJoystick.cpp
// CPP Project
// 
// Created by Mitchell Oleson on 4/19/2016
//
// Made for Windows
//

#include "stdafx.h" // Needed for Visual Studio to compile
#include "SaitekJoystick.h"

bool g_bFilterOutXinputDevices = false;
XINPUT_DEVICE_NODE* g_pXInputDeviceList = nullptr;

// Entry point for the application
SaitekJoystick::SaitekJoystick(HWND hDlg) {
	HRESULT hr;

	// Register with the DirectInput subsystem and get a pointer
	// to a IDirectInput interface we can use.
	// Create a DInput object
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_pDI, nullptr))) {
		exit(1);
	}


	if (g_bFilterOutXinputDevices) {
		SetupForIsXInputDevice();
	}

	DIJOYCONFIG PreferredJoyCfg = { 0 };
	DI_ENUM_CONTEXT enumContext;
	enumContext.pPreferredJoyCfg = &PreferredJoyCfg;
	enumContext.bPreferredJoyCfgValid = false;

	IDirectInputJoyConfig8* pJoyConfig = nullptr;
	if (FAILED(hr = g_pDI->QueryInterface(IID_IDirectInputJoyConfig8, (void**)&pJoyConfig))) {
		exit(1);
	}

	PreferredJoyCfg.dwSize = sizeof(PreferredJoyCfg);
	// This function is expected to fail if no joystick is attached
	if (SUCCEEDED(pJoyConfig->GetConfig(0, &PreferredJoyCfg, DIJC_GUIDINSTANCE))) {
		enumContext.bPreferredJoyCfgValid = true;
	}
	SAFE_RELEASE(pJoyConfig);

	// Look for a simple joystick we can use for this sample program.
	if (FAILED(hr = g_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, &enumContext, DIEDFL_ATTACHEDONLY))) {
		exit(1);
	}

	if (g_bFilterOutXinputDevices) {
		CleanupForIsXInputDevice();
	}

	// Make sure we got a joystick
	if (!g_pJoystick) {
		MessageBox(nullptr, TEXT("Joystick not found. The sample will now exit."), TEXT("DirectInput Sample"), MB_ICONERROR | MB_OK);
		EndDialog(hDlg, 0);
		exit(1);
	}

	// Set the data format to "simple joystick" - a predefined data format 
	//
	// A data format specifies which controls on a device we are interested in,
	// and how they should be reported. This tells DInput that we will be
	// passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
	if (FAILED(hr = g_pJoystick->SetDataFormat(&c_dfDIJoystick2))) {
		exit(1);
	}

	// Set the cooperative level to let DInput know how this device should
	// interact with the system and with other DInput applications.
	// DISCL_EXCLUSIVE, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_BACKGROUND
	if (FAILED(hr = g_pJoystick->SetCooperativeLevel(hDlg, DISCL_EXCLUSIVE | DISCL_BACKGROUND))) {
		exit(1);
	}

	// Enumerate the joystick objects. The callback function enabled user
	// interface elements for objects that are found, and sets the min/max
	// values property for discovered axes.
	if (FAILED(hr = g_pJoystick->EnumObjects(EnumObjectsCallback, (VOID*)hDlg, DIDFT_ALL))) {
		exit(1);
	}
}

// Uninitialize the DirectInput variables
SaitekJoystick::~SaitekJoystick() {
	// Unacquire the device one last time just in case 
	// the app tried to exit while the device is still acquired.
	if (g_pJoystick) {
		g_pJoystick->Unacquire();
	}

	// Release any DirectInput objects.
	SAFE_RELEASE(g_pJoystick);
	SAFE_RELEASE(g_pDI);
}


// Setup the XInput Device
HRESULT SaitekJoystick::SetupForIsXInputDevice() {
	IWbemServices* pIWbemServices = nullptr;
	IEnumWbemClassObject* pEnumDevices = nullptr;
	IWbemLocator* pIWbemLocator = nullptr;
	IWbemClassObject* pDevices[20] = { 0 };
	BSTR bstrDeviceID = nullptr;
	BSTR bstrClassName = nullptr;
	BSTR bstrNamespace = nullptr;
	DWORD uReturned = 0;
	bool bCleanupCOM = false;
	UINT iDevice = 0;
	VARIANT var;
	HRESULT hr;

	// CoInit if needed
	hr = CoInitialize(nullptr);
	bCleanupCOM = SUCCEEDED(hr);

	// Create WMI
	hr = CoCreateInstance(__uuidof(WbemLocator), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IWbemLocator), (LPVOID*)&pIWbemLocator);
	if (FAILED(hr) || pIWbemLocator == nullptr) {
		goto LCleanup;
	}

	// Create BSTRs for WMI
	bstrNamespace = SysAllocString(L"\\\\.\\root\\cimv2"); if (bstrNamespace == nullptr) goto LCleanup;
	bstrDeviceID = SysAllocString(L"DeviceID");           if (bstrDeviceID == nullptr)  goto LCleanup;
	bstrClassName = SysAllocString(L"Win32_PNPEntity");    if (bstrClassName == nullptr) goto LCleanup;

	// Connect to WMI 
	hr = pIWbemLocator->ConnectServer(bstrNamespace, nullptr, nullptr, 0L, 0L, nullptr, nullptr, &pIWbemServices);
	if (FAILED(hr) || pIWbemServices == nullptr) {
		goto LCleanup;
	}

	// Switch security level to IMPERSONATE
	(void)CoSetProxyBlanket(pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, 0);

	// Get list of Win32_PNPEntity devices
	hr = pIWbemServices->CreateInstanceEnum(bstrClassName, 0, nullptr, &pEnumDevices);
	if (FAILED(hr) || pEnumDevices == nullptr) {
		goto LCleanup;
	}

	// Loop over all devices
	for (; ; ) {
		// Get 20 at a time
		hr = pEnumDevices->Next(10000, 20, pDevices, &uReturned);
		if (FAILED(hr)) {
			goto LCleanup;
		}
		if (uReturned == 0) {
			break;
		}

		for (iDevice = 0; iDevice < uReturned; iDevice++) {
			if (!pDevices[iDevice]) {
				continue;
			}

			// For each device, get its device ID
			hr = pDevices[iDevice]->Get(bstrDeviceID, 0L, &var, nullptr, nullptr);
			if (SUCCEEDED(hr) && var.vt == VT_BSTR && var.bstrVal != nullptr) {
				// Check if the device ID contains "IG_".  If it does, then it’s an XInput device
				// Unfortunately this information can not be found by just using DirectInput 
				if (wcsstr(var.bstrVal, L"IG_")) {
					// If it does, then get the VID/PID from var.bstrVal
					DWORD dwPid = 0, dwVid = 0;
					WCHAR* strVid = wcsstr(var.bstrVal, L"VID_");
					if (strVid && swscanf(strVid, L"VID_%4X", &dwVid) != 1) {
						dwVid = 0;
					}
					WCHAR* strPid = wcsstr(var.bstrVal, L"PID_");
					if (strPid && swscanf(strPid, L"PID_%4X", &dwPid) != 1) {
						dwPid = 0;
					}

					DWORD dwVidPid = MAKELONG(dwVid, dwPid);

					// Add the VID/PID to a linked list
					XINPUT_DEVICE_NODE* pNewNode = new XINPUT_DEVICE_NODE;
					if (pNewNode) {
						pNewNode->dwVidPid = dwVidPid;
						pNewNode->pNext = g_pXInputDeviceList;
						g_pXInputDeviceList = pNewNode;
					}
				}
			}
			SAFE_RELEASE(pDevices[iDevice]);
		}
	}

LCleanup:
	if (bstrNamespace)
		SysFreeString(bstrNamespace);
	if (bstrDeviceID)
		SysFreeString(bstrDeviceID);
	if (bstrClassName)
		SysFreeString(bstrClassName);
	for (iDevice = 0; iDevice < 20; iDevice++)
		SAFE_RELEASE(pDevices[iDevice]);
	SAFE_RELEASE(pEnumDevices);
	SAFE_RELEASE(pIWbemLocator);
	SAFE_RELEASE(pIWbemServices);

	return hr;
}

// Get the input device's state and display it
HRESULT SaitekJoystick::UpdateInputState() {
    HRESULT hr;

	if (g_pJoystick == NULL) {
		return S_OK;
	}

    // Poll the device to read the current state
    hr = g_pJoystick->Poll();
    if (FAILED(hr)) {
        // DInput is telling us that the input stream has been
        // interrupted. We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done. We
        // just re-acquire and try again.
        hr = g_pJoystick->Acquire();
		while (hr == DIERR_INPUTLOST) {
			hr = g_pJoystick->Acquire();
		}

        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
        return S_OK;
    }

    // Get the input's device state
	if (FAILED(hr = g_pJoystick->GetDeviceState(sizeof(DIJOYSTATE2), &js))) {
		// The device should have been acquired during the Poll()
		return hr;
	}

    return S_OK;
}



// Global functions needed for enumerating the joystick and buttons
// These will cause program to throw an error if included under the
// SaitekJoystick class

// Called once for each enumerated joystick
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) {
	auto pEnumContext = reinterpret_cast<DI_ENUM_CONTEXT*>(pContext);
	HRESULT hr;

	if (g_bFilterOutXinputDevices && IsXInputDevice(&pdidInstance->guidProduct)) {
		return DIENUM_CONTINUE;
	}

	// Skip anything other than the perferred joystick device as defined by the control panel.  
	// Instead you could store all the enumerated joysticks and let the user pick.
	if (pEnumContext->bPreferredJoyCfgValid && !IsEqualGUID(pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance)) {
		return DIENUM_CONTINUE;
	}

	// Obtain an interface to the enumerated joystick.
	hr = g_pDI->CreateDevice(pdidInstance->guidInstance, &g_pJoystick, nullptr);

	// If it failed, then we can't use this joystick. (Maybe the user unplugged
	// it while we were in the middle of enumerating it.)
	if (FAILED(hr)) {
		return DIENUM_CONTINUE;
	}

	// Stop enumeration. Note: we're just taking the first joystick we get. You
	// could store all the enumerated joysticks and let the user pick.
	return DIENUM_STOP;
}

// Callback function for enumerating objects (axes, buttons, POVs) on a joystick
BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext) {
	HWND hDlg = (HWND)pContext;

	static int nSliderCount = 0;  // Number of returned slider controls
	static int nPOVCount = 0;     // Number of returned POV controls

								  // For axes that are returned, set the DIPROP_RANGE property for the
								  // enumerated axis in order to scale min/max values.
	if (pdidoi->dwType & DIDFT_AXIS) {
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
		diprg.lMin = -1000;
		diprg.lMax = +1000;

		// Set the range for the axis
		if (FAILED(g_pJoystick->SetProperty(DIPROP_RANGE, &diprg.diph))) {
			return DIENUM_STOP;
		}

	}

	return DIENUM_CONTINUE;
}

// Test whether object is an XInput Device
bool IsXInputDevice(const GUID* pGuidProductFromDirectInput) {
	// Check each xinput device to see if this device's vid/pid matches
	XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
	while (pNode) {
		if (pNode->dwVidPid == pGuidProductFromDirectInput->Data1) {
			return true;
		}
		pNode = pNode->pNext;
	}

	return false;
}

// Cleanup XInput Device Testing
void CleanupForIsXInputDevice() {
	// Cleanup linked list
	XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
	while (pNode) {
		XINPUT_DEVICE_NODE* pDelete = pNode;
		pNode = pNode->pNext;
		SAFE_DELETE(pDelete);
	}
}