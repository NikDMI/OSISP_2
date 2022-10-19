#pragma once
#include "../Config.h"
#include "../Painter/Painter.h"
#include <string>
#include <vector>
#include <d2d1.h>

namespace LAB2 {

	class Table final {
	private:
		const unsigned int MAX_ROW_COUNT = 30;
		const unsigned int MAX_COL_COUNT = 30;
		const FLOAT HLINE_PIXEL_WIDTH = 7; //Line width in DIP pixels
		const FLOAT VLINE_PIXEL_WIDTH = 7; //Line h in DIP pixels
		unsigned int m_rowCount = 0;
		unsigned int m_colCount = 0;
		std::vector<std::vector<std::wstring>> m_table{ MAX_ROW_COUNT,std::vector<std::wstring>{MAX_COL_COUNT} };

		Painter* m_painter;
		COLOR m_tableColor;
		COLOR m_lineColor;

		friend class TableParser;

	public:
		Table(Painter* painter, COLOR tableColor, COLOR lineColor = {0.0f, 0.0f, 0.0f, 1.0f});

		//Set text to specific ceil
		void SetText(unsigned int row, unsigned int column, const std::wstring& text);

		void ClearTable();

		//ShowTable
		//x/yOffset - offset of the table from (0;0) point - like a push
		FLOAT ShowHorizontalLinearTable(D2D_RECT_F tableRectF, FLOAT xOffsetInPixels, FLOAT yOffsetInPixels);
	};

	class OutOfTableRangeException : public Exception {
	public:
		OutOfTableRangeException() :Exception(L"Invalid col or row index") {};
	};
}