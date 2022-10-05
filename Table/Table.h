#pragma once
#include "../Config.h"
#include "../Painter/Painter.h"
#include <string>
#include <vector>

namespace LAB2 {

	class Table final {
	private:
		const unsigned int MAX_ROW_COUNT = 30;
		const unsigned int MAX_COL_COUNT = 30;
		unsigned int m_rowCount = 0;
		unsigned int m_colCount = 0;
		std::vector<std::vector<std::wstring>> m_table{ MAX_ROW_COUNT,std::vector<std::wstring>{MAX_COL_COUNT} };

		Painter* m_painter;

	public:
		Table(Painter* painter);
		//Set text to specific ceil
		void SetText(unsigned int row, unsigned int column, const std::wstring& text);
		//ShowTable
		void ShowHorizontalLinearTable(unsigned int tableWidth, unsigned int tableHeight, FLOAT yOffsetInPixels);
	};

	class OutOfTableRangeException : public Exception {
	public:
		OutOfTableRangeException() :Exception(L"Invalid col or row index") {};
	};
}