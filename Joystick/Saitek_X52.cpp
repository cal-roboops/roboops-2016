//
// Saitek_X52.cpp
// CPP Project
//
// Created by Mitchell Oleson on 4/4/2016
//
// Written for Windows
//

#include "Saitek_X52.h"

// Saitek Controller Constructor
Saitek_X52::Saitek_X52( _In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int ) {
	InitCommonControls();

    WCHAR* strCmdLine;
    int nNumArgs;
    LPWSTR* pstrArgList = CommandLineToArgvW( GetCommandLineW(), &nNumArgs );
    for( int iArg = 1; iArg < nNumArgs; iArg++ )
    {
        strCmdLine = pstrArgList[iArg];

        // Handle flag args
        if( *strCmdLine == L'/' || *strCmdLine == L'-' )
        {
            strCmdLine++;

            int nArgLen = ( int )wcslen( L"noxinput" );
            if( _wcsnicmp( strCmdLine, L"noxinput", nArgLen ) == 0 && strCmdLine[nArgLen] == 0 )
            {
                g_bFilterOutXinputDevices = true;
                continue;
            }
        }
    }
    LocalFree( pstrArgList );

    // Display the main dialog box.
    DialogBox( hInst, MAKEINTRESOURCE( IDD_JOYST_IMM ), nullptr, MainDlgProc );

    return 0;
}

// Saitek Controller Destructor
Saitek_X52::~Saitek_X52() {
	FreeDirectInput();
}

// Initialize the direct input device
HRESULT Saitek_X52::InitDirectInput(HWND hDlg) {
	HRESULT hr;

    // Register with the DirectInput subsystem and get a pointer
    // to a IDirectInput interface we can use.
    // Create a DInput object
    if( FAILED( hr = DirectInput8Create( GetModuleHandle( nullptr ), DIRECTINPUT_VERSION,
                                         IID_IDirectInput8, ( VOID** )&g_pDI, nullptr ) ) )
        return hr;


    if( g_bFilterOutXinputDevices )
        SetupForIsXInputDevice();

    DIJOYCONFIG PreferredJoyCfg = {0};
    DI_ENUM_CONTEXT enumContext;
    enumContext.pPreferredJoyCfg = &PreferredJoyCfg;
    enumContext.bPreferredJoyCfgValid = false;

    IDirectInputJoyConfig8* pJoyConfig = nullptr;
    if( FAILED( hr = g_pDI->QueryInterface( IID_IDirectInputJoyConfig8, ( void** )&pJoyConfig ) ) )
        return hr;

    PreferredJoyCfg.dwSize = sizeof( PreferredJoyCfg );
    if( SUCCEEDED( pJoyConfig->GetConfig( 0, &PreferredJoyCfg, DIJC_GUIDINSTANCE ) ) ) // This function is expected to fail if no joystick is attached
        enumContext.bPreferredJoyCfgValid = true;
    SAFE_RELEASE( pJoyConfig );

    // Look for a simple joystick we can use for this sample program.
    if( FAILED( hr = g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL,
                                         EnumJoysticksCallback,
                                         &enumContext, DIEDFL_ATTACHEDONLY ) ) )
        return hr;

    if( g_bFilterOutXinputDevices )
        CleanupForIsXInputDevice();

    // Make sure we got a joystick
    if( !g_pJoystick ) {
        MessageBox( nullptr, TEXT( "Joystick not found. The sample will now exit." ),
                    TEXT( "DirectInput Sample" ),
                    MB_ICONERROR | MB_OK );
        EndDialog( hDlg, 0 );
        return S_OK;
    }

    // Set the data format to "simple joystick" - a predefined data format 
    //
    // A data format specifies which controls on a device we are interested in,
    // and how they should be reported. This tells DInput that we will be
    // passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
    if( FAILED( hr = g_pJoystick->SetDataFormat( &c_dfDIJoystick2 ) ) )
        return hr;

    // Set the cooperative level to let DInput know how this device should
    // interact with the system and with other DInput applications.
    if( FAILED( hr = g_pJoystick->SetCooperativeLevel( hDlg, DISCL_EXCLUSIVE |
                                                       DISCL_FOREGROUND ) ) )
        return hr;

    // Enumerate the joystick objects. The callback function enabled user
    // interface elements for objects that are found, and sets the min/max
    // values property for discovered axes.
    if( FAILED( hr = g_pJoystick->EnumObjects( EnumObjectsCallback,
                                               ( VOID* )hDlg, DIDFT_ALL ) ) )
        return hr;

    return S_OK;
}

