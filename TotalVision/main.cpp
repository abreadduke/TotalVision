#include <windows.h>
#include <iostream>

#define WINDOW_NAME L"TotalVision"
const int xWindowStartPos = 200;
const int yWindowStartPos = 200;
const int windowStartWidth = 200;
const int windowStartHeigh = 200;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

WNDCLASS createMainWindowClass(const WCHAR * classname, HINSTANCE instance, WNDPROC wproc) {
	WNDCLASS wclass = { 0 };
	wclass.lpszClassName = classname;
	wclass.hInstance = instance;
	wclass.lpfnWndProc = wproc;
	RegisterClass(&wclass);
	return wclass;
}
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmd, int cmdshow) {
	WNDCLASS windowclass = createMainWindowClass(L"totalvision", hInstance, WindowProc);
	HWND window = CreateWindowW(windowclass.lpszClassName, WINDOW_NAME, 
		WS_OVERLAPPEDWINDOW, xWindowStartPos, yWindowStartPos, windowStartWidth, windowStartHeigh, NULL, NULL, hInstance, NULL);
	//if (window == NULL) {
	//	throw std::runtime_error("Can't create the window");
	//}
	ShowWindow(window, cmdshow);
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return 0;
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}