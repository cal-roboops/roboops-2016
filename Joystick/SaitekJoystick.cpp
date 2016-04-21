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


// Entry point for the application
SaitekJoystick::SaitekJoystick() {
	HRESULT hr;

	// Create a DirectInput device
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**) &g_pDI, NULL))) {
		printf("Failed to create DirectInput device.\n");
		exit(1);
	}

	// Look for the first simple joystick we can find.
	if (FAILED(hr = g_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY))) {
		printf("Failed to find simple joystick.\n");
		exit(1);
	}

	// Make sure we got a joystick
	if (g_pJoystick == NULL) {
		printf("Joystick not found.\n");
		exit(1);
	}

	// Set the data format to "simple joystick" - a predefined data format 
	//
	// A data format specifies which controls on a device we are interested in,
	// and how they should be reported. This tells DInput that we will be
	// passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
	if (FAILED(hr = g_pJoystick->SetDataFormat(&c_dfDIJoystick2))) {
		printf("Couldn't set data format to simple joystick.\n");
		exit(1);
	}

	// Set the cooperative level to let DInput know how this device should
	// interact with the system and with other DInput applications.
	//if (FAILED(hr = g_pJoystick->SetCooperativeLevel(NULL, DISCL_EXCLUSIVE | DISCL_FOREGROUND))) {
	//	printf("Couldn't set cooperative level.\n");
	//	exit(2);
	//}

	// Determine how many axis the joystick has (so we don't error out setting
	// properties for unavailable axis)
	capabilities.dwSize = sizeof(DIDEVCAPS);
	if (FAILED(hr = g_pJoystick->GetCapabilities(&capabilities))) {
		printf("Couldn't determine how many axis the joystick has.\n");
		exit(1);
	}

	// Enumerate the axes of the joyctick and set the range of each axis. Note:
	// we could just use the defaults, but we're just trying to show an example
	// of enumerating device objects (axes, buttons, etc.).
	if (FAILED(hr = g_pJoystick->EnumObjects(EnumObjectsCallback, NULL, DIDFT_AXIS))) {
		printf("Couldn't enumerate the axes.\n");
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

	/*
    // Display joystick state to dialog

    // Axes
    _stprintf_s(strText, 512, TEXT("%ld"), js.lX);
    SetWindowText(GetDlgItem(hDlg, IDC_X_AXIS), strText);
    _stprintf_s(strText, 512, TEXT("%ld"), js.lY);
    SetWindowText(GetDlgItem(hDlg, IDC_Y_AXIS), strText);
    _stprintf_s(strText, 512, TEXT("%ld"), js.lZ);
    SetWindowText(GetDlgItem(hDlg, IDC_Z_AXIS), strText);
    _stprintf_s(strText, 512, TEXT("%ld"), js.lRx);
    SetWindowText(GetDlgItem(hDlg, IDC_X_ROT), strText);
    _stprintf_s(strText, 512, TEXT("%ld"), js.lRy);
    SetWindowText(GetDlgItem(hDlg, IDC_Y_ROT), strText);
    _stprintf_s(strText, 512, TEXT("%ld"), js.lRz);
    SetWindowText(GetDlgItem(hDlg, IDC_Z_ROT), strText);

    // Slider controls
    _stprintf_s(strText, 512, TEXT("%ld"), js.rglSlider[0]);
    SetWindowText(GetDlgItem(hDlg, IDC_SLIDER0), strText);
    _stprintf_s(strText, 512, TEXT("%ld"), js.rglSlider[1]);
    SetWindowText(GetDlgItem(hDlg, IDC_SLIDER1), strText);

    // Points of view
    _stprintf_s(strText, 512, TEXT("%lu"), js.rgdwPOV[0]);
    SetWindowText(GetDlgItem(hDlg, IDC_POV0), strText);
    _stprintf_s(strText, 512, TEXT("%lu"), js.rgdwPOV[1]);
    SetWindowText(GetDlgItem(hDlg, IDC_POV1), strText);
    _stprintf_s(strText, 512, TEXT("%lu"), js.rgdwPOV[2]);
    SetWindowText(GetDlgItem(hDlg, IDC_POV2), strText);
    _stprintf_s(strText, 512, TEXT("%lu"), js.rgdwPOV[3]);
    SetWindowText(GetDlgItem(hDlg, IDC_POV3), strText);


    // Fill up text with which buttons are pressed
    _tcscpy_s(strText, 512, TEXT(""));
    for (int i = 0; i < 128; i++) {
        if (js.rgbButtons[i] & 0x80) {
            TCHAR sz[128];
            _stprintf_s(sz, 128, TEXT("%02d "), i);
            _tcscat_s(strText, 512, sz);
        }
    }

    SetWindowText(GetDlgItem(hDlg, IDC_BUTTONS), strText);
	*/

    return S_OK;
}

// Global functions needed for enumerating the joystick and buttons
// These will cause program to throw an error if included under the
// SaitekJoystick class

// Called once for each enumerated joystick
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) {
	HRESULT hr;

	// Obtain an interface to the enumerated joystick.
	hr = g_pDI->CreateDevice(pdidInstance->guidInstance, &g_pJoystick, NULL);

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