HRESULT Saitek_X52::SetupForIsXInputDevice() {
    IWbemServices* pIWbemServices = nullptr;
    IEnumWbemClassObject* pEnumDevices = nullptr;
    IWbemLocator* pIWbemLocator = nullptr;
    IWbemClassObject* pDevices[20] = {0};
    BSTR bstrDeviceID = nullptr;
    BSTR bstrClassName = nullptr;
    BSTR bstrNamespace = nullptr;
    DWORD uReturned = 0;
    bool bCleanupCOM = false;
    UINT iDevice = 0;
    VARIANT var;
    HRESULT hr;

    // CoInit if needed
    hr = CoInitialize( nullptr );
    bCleanupCOM = SUCCEEDED( hr );

    // Create WMI
    hr = CoCreateInstance( __uuidof( WbemLocator ),
                           nullptr,
                           CLSCTX_INPROC_SERVER,
                           __uuidof( IWbemLocator ),
                           ( LPVOID* )&pIWbemLocator );
    if( FAILED( hr ) || pIWbemLocator == nullptr )
        goto LCleanup;

    // Create BSTRs for WMI
    bstrNamespace = SysAllocString( L"\\\\.\\root\\cimv2" ); if( bstrNamespace == nullptr ) goto LCleanup;
    bstrDeviceID = SysAllocString( L"DeviceID" );           if( bstrDeviceID == nullptr )  goto LCleanup;
    bstrClassName = SysAllocString( L"Win32_PNPEntity" );    if( bstrClassName == nullptr ) goto LCleanup;

    // Connect to WMI 
    hr = pIWbemLocator->ConnectServer( bstrNamespace, nullptr, nullptr, 0L,
                                       0L, nullptr, nullptr, &pIWbemServices );
    if( FAILED( hr ) || pIWbemServices == nullptr )
        goto LCleanup;

    // Switch security level to IMPERSONATE
    (void)CoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr,
                             RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, 0 );

    // Get list of Win32_PNPEntity devices
    hr = pIWbemServices->CreateInstanceEnum( bstrClassName, 0, nullptr, &pEnumDevices );
    if( FAILED( hr ) || pEnumDevices == nullptr )
        goto LCleanup;

    // Loop over all devices
    for(; ; ) {
        // Get 20 at a time
        hr = pEnumDevices->Next( 10000, 20, pDevices, &uReturned );
        if( FAILED( hr ) )
            goto LCleanup;
        if( uReturned == 0 )
            break;

        for( iDevice = 0; iDevice < uReturned; iDevice++ ) {
            if ( !pDevices[iDevice] )
               continue;

            // For each device, get its device ID
            hr = pDevices[iDevice]->Get( bstrDeviceID, 0L, &var, nullptr, nullptr );
            if( SUCCEEDED( hr ) && var.vt == VT_BSTR && var.bstrVal != nullptr ) {
                // Check if the device ID contains "IG_".  If it does, then it’s an XInput device
                // Unfortunately this information can not be found by just using DirectInput 
                if( wcsstr( var.bstrVal, L"IG_" ) ) {
                    // If it does, then get the VID/PID from var.bstrVal
                    DWORD dwPid = 0, dwVid = 0;
                    WCHAR* strVid = wcsstr( var.bstrVal, L"VID_" );
                    if( strVid && swscanf( strVid, L"VID_%4X", &dwVid ) != 1 )
                        dwVid = 0;
                    WCHAR* strPid = wcsstr( var.bstrVal, L"PID_" );
                    if( strPid && swscanf( strPid, L"PID_%4X", &dwPid ) != 1 )
                        dwPid = 0;

                    DWORD dwVidPid = MAKELONG( dwVid, dwPid );

                    // Add the VID/PID to a linked list
                    XINPUT_DEVICE_NODE* pNewNode = new XINPUT_DEVICE_NODE;
                    if( pNewNode ) {
                        pNewNode->dwVidPid = dwVidPid;
                        pNewNode->pNext = g_pXInputDeviceList;
                        g_pXInputDeviceList = pNewNode;
                    }
                }
            }
            SAFE_RELEASE( pDevices[iDevice] );
        }
    }

LCleanup:
    if( bstrNamespace )
        SysFreeString( bstrNamespace );
    if( bstrDeviceID )
        SysFreeString( bstrDeviceID );
    if( bstrClassName )
        SysFreeString( bstrClassName );
    for( iDevice = 0; iDevice < 20; iDevice++ )
    SAFE_RELEASE( pDevices[iDevice] );
    SAFE_RELEASE( pEnumDevices );
    SAFE_RELEASE( pIWbemLocator );
    SAFE_RELEASE( pIWbemServices );

    return hr;
}

bool Saitek_X52::IsXInputDevice( const GUID* pGuidProductFromDirectInput ) {
    // Check each xinput device to see if this device's vid/pid matches
    XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
    while( pNode ) {
        if( pNode->dwVidPid == pGuidProductFromDirectInput->Data1 )
            return true;
        pNode = pNode->pNext;
    }

    return false;
}

