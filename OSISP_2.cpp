#include <Windows.h>
#include <string>
#include "Font/FontD2D.h"
#include "Painter/PainterD2D.h"
#include "Table/Table.h"
#include "resource.h"
#include <commdlg.h>

#include <regex>

//using namespace LAB2;


const std::wstring CLASS_NAME = L"WinClass1";

LAB2::COLOR windowColor { 0.7f, 0.7f, 0.7f, 1.0f };

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

using namespace LAB2;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lCmdLine, int nCmdShow) {
	
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

	std::string s = "@1_2@Some people@end@, @1_1@when confronted with a problem, @end@think "
		"\"I know, I'll use regular expressions.\" "
		"Now they have two problems.";

	std::regex word_regex("@(\\d+)_(\\d+)@(.+?)@end@");
	auto words_begin =
		std::sregex_iterator(s.begin(), s.end(), word_regex);
	auto words_end = std::sregex_iterator();

	while (words_begin != words_end) {
		auto res = *(words_begin++);
		auto subMathces = res.begin();
		while (subMathces != res.end()) {
			auto subM = subMathces->str();
			subMathces++;
		}
		auto str = res.str();
	}

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);

	}
	return 0;
}

struct TableConfig {
	Table* m_table;
	FLOAT m_tableOffsetX = 0.0f;
	FLOAT m_tableOffsetY = 0.0f;
	D2D_RECT_F m_tableRect;
	const FLOAT epsilon = 0.000001;

	TableConfig(Table* table, const D2D_RECT_F& tableRect) :m_table{ table }, m_tableRect{tableRect} {}


	void DrawTable() {
		FLOAT finalOffset = m_table->ShowHorizontalLinearTable(m_tableRect, m_tableOffsetX, m_tableOffsetY);
		if (finalOffset > epsilon) {	//Some part of table wasn't drawn
			if (m_tableOffsetY > epsilon) {
				m_tableOffsetY = (m_tableOffsetY - finalOffset > epsilon) ? m_tableOffsetY - finalOffset : 0;
				DrawTable();
			}
		}
	}

	void SetRect(const RECT& tableRect) {
		m_tableRect = { 0,0,(FLOAT)tableRect.right, (FLOAT)tableRect.bottom };
	}

	void ScrollTableByY(FLOAT dy) {
		m_tableOffsetY += dy;
		if (m_tableOffsetY < epsilon) m_tableOffsetY = 0.0f;
	}
};


const double WHEEL_SENSETIVE = 40; //number of pixels to "rotate" wheel

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static Painter* painter;
	static LAB2::IFont* defaultFont;
	static Table* table;
	static TableConfig* tableConfig;
	FLOAT h;
	RECT clientRect;
	static CHOOSEFONT chooseFont;
	static LOGFONT logFont;
	FLOAT tableFinalOffset;
	FLOAT wheelCount;
	switch (uMsg) {

	case WM_CREATE:
		painter = new PainterD2D{hWnd, windowColor };
		defaultFont = painter->CreateIFontObject();
		defaultFont->SetSizeInPixels(30);
		table = new Table{ painter, windowColor };
		GetClientRect(hWnd, &clientRect);
		tableConfig = new TableConfig(table, D2D1_RECT_F{ 0, 0, (FLOAT)clientRect.right, (FLOAT)clientRect.bottom });
		table->SetText(0, 0, L"Hello");
		table->SetText(0, 1, L"Hello2");
		table->SetText(1, 1, L"HI2");
		table->SetText(2, 2, L"HI2HH");
		table->SetText(3, 2, L"No no no no no no");
		table->SetText(3, 10, L"Nikita hello, how are you&");
		break;

	case WM_SIZE:
		painter->Resize(LOWORD(lParam), HIWORD(lParam));
		tableConfig->SetRect(RECT{ 0,0,LOWORD(lParam), HIWORD(lParam) });
		break;

	case WM_PAINT:
		GetClientRect(hWnd, &clientRect);
		painter->StartDraw();
		painter->SetFontObject(defaultFont);
		tableConfig->DrawTable();
		//tableFinalOffset = table->ShowHorizontalLinearTable({ 10,30,(FLOAT)clientRect.right - 30, (FLOAT)clientRect.bottom - 30 }, 0, 0);
		painter->EndDraw();
		break;

	case WM_ERASEBKGND:
		break;

	case WM_MOUSEWHEEL:
		wheelCount = ((double)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA) * WHEEL_SENSETIVE;
		tableConfig->ScrollTableByY(-wheelCount);
		InvalidateRect(hWnd, NULL, FALSE);
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

