#include <dinput.h>
dinput8.lib dxguid.lib

#define SAFE_DELETE(p) { if(p) { delete(p); (p) = NULL; }}
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = NULL; }}

LPDIRECTINPUTDEVICE8 joystick;
IDirectInput8 di;

HRESULT hr;
hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8,
	(VOID**)&di, NULL);

if (FAILED(hr)) {
	return DIdevice;
}



hr = di->EnumDevices(DI8DEVCLASS_GAMECTRL, enumCallback, NULL, DIEDBSFL_ATTACHEDONLY);

if (FAILED(DIdevice)) {

	return hr;
}

if (joystick == NULL) {
	printf("Joystick not found.\n");
}

BOOL CALLBACK enumCallback(const DIDEVICEINSTANCE* instance, VOID* context)
{
	HRESULT hr;
	hr = di->CreateDevice(instance->guiInstance, &joystick, NULL);
	if (FAILED(hr)) {
		return DIENUM_CONTINUE;
	}
	return DIENUM_STOP;
}

DIDEVCAPS capabilities;

hr = joystick->SetDataFormat(&c_dfDIJoystick2);
if (FAILED(hr)) {
	return hr;
}

hr = joystick->SetCooperativeLevel(NULL, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
if (FAILEd(hr)) {
	return hr;
}

capabilities.dwsize = sizeof(DIDEVCAPS);
hr = joystick->GetCapabilities(&capabilities);
if (FAILED(hr)) {
	return hr;
}

HRRESULT UpdateInputState(HWND hDlg)
{
	HRESULT hr;
	TCHAR strText[512] = {0};
	DIJOYSTATE2 js;
	if (joystick == NULL) {
		return S_OK;
	}

	hr = joystick->Poll;
	if (FAILED(hr)) {
		hr = joystick->Acquire();
		while (hr == DIERR_INPUTLOST) {
			hr = joystick->Acquire();
		}
		if ((hr == DIERR_INVALIDPARAM) || (hr == DIERR_NOTINITIALIZED)) {
			return E_FAIL;
		}
		if (hr == DIERR_OTHERAPPHASPRIO) {
			cout << "Another device is using the joystick.";
			return S_OK;
		}
	}

	if (FAILED(hr = joystick->GetDeviceState(sizeof(DIJOYSTATE2), js)) {
		return hr;
	}
}

