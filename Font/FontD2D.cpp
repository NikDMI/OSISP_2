#include "FontD2D.h"

#pragma comment(lib, "dwrite.lib")

namespace LAB2 {

	bool FontD2D::_isDWriteFactoryCreate = false;
	ComPtr<IDWriteFactory> FontD2D::_pWriteFactory {};

	FontD2D::FontD2D(const std::wstring& fontFamily, FLOAT fontSize) :FontD2D{fontFamily, NULL, DWRITE_FONT_WEIGHT_NORMAL, 
	DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L""} {
		
	}

	FontD2D::FontD2D(const std::wstring& fontFamily,IDWriteFontCollection* fontCollection, DWRITE_FONT_WEIGHT weight,
		DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch, FLOAT fontSize, const std::wstring& localName) {

		if (!_isDWriteFactoryCreate) {
			HRESULT hRes = DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(_pWriteFactory.GetAddressOf())
			);
			_isDWriteFactoryCreate = true;
		}
		HRESULT hRes = _pWriteFactory->CreateTextFormat(fontFamily.c_str(), fontCollection, weight, fontStyle, fontStretch,
			fontSize, localName.c_str(), m_textFormat.GetAddressOf());
		if (hRes != S_OK) {
			DWORD errCode = GetLastError();
			throw CreateFontException{};

		}
	}


	void FontD2D::SetSizeInPt(int sizeInPt) {
		//m_textFormat->
	}


	void FontD2D::SetSizeInPixels(int sizeInPixels) {

	}


	void FontD2D::SetFamily(std::wstring familyName) {

	}

	void FontD2D::SetMaxTextWidth(FLOAT w) {
		IFont::SetMaxTextWidth(w);
		//m_changedFontStates |= StateFontFlags::textRectangle;
	}


	void FontD2D::SetMaxTextHeight(FLOAT h) {
		IFont::SetMaxTextHeight(h);
		//m_changedFontStates |= StateFontFlags::textRectangle;
	}

	/*
	void FontD2D::ChangeFontState() {
		if (m_changedFontStates) {

			if (m_changedFontStates & StateFontFlags::textRectangle) {
				m_textLayout->SetMaxHeight(m_maxTextHeight);
				m_textLayout->SetMaxWidth(m_maxTextWidth);//ѕока без проверки на отрицательные значени€
			}
			m_changedFontStates = 0;
		}
	}
	*/

	ComPtr<IDWriteTextLayout> FontD2D::GetFormattedTextLayout(const std::wstring text) {
		ComPtr<IDWriteTextLayout> textLayout;
		HRESULT hRes = _pWriteFactory->CreateTextLayout(text.c_str(), text.size(), m_textFormat.Get(),
			m_maxTextWidth, m_maxTextHeight, textLayout.GetAddressOf());
		if (hRes != S_OK)
			throw CreateFontException{};
		return textLayout;
	}

}