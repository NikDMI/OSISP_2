#include "Table.h"

namespace LAB2 {

	Table::Table(Painter* painter) :m_painter{painter} {

	}

	void Table::SetText(unsigned int row, unsigned int column, const std::wstring& text) {
		if (row < MAX_ROW_COUNT && column < MAX_COL_COUNT) {
			m_table[row][column] = text;
			if (m_rowCount <= row) m_rowCount = row+1;
			if (m_colCount <= column) m_colCount = column+1;
		}
		else {
			throw OutOfTableRangeException{};
		}
	}

	void Table::ShowHorizontalLinearTable(unsigned int tableWidth, unsigned int tableHeight, FLOAT yOffsetInPixels) {
		FLOAT columnPixelWidth = (FLOAT)tableWidth / m_colCount;
		IFont* currentPainterFont = m_painter->GetCurrentFontObject();
		if (currentPainterFont != nullptr) {
			FLOAT currentYCoords = 0.0f;
			for (int i = 0; i < m_rowCount; ++i) {
				FLOAT maxRowHeight = 0.0f;
				for (int j = 0; j < m_colCount; ++j) {
					FLOAT height;
					if (maxRowHeight < (height=currentPainterFont->GetTextMaxHeight(m_table[i][j], columnPixelWidth))) {
						maxRowHeight = height;
					}
				}
				yOffsetInPixels -= maxRowHeight;
				if (yOffsetInPixels < 0) {
					RECT layoutRect{ 0,currentYCoords,columnPixelWidth,currentYCoords + maxRowHeight };
					FLOAT yColumnOffset = currentYCoords + yOffsetInPixels + maxRowHeight;
					for (int j = 0; j < m_colCount; ++j) {
						m_painter->DrawTextLayout(m_table[i][j], layoutRect, 0, yColumnOffset);
						OffsetRect(&layoutRect, columnPixelWidth, 0);
					}
				}
				currentYCoords += maxRowHeight;
			}
		}
	}

}