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

		//Describes font stated, that can be changed
		enum StateFontFlags:int64_t {fontSize = 0x1, fontFamily = 0x2};
		int64_t m_changedFontStates = ~(int64_t)0; //Show, that state of the text layout was chanded (add new config params)

		void ChangeFontState();
		void CreateNewTextFormat();
		
	public:
		FontD2D(const std::wstring& fontFamily, IDWriteFontCollection* fontCollection, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch,
			FLOAT fontSize, const std::wstring& localName);
		FontD2D(const std::wstring& fontFamily, FLOAT fontSize);

		void SetSizeInPt(int sizeInPt) override;
		void SetSizeInPixels(int sizeInPixels) override;
		void SetFamily(std::wstring familyName) override;

		FLOAT GetTextMaxHeight(const std::wstring& text, FLOAT maxWidth) override;

		//This function is called every time, when user want to draw text layout
		ComPtr<IDWriteTextLayout> GetFormattedTextLayout(const std::wstring text, D2D_RECT_F textRect);

	private:
		std::wstring m_fontFamily;
		std::wstring m_fontLocalName;
		IDWriteFontCollection* m_fontCollection;
		DWRITE_FONT_WEIGHT m_fontWeight;
		DWRITE_FONT_STYLE m_fontStyle;
		DWRITE_FONT_STRETCH m_fontStretch;
		FLOAT m_fontSize; //In pixels
	};

}