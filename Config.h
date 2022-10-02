#pragma once
#include <string>
#include <ctime>
#include <Windows.h>

namespace LAB2 {

	class Exception {
	protected:
		std::wstring m_ExceptionDesc;
	public:
		Exception(std::wstring exceptionInfo) :m_ExceptionDesc{ exceptionInfo } {}
		void ShowDebug() {
			MessageBox(NULL, m_ExceptionDesc.c_str(), L"DebugInfo", MB_OK | MB_ICONERROR);
		}
	};

	class ExceptionBadArgs :public Exception {
	public:
		ExceptionBadArgs() :Exception{ L"Invalid args" } {}
	};

	inline void InitApplication() {
		srand(time(NULL));
	}
}