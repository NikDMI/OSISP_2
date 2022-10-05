#pragma once
#include "../Config.h"

namespace LAB2 {

	class IFont {
	protected:
		//Boundering text region
		//FLOAT m_maxTextWidth = 100;
		//FLOAT m_maxTextHeight = 100;
	public:
		virtual void SetSizeInPt(int sizeInPt) = 0;
		virtual void SetSizeInPixels(int sizeInPixels) = 0;
		virtual void SetFamily(std::wstring familyName) = 0;

		virtual FLOAT GetTextMaxHeight(const std::wstring& text, FLOAT maxWidth) = 0;

		//virtual void SetMaxTextWidth(FLOAT w) { m_maxTextWidth = w; };
		//virtual void SetMaxTextHeight(FLOAT h) { m_maxTextHeight = h; };
	};

	class CreateFontException : public Exception {
	public:
		CreateFontException() :Exception(L"Creatinon of IFont was failed") {};
	};

}