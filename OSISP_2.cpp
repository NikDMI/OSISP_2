#include <Windows.h>
#include <string>
#include "Font/FontD2D.h"
#include "Painter/PainterD2D.h"
#include "Table/Table.h"
#include "TableParser/TableParser.h"
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

	FLOAT m_countedTableWidth;
	FLOAT m_tableWidth;

	TableConfig(Table* table, const D2D_RECT_F& tableRect) :m_table{ table }, m_tableRect{tableRect} {
		m_tableWidth = tableRect.right - tableRect.left;
		m_countedTableWidth = table->GetCountedTableWidth(tableRect);
	}


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
		m_tableWidth = tableRect.right - tableRect.left;
		CorrectTableOffsetX();
	}

	void ScrollTableByY(FLOAT dy) {
		m_tableOffsetY += dy;
		if (m_tableOffsetY < epsilon) m_tableOffsetY = 0.0f;
	}

	void ScrollTableByX(FLOAT dx) {
		m_tableOffsetX += dx;
		CorrectTableOffsetX();
	}

	void CorrectTableOffsetX() {
		//Correct table offset
		m_countedTableWidth = m_table->GetCountedTableWidth(m_tableRect);
		if (m_countedTableWidth < epsilon) return;
		if (m_tableOffsetX < epsilon) {
			if (-m_tableOffsetX + m_tableWidth > m_countedTableWidth) {
				m_tableOffsetX = -(m_countedTableWidth - m_tableWidth);
			}
		}
		else {
			m_tableOffsetX = 0.0f;
		}
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

	case WM_MOUSEHWHEEL:
		wheelCount = ((double)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA) * WHEEL_SENSETIVE;
		tableConfig->ScrollTableByX(-wheelCount);
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

		case  ID_MENU_SAVE_TABLE_DATA:
			TableParser::WriteDataContentIntoFile(L"OutputFile.txt", table);
			break;

		case ID_MENU_LOAD_TABLE_DATA:
			TableParser::ParseFileIntoTable(L"OutputFile.txt", table);
			tableConfig->CorrectTableOffsetX();
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

