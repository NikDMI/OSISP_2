#pragma once
#include "../Table/Table.h"
#include <string>

namespace LAB2 {

	//Static class (need to be a friend to Table class)
	class TableParser final {
	public:
		static void ParseFileIntoTable(std::wstring fileName, Table* table);
		static void WriteDataContentIntoFile(std::wstring fileName, Table* table);

	private:
		static void GetTableCells(const std::wstring& tableData, Table* table);
	};
}