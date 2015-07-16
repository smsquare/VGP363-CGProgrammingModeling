// Main.cpp
//////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "libEGL.lib")
#pragma comment(lib, "libGLESv2.lib")
#pragma comment(lib, "preprocessor.lib")
#pragma comment(lib, "translator_lib.lib")

#include <Windows.h>

//////////////////////////////
// High end machines		//
//#include <GLES3\gl3.h>	//
//#include <GLES3\gl3ext.h>	//
//////////////////////////////

#include <GLES2\gl2.h>
#include <GLES2\gl2ext.h>

#include <EGL\egl.h>
#include <EGL\eglext.h>

#include "Utility.Debug.h"

EGLDisplay mDisplay = 0;
EGLNativeWindowType mNativeWindow = 0;
EGLNativeDisplayType mNativeDisplay = 0;
bool mAppRunning = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == WM_CLOSE) {
		mAppRunning = false;
	}
	
	return DefWindowProcA(hWnd, message, wParam, lParam);
}

bool OnInitInstance() {
	WNDCLASSEXA wcex = {0};
	wcex.cbSize = sizeof(WNDCLASSEXA);
	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursorA(NULL, IDC_ARROW);
	wcex.hbrBackground = 0;
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = "VGPProjectWndClass";

	//bool result = RegisterClassExA(&wcex);

	if (!RegisterClassExA(&wcex)) {
		// Exit function
		CDebug::ErrorLog("ERROR: Window registration failed!");
		return false;
	}
	CDebug::MsgLog("Window registration success.");

	DWORD windowStyle = WS_VISIBLE| WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_SYSMENU;

	mNativeWindow = CreateWindowExA(
		WS_EX_APPWINDOW, 
		wcex.lpszClassName, 
		"Main Window", 
		windowStyle,
		CW_USEDEFAULT,			// x
		CW_USEDEFAULT,			// y
		1280,					// Width
		728,					// Height
		0,						// Parent Window
		0,						// Menu Handle
		GetModuleHandle(NULL),	// App Instance
		0						// System Value
	);

	ShowWindow(mNativeWindow, SW_SHOW);

	mNativeDisplay = GetDC(mNativeWindow);	
	if (!mNativeDisplay) {
		CDebug::ErrorLog("Basic windows graphical device context couldn't be created for the window.");
		return false;
	}

	EGLDisplay mDisplay = eglGetDisplay(mNativeDisplay);
	if (mDisplay == EGL_NO_DISPLAY) {
		CDebug::ErrorLog("OpenGL|ES context couldn't be created!");
		return false;
	}

	EGLint majorVersion, minorVersion;
	if (!eglInitialize(mDisplay, &majorVersion, &minorVersion)) {
		CDebug::ErrorLog("OpenGL|ES context couldn't be created!");
		return false;
	}

	eglBindAPI(EGL_OPENGL_ES_API);

	if (eglGetError() != EGL_SUCCESS) {
		CDebug::ErrorLog("OpenGL|ES context couldn't be created!");
		return false;
	}

	mAppRunning = true;
	return true;
}

void OnInitialize() {

}

void OnFrame() {

}

int main(int argc, char** argv) {

	if (!OnInitInstance()) {
		return 1;
	}

	OnInitialize();

	while(mAppRunning == true) {
		MSG message = {0};
		bool result = PeekMessage(&message, 0, 0, 0, PM_REMOVE);

		if (result == true) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		OnFrame();
	}

	system("pause");
	return 0;
}