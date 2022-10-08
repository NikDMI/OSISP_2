#pragma once
#include "../Config.h"

namespace LAB2 {

	class IFont {
	public:
		virtual void SetSizeInPt(int sizeInPt) = 0;
		virtual void SetSizeInPixels(int sizeInPixels) = 0;
		virtual void SetFamily(std::wstring familyName) = 0;

		virtual FLOAT GetTextMaxHeight(const std::wstring& text, FLOAT maxWidth) = 0;
	};

	class CreateFontException : public Exception {
	public:
		CreateFontException() :Exception(L"Creatinon of IFont was failed") {};
	};

}