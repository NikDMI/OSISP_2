#include <Windows.h>
#include <string>
#include "Font/FontD2D.h"
#include "Painter/PainterD2D.h"

//using namespace LAB2;

const std::wstring CLASS_NAME = L"WinClass1";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

using namespace LAB2;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lCmdLine, int nCmdShow) {
	//InitApplication();
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME.c_str();
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = wc.hIcon;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	RegisterClassEx(&wc);
	HWND hWnd = CreateWindow(CLASS_NAME.c_str(), L"OSISP_2", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, 0);
	ShowWindow(hWnd, SW_SHOW);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);

	}
	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static Painter* painter;
	static LAB2::IFont* defaultFont;
	switch (uMsg) {

	case WM_CREATE:
		painter = new PainterD2D{hWnd};
		defaultFont = painter->CreateIFontObject();
		break;

	case WM_SIZE:
		painter->Resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_PAINT:
		painter->StartDraw();
		painter->SetFontObject(defaultFont);
		painter->DrawTextLayout(L"Hello", { 0,0,100,100 });
		painter->EndDraw();
		break;

	case WM_ERASEBKGND:
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

