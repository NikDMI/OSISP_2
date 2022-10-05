#pragma once
#include "IFont.h"
#include <wrl.h>
#include <dwrite.h>
#include <d2d1.h>


namespace LAB2 {

	using Microsoft::WRL::ComPtr;

	class FontD2D : public IFont {
	private:
		static bool _isDWriteFactoryCreate;//Must create throught pattern singelton
		static ComPtr<IDWriteFactory> _pWriteFactory;

		ComPtr<IDWriteTextFormat> m_textFormat; //Represent default text format
		//ComPtr<IDWriteTextLayout> m_textLayout; //Represent last used text layout

		//Describes font stated, that can be changed
		//enum StateFontFlags:int64_t {textRectangle = 0x1};
		//int64_t m_changedFontStates = ~(int64_t)0; //Show, that state of the text layout was chanded (add new config params)
		
		//ComPtr<IDWriteTextLayout> m_textLayout; //This object is used for formatted texts
		
	public:
		FontD2D(const std::wstring& fontFamily, IDWriteFontCollection* fontCollection, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch,
			FLOAT fontSize, const std::wstring& localName);
		FontD2D(const std::wstring& fontFamily, FLOAT fontSize);

		void SetSizeInPt(int sizeInPt) override;
		void SetSizeInPixels(int sizeInPixels) override;
		void SetFamily(std::wstring familyName) override;

		void SetMaxTextWidth(FLOAT w) override;
		void SetMaxTextHeight(FLOAT h) override;

		ComPtr<IDWriteTextLayout> GetFormattedTextLayout(const std::wstring text);
	};

}