void Saitek_X52::CleanupForIsXInputDevice() {
    // Cleanup linked list
    XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
    while( pNode ) {
        XINPUT_DEVICE_NODE* pDelete = pNode;
        pNode = pNode->pNext;
        SAFE_DELETE( pDelete );
    }
}

BOOL CALLBACK Saitek_X52::EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,
                                     VOID* pContext ) {
    auto pEnumContext = reinterpret_cast<DI_ENUM_CONTEXT*>( pContext );
    HRESULT hr;

    if( g_bFilterOutXinputDevices && IsXInputDevice( &pdidInstance->guidProduct ) )
        return DIENUM_CONTINUE;

    // Skip anything other than the perferred joystick device as defined by the control panel.  
    // Instead you could store all the enumerated joysticks and let the user pick.
    if( pEnumContext->bPreferredJoyCfgValid &&
        !IsEqualGUID( pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance ) )
        return DIENUM_CONTINUE;

    // Obtain an interface to the enumerated joystick.
    hr = g_pDI->CreateDevice( pdidInstance->guidInstance, &g_pJoystick, nullptr );

    // If it failed, then we can't use this joystick. (Maybe the user unplugged
    // it while we were in the middle of enumerating it.)
    if( FAILED( hr ) )
        return DIENUM_CONTINUE;

    // Stop enumeration. Note: we're just taking the first joystick we get. You
    // could store all the enumerated joysticks and let the user pick.
    return DIENUM_STOP;
}

BOOL CALLBACK Saitek_X52::EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
                                   VOID* pContext ) {
    HWND hDlg = ( HWND )pContext;

    static int nSliderCount = 0;  // Number of returned slider controls
    static int nPOVCount = 0;     // Number of returned POV controls

    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if( pdidoi->dwType & DIDFT_AXIS ) {
        DIPROPRANGE diprg;
        diprg.diph.dwSize = sizeof( DIPROPRANGE );
        diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
        diprg.diph.dwHow = DIPH_BYID;
        diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin = -1000;
        diprg.lMax = +1000;

        // Set the range for the axis
        if( FAILED( g_pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
            return DIENUM_STOP;

    }

/*
    // Set the UI to reflect what objects the joystick supports
    if( pdidoi->guidType == GUID_XAxis ) {
        EnableWindow( GetDlgItem( hDlg, IDC_X_AXIS ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_X_AXIS_TEXT ), TRUE );
    }
    if( pdidoi->guidType == GUID_YAxis ) {
        EnableWindow( GetDlgItem( hDlg, IDC_Y_AXIS ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_Y_AXIS_TEXT ), TRUE );
    }
    if( pdidoi->guidType == GUID_ZAxis ) {
        EnableWindow( GetDlgItem( hDlg, IDC_Z_AXIS ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_Z_AXIS_TEXT ), TRUE );
    }
    if( pdidoi->guidType == GUID_RxAxis ) {
        EnableWindow( GetDlgItem( hDlg, IDC_X_ROT ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_X_ROT_TEXT ), TRUE );
    }
    if( pdidoi->guidType == GUID_RyAxis ) {
        EnableWindow( GetDlgItem( hDlg, IDC_Y_ROT ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_Y_ROT_TEXT ), TRUE );
    }
    if( pdidoi->guidType == GUID_RzAxis ) {
        EnableWindow( GetDlgItem( hDlg, IDC_Z_ROT ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_Z_ROT_TEXT ), TRUE );
    }
    if( pdidoi->guidType == GUID_Slider ) {
        switch( nSliderCount++ ) {
            case 0 :
                EnableWindow( GetDlgItem( hDlg, IDC_SLIDER0 ), TRUE );
                EnableWindow( GetDlgItem( hDlg, IDC_SLIDER0_TEXT ), TRUE );
                break;

            case 1 :
                EnableWindow( GetDlgItem( hDlg, IDC_SLIDER1 ), TRUE );
                EnableWindow( GetDlgItem( hDlg, IDC_SLIDER1_TEXT ), TRUE );
                break;
        }
    }
    if( pdidoi->guidType == GUID_POV ) {
        switch( nPOVCount++ ) {
            case 0 :
                EnableWindow( GetDlgItem( hDlg, IDC_POV0 ), TRUE );
                EnableWindow( GetDlgItem( hDlg, IDC_POV0_TEXT ), TRUE );
                break;

            case 1 :
                EnableWindow( GetDlgItem( hDlg, IDC_POV1 ), TRUE );
                EnableWindow( GetDlgItem( hDlg, IDC_POV1_TEXT ), TRUE );
                break;

            case 2 :
                EnableWindow( GetDlgItem( hDlg, IDC_POV2 ), TRUE );
                EnableWindow( GetDlgItem( hDlg, IDC_POV2_TEXT ), TRUE );
                break;

            case 3 :
                EnableWindow( GetDlgItem( hDlg, IDC_POV3 ), TRUE );
                EnableWindow( GetDlgItem( hDlg, IDC_POV3_TEXT ), TRUE );
                break;
        }
    }
*/
    return DIENUM_CONTINUE;
}

HRESULT Saitek_X52::UpdateInputState( HWND hDlg ) {
    HRESULT hr;
    TCHAR strText[512] = {0}; // Device state text
    DIJOYSTATE2 js;           // DInput joystick state 

    if( !g_pJoystick )
        return S_OK;

    // Poll the device to read the current state
    hr = g_pJoystick->Poll();
    if( FAILED( hr ) ) {
        // DInput is telling us that the input stream has been
        // interrupted. We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done. We
        // just re-acquire and try again.
        hr = g_pJoystick->Acquire();
        while( hr == DIERR_INPUTLOST )
            hr = g_pJoystick->Acquire();

        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
        return S_OK;
    }

    // Get the input's device state
    if( FAILED( hr = g_pJoystick->GetDeviceState( sizeof( DIJOYSTATE2 ), &js ) ) )
        return hr; // The device should have been acquired during the Poll()

    // Display joystick state to dialog

    // Axes
    _stprintf_s( strText, 512, TEXT( "%ld" ), js.lX );
    SetWindowText( GetDlgItem( hDlg, IDC_X_AXIS ), strText );
    _stprintf_s( strText, 512, TEXT( "%ld" ), js.lY );
    SetWindowText( GetDlgItem( hDlg, IDC_Y_AXIS ), strText );
    _stprintf_s( strText, 512, TEXT( "%ld" ), js.lZ );
    SetWindowText( GetDlgItem( hDlg, IDC_Z_AXIS ), strText );
    _stprintf_s( strText, 512, TEXT( "%ld" ), js.lRx );
    SetWindowText( GetDlgItem( hDlg, IDC_X_ROT ), strText );
    _stprintf_s( strText, 512, TEXT( "%ld" ), js.lRy );
    SetWindowText( GetDlgItem( hDlg, IDC_Y_ROT ), strText );
    _stprintf_s( strText, 512, TEXT( "%ld" ), js.lRz );
    SetWindowText( GetDlgItem( hDlg, IDC_Z_ROT ), strText );

    // Slider controls
    _stprintf_s( strText, 512, TEXT( "%ld" ), js.rglSlider[0] );
    SetWindowText( GetDlgItem( hDlg, IDC_SLIDER0 ), strText );
    _stprintf_s( strText, 512, TEXT( "%ld" ), js.rglSlider[1] );
    SetWindowText( GetDlgItem( hDlg, IDC_SLIDER1 ), strText );

    // Points of view
    _stprintf_s( strText, 512, TEXT( "%lu" ), js.rgdwPOV[0] );
    SetWindowText( GetDlgItem( hDlg, IDC_POV0 ), strText );
    _stprintf_s( strText, 512, TEXT( "%lu" ), js.rgdwPOV[1] );
    SetWindowText( GetDlgItem( hDlg, IDC_POV1 ), strText );
    _stprintf_s( strText, 512, TEXT( "%lu" ), js.rgdwPOV[2] );
    SetWindowText( GetDlgItem( hDlg, IDC_POV2 ), strText );
    _stprintf_s( strText, 512, TEXT( "%lu" ), js.rgdwPOV[3] );
    SetWindowText( GetDlgItem( hDlg, IDC_POV3 ), strText );


    // Fill up text with which buttons are pressed
    _tcscpy_s( strText, 512, TEXT( "" ) );
    for( int i = 0; i < 128; i++ ) {
        if( js.rgbButtons[i] & 0x80 ) {
            TCHAR sz[128];
            _stprintf_s( sz, 128, TEXT( "%02d " ), i );
            _tcscat_s( strText, 512, sz );
        }
    }

    SetWindowText( GetDlgItem( hDlg, IDC_BUTTONS ), strText );

    return S_OK;
}

VOID Saitek_X52::FreeDirectInput() {
    // Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
    if( g_pJoystick )
        g_pJoystick->Unacquire();

    // Release any DirectInput objects.
    SAFE_RELEASE( g_pJoystick );
    SAFE_RELEASE( g_pDI );
}

// Update button positions and sliders
void Saitek_X52::update() {
    UpdateInputState(hDlg);
}

// Main method for Client testing
// Sets up intial config and begins ouputing to the terminal
// Rename to main if compiling only this file
int main() {
    // No print buffering
    setvbuf (stdout, NULL, _IONBF, 0);

	// Create joystick object
	printf("Creating joystick... ");
	Saitek_X52* s_X52 = new Saitek_X52();
	printf("Done!\n");

	do {
		s_X52->update();
		delay(10000);
	} while(true);

	return 0;
}