// main.cpp
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

#include "main.h"
#include "APP.Main.h"

using namespace CORE;

EGLDisplay mDisplay = 0;
EGLNativeWindowType mNativeWindow = 0;
EGLNativeDisplayType mNativeDisplay = 0;
bool mAppRunning = false;

EGLConfig mConfig = 0;
EGLSurface mSurface = 0;
EGLContext mContext = 0;


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
		CDebug::ErrorLog("ERROR: Window registration failed\n");
		return false;
	}
	CDebug::MsgLog("Window registration success.\n");

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
		CDebug::ErrorLog("Basic windows graphical device context couldn't be created for the window.\n");
		return false;
	}

	mDisplay = eglGetDisplay(mNativeDisplay);
	if (mDisplay == EGL_NO_DISPLAY) {
		CDebug::ErrorLog("OpenGL|ES context couldn't be created!\n");
		return false;
	}

	EGLint majorVersion, minorVersion;
	if (!eglInitialize(mDisplay, &majorVersion, &minorVersion)) {
		CDebug::ErrorLog("OpenGL|ES context couldn't be created!\n");
		return false;
	}

	eglBindAPI(EGL_OPENGL_ES_API);

	if (eglGetError() != EGL_SUCCESS) {
		CDebug::ErrorLog("OpenGL|ES context couldn't be created!\n");
		return false;
	}

	mAppRunning = true;
	return true;
}

bool GL_Initialize() {
	// Configure EGL
	EGLint configAttributes[] = {
		EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, 
		EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 24,
		EGL_STENCIL_SIZE, 8,
		EGL_SAMPLE_BUFFERS, EGL_DONT_CARE, EGL_NONE
	};

	EGLint mConfigCount = 0;
	bool result = eglChooseConfig(mDisplay, configAttributes, &mConfig, 1, &mConfigCount);

	if (result == false || mConfigCount != 1) {
		CDebug::ErrorLog("3D Hardware Accelerator configuration could not be queried!\n");
		return false;
	}

	// Configure EGL Surface
	EGLint surfaceAttributes[] = {
		EGL_POST_SUB_BUFFER_SUPPORTED_NV, EGL_TRUE, EGL_NONE, EGL_NONE
	};
	mSurface = eglCreateWindowSurface(mDisplay, mConfig, mNativeWindow, surfaceAttributes);
	if (mSurface == EGL_NO_SURFACE || eglGetError() != EGL_SUCCESS) {
		CDebug::ErrorLog("3D Hardware Accelerator could not be bound to the specified window as configured!\n");
		return false;
	}

	// Create EGL context
	EGLint contextAttributes[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
	};
	mContext = eglCreateContext(mDisplay, mConfig, 0, contextAttributes);
	if (eglGetError() != EGL_SUCCESS) {
		CDebug::ErrorLog("An OpenGL|ES Context could not be created with the version specified!\n");
		return false;
	}

	// Make display current
	eglMakeCurrent(mDisplay, mSurface, mSurface, mContext);
	if (eglGetError() != EGL_SUCCESS) {
		CDebug::ErrorLog("The specified EGL rendering context could not be activated on the specified rendering surface and display!\n");
		return false;
	}

	return true;
}

void OnInitialize() {
	glClearColor(0.0, 0.3, 0.0, 1.0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(true);
	glClearDepthf(1.0f);

	APP::OnInitialize();
}

void OnFrame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	APP::OnFrame();
	eglSwapBuffers(mDisplay, mSurface);
}

void GL_Uninitialize() {
	if (mSurface != 0) {
		eglDestroySurface(mDisplay, mSurface);
		mSurface = 0;
	}
	if (mContext != 0) {
		eglDestroyContext(mDisplay, mContext);
		mContext = 0;
	}
}

void OnDestroyInstance() {
	GL_Uninitialize();

	if (mDisplay != EGL_NO_DISPLAY) {
		eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglTerminate(mDisplay);
		mDisplay = EGL_NO_DISPLAY;
	}

	if (mNativeDisplay != 0) {
		ReleaseDC(mNativeWindow, mNativeDisplay);
		mNativeDisplay = 0;
	}

	if (mNativeWindow != 0) {
		DestroyWindow(mNativeWindow);
		mNativeWindow = 0;
	}

	UnregisterClassA("VGPProjectWndClass", NULL);
}

int main(int argc, char** argv) {

	// Initialize the window and the GL components
	if (!OnInitInstance() || !GL_Initialize()) {
		return 1;
	}

	// Initialize the application
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
	OnDestroyInstance();
	return 0;
}