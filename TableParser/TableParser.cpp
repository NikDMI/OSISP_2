#include "TableParser.h"
#include "../Config.h"
#include <fstream>
#include <regex>

namespace LAB2 {

	void TableParser::WriteDataContentIntoFile(std::wstring fileName, Table* table) {
		std::ofstream oFile{};
		oFile.open(fileName.c_str(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
		if(!oFile) throw Exception(L"Can't create file");
		std::wstring tableContent;
		for (int i = 0; i < table->m_rowCount; ++i) {
			for (int j = 0; j < table->m_colCount; ++j) {
				if (table->m_table[i][j] != L"") {
					std::wstring columnData = L"@" + std::to_wstring(i+1) + L"_" + std::to_wstring(j+1) + L"@" +
						table->m_table[i][j] + L"@end@\n";
					tableContent += columnData;
				}
			}
		}
		if (tableContent.size()) {
			int dataLength = WideCharToMultiByte(CP_UTF8, 0, tableContent.c_str(), tableContent.size(), NULL, 0, NULL, NULL);
			char* data = new char[dataLength];
			dataLength = WideCharToMultiByte(CP_UTF8, 0, tableContent.c_str(), tableContent.size(), data, dataLength, NULL, NULL);
			if (dataLength == 0) {
				throw Exception(L"Can't create table data");
				delete[] data;
				oFile.close();
			}
			oFile.write(data, dataLength);
			if (oFile.fail()) throw Exception(L"Can't write output data");
			delete[] data;
		}
		oFile.close();
	}

	void TableParser::GetTableCells(const std::wstring& tableData, Table* table) {
		std::wregex word_regex(L"@(\\d+)_(\\d+)@(.+?)@end@");
		auto cells_begin =
			std::wsregex_iterator(tableData.begin(), tableData.end(), word_regex);
		auto cells_end = std::wsregex_iterator();

		while (cells_begin != cells_end) {
			auto cellCoding = *(cells_begin++);
			auto subMathces = cellCoding.begin();
			subMathces++;
			std::wstring rowNumber = (subMathces++)->str();
			std::wstring columnNumber = (subMathces++)->str();
			std::wstring cellText = (subMathces++)->str();
			int row = _wtoi(rowNumber.c_str());
			int col = _wtoi(columnNumber.c_str());
			if (row-- != 0 && col-- != 0) {
				try {
					table->SetText(row, col, cellText);
				}
				catch (OutOfTableRangeException e) { }
			}
		}
	}

	void TableParser::ParseFileIntoTable(std::wstring fileName, Table* table) {
		std::ifstream iFile{};
		iFile.open(fileName.c_str(), std::ios_base::in | std::ios_base::beg | std::ios_base::binary);
		if (iFile) {
			iFile.seekg(0, std::ios_base::_Seekend);
			std::streamoff fileLength = iFile.tellg();
			iFile.seekg(0, std::ios_base::_Seekbeg);
			if(!iFile) throw Exception(L"File was corrupted in the time of prosess");
			//Read file data
			char* data = new char[fileLength];
			iFile.read(data, fileLength);
			if (iFile.fail()) {
				delete data;
				throw Exception(L"File was corrupted in the time of prosess");
			}
			int textSize = MultiByteToWideChar(CP_UTF8, 0, data, fileLength, NULL, 0);
			std::wstring textData; textData.resize(textSize);
			textSize = MultiByteToWideChar(CP_UTF8, 0, data, fileLength, &textData[0], textData.capacity());
			if (textSize == 0) {
				delete data;
				iFile.close();
				throw Exception(L"File contains invalid data");
			}
			//Parse table data
			table->ClearTable();
			GetTableCells(textData, table);
			delete[] data;
			iFile.close();
		}
		else {
			throw Exception(L"File can't be open");
		}
	}

}