#include "Table.h"

namespace LAB2 {

	Table::Table(Painter* painter, COLOR tableColor, COLOR lineColor) :m_painter{painter}, 
		m_lineColor{ lineColor }, m_tableColor{tableColor}
	{

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

	void Table::ClearTable() {
		for (int i = 0; i < m_rowCount; ++i) {
			for (int j = 0; j < m_colCount; ++j) {
				m_table[i][j] = L"";
			}
		}
		m_colCount = 0;
		m_rowCount = 0;
	}

	FLOAT Table::GetCountedTableWidth(D2D_RECT_F tableRectF) {
		if (m_colCount == 0) return 0;
		FLOAT tableWidth = tableRectF.right - tableRectF.left;
		FLOAT columnPixelWidth = (FLOAT)(tableWidth - VLINE_PIXEL_WIDTH * (m_colCount + 1)) / m_colCount;//Width of text area
		if (columnPixelWidth < MIN_CEILL_WIDTH) {
			return MIN_CEILL_WIDTH * m_colCount + (m_colCount+1) * VLINE_PIXEL_WIDTH;
		}
		return tableWidth;
	}

	FLOAT Table::ShowHorizontalLinearTable(D2D_RECT_F tableRectF, FLOAT xOffsetInPixels, FLOAT yOffsetInPixels) {
		if (m_colCount == 0) return 0;
		FLOAT tableWidth = tableRectF.right - tableRectF.left;
		FLOAT tableHeight = tableRectF.bottom - tableRectF.top;
		FLOAT columnPixelWidth = (FLOAT)(tableWidth - VLINE_PIXEL_WIDTH * (m_colCount + 1)) / m_colCount;//Width of text area
		if (columnPixelWidth < MIN_CEILL_WIDTH) { 
			columnPixelWidth = MIN_CEILL_WIDTH;
			tableWidth = MIN_CEILL_WIDTH * m_colCount + (m_colCount + 1) * VLINE_PIXEL_WIDTH;
		}

		FLOAT startYOffset = yOffsetInPixels;
		IFont* currentPainterFont = m_painter->GetCurrentFontObject();
		if (currentPainterFont != nullptr) {
			m_painter->SetClipRect(tableRectF);
			m_painter->SetBrushColor(m_tableColor);
			m_painter->Rectangle(tableRectF);
			m_painter->SetBrushColor(m_lineColor);
			FLOAT currentYCoords = HLINE_PIXEL_WIDTH / 2; //Abstract start of the table in local coordinates
			yOffsetInPixels -= HLINE_PIXEL_WIDTH;
			int drawedRow = 0;
			while (yOffsetInPixels + tableHeight > 0 && drawedRow < m_rowCount) {//Does we not fill all visible area with owr table

				FLOAT maxRowHeight = 0.0f;
				for (int j = 0; j < m_colCount; ++j) {
					FLOAT height;
					if (maxRowHeight < (height = currentPainterFont->GetTextMaxHeight(m_table[drawedRow][j], columnPixelWidth))) {
						maxRowHeight = height;
					}
				}
				yOffsetInPixels -= maxRowHeight; //This var means: how much pixels we drawn with - sigh (-20 => 20 pixels was drawn)
				currentYCoords += HLINE_PIXEL_WIDTH / 2; //TopLeft of the table
				if (yOffsetInPixels < 0) { //If ceils are visible
					//Count client coords of ceils
					D2D1_RECT_F layoutRect{ 0, 0, columnPixelWidth, maxRowHeight };
					//Device dependent coordinats of table ceil
					FLOAT yColumnOffsetDDP = currentYCoords - startYOffset + tableRectF.top;
					FLOAT xColumnOffsetDDP = xOffsetInPixels + VLINE_PIXEL_WIDTH + tableRectF.left;

					//Draw horizontal line (up)
					m_painter->Line({ xColumnOffsetDDP - VLINE_PIXEL_WIDTH, (yColumnOffsetDDP - HLINE_PIXEL_WIDTH / 2) },
						{ xColumnOffsetDDP - VLINE_PIXEL_WIDTH + tableWidth, (yColumnOffsetDDP - HLINE_PIXEL_WIDTH / 2) }, HLINE_PIXEL_WIDTH);
					
					//Draw ceils
					for (int j = 0; j < m_colCount; ++j) {
						m_painter->DrawTextLayout(m_table[drawedRow][j], layoutRect, xColumnOffsetDDP, yColumnOffsetDDP);
						//Draw vertical line (left)
						m_painter->Line({ (FLOAT)(xColumnOffsetDDP - VLINE_PIXEL_WIDTH / 2), (yColumnOffsetDDP) },
							{ (FLOAT)(xColumnOffsetDDP - VLINE_PIXEL_WIDTH / 2), (yColumnOffsetDDP + maxRowHeight) }, VLINE_PIXEL_WIDTH);
						xColumnOffsetDDP += columnPixelWidth + VLINE_PIXEL_WIDTH;
					}
					//Draw right vertical line
					m_painter->Line({ (FLOAT)(xColumnOffsetDDP - VLINE_PIXEL_WIDTH / 2), (yColumnOffsetDDP) },
						{ (FLOAT)(xColumnOffsetDDP - VLINE_PIXEL_WIDTH / 2), (yColumnOffsetDDP + maxRowHeight) }, VLINE_PIXEL_WIDTH);
				}
				currentYCoords += maxRowHeight + HLINE_PIXEL_WIDTH / 2;
				yOffsetInPixels -= HLINE_PIXEL_WIDTH;
				++drawedRow;
			}
			
			{//fill all visible table
				m_painter->Line({ xOffsetInPixels + tableRectF.left, (currentYCoords - startYOffset + tableRectF.top) },
					{ (xOffsetInPixels + tableRectF.right), (currentYCoords - startYOffset + tableRectF.top) }, HLINE_PIXEL_WIDTH);
			}
			m_painter->SetNormalClipRect();
			FLOAT tableFinalOffset = yOffsetInPixels + tableHeight;
			return tableFinalOffset; //Return free space, when table was drawn (>0 - all table wasn't drawn, <0 - table not inluded)
		}
	}

}