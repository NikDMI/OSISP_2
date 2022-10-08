#include <Windows.h>
#include <string>
#include "Font/FontD2D.h"
#include "Painter/PainterD2D.h"
#include "Table/Table.h"
#include "resource.h"
#include <commdlg.h>

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
	SetMenu(hWnd, (HMENU)LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MAINMENU)));
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
	static Table* table;
	FLOAT h;
	RECT clientRect;
	static CHOOSEFONT chooseFont;
	static LOGFONT logFont;
	switch (uMsg) {

	case WM_CREATE:
		painter = new PainterD2D{hWnd};
		defaultFont = painter->CreateIFontObject();
		defaultFont->SetSizeInPixels(30);
		table = new Table{ painter };
		table->SetText(0, 0, L"Hello");
		table->SetText(0, 1, L"Hello2");
		table->SetText(1, 1, L"HI2");
		table->SetText(2, 2, L"HI2HH");
		table->SetText(3, 2, L"No no no no no no");
		table->SetText(3, 10, L"Nikita hello, how are you&");
		break;

	case WM_SIZE:
		painter->Resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_PAINT:
		GetClientRect(hWnd, &clientRect);
		painter->StartDraw();
		painter->SetFontObject(defaultFont);
		table->ShowHorizontalLinearTable({ 10,30,(FLOAT)clientRect.right - 30, (FLOAT)clientRect.bottom - 30 }, 45, 25);
		painter->EndDraw();
		break;

	case WM_ERASEBKGND:
		break;

	case WM_COMMAND://Menu events
		switch (LOWORD(wParam)) {//Switch by ID

		case ID_MENU_SET_FONT:
			chooseFont.lStructSize = sizeof(chooseFont);
			chooseFont.hwndOwner = hWnd;
			chooseFont.lpLogFont = &logFont;
			chooseFont.Flags = CF_INITTOLOGFONTSTRUCT;
			ChooseFont(&chooseFont);
			defaultFont->SetSizeInPixels(abs(logFont.lfHeight));
			defaultFont->SetFamily(logFont.lfFaceName);
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

