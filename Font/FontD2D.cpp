#include "FontD2D.h"

#pragma comment(lib, "dwrite.lib")

namespace LAB2 {

	bool FontD2D::_isDWriteFactoryCreate = false;
	ComPtr<IDWriteFactory> FontD2D::_pWriteFactory {};

	FontD2D::FontD2D(const std::wstring& fontFamily, FLOAT fontSize) :FontD2D{fontFamily, NULL, DWRITE_FONT_WEIGHT_NORMAL, 
	DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L""} {
		
	}

	FontD2D::FontD2D(const std::wstring& fontFamily,IDWriteFontCollection* fontCollection, DWRITE_FONT_WEIGHT weight,
		DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch, FLOAT fontSize, const std::wstring& localName):
		m_fontFamily{ fontFamily }, m_fontCollection{ fontCollection }, m_fontWeight{weight},
		m_fontStyle{ fontStyle }, m_fontStretch{ fontStretch }, m_fontSize{ fontSize }, m_fontLocalName{localName}
	{

		if (!_isDWriteFactoryCreate) {
			HRESULT hRes = DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(_pWriteFactory.GetAddressOf())
			);
			_isDWriteFactoryCreate = true;
		}
		CreateNewTextFormat();
	}

	void FontD2D::CreateNewTextFormat() {
		HRESULT hRes = _pWriteFactory->CreateTextFormat(m_fontFamily.c_str(), m_fontCollection, m_fontWeight, m_fontStyle,
			m_fontStretch, m_fontSize, m_fontLocalName.c_str(), m_textFormat.GetAddressOf());
		if (hRes != S_OK) {
			DWORD errCode = GetLastError();
			throw CreateFontException{};
		}
		m_textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_EMERGENCY_BREAK); //Breaks big words
		m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}


	void FontD2D::SetSizeInPt(int sizeInPt) {
		//m_textFormat->
	}


	void FontD2D::SetSizeInPixels(int sizeInPixels) {
		m_fontSize = sizeInPixels;
		m_changedFontStates |= StateFontFlags::fontSize;
	}


	void FontD2D::SetFamily(std::wstring familyName) {
		m_fontFamily = familyName;
		m_changedFontStates |= StateFontFlags::fontFamily;
	}

	void FontD2D::ChangeFontState() {
		if (m_changedFontStates) {
			bool isCriticalChanges = false; //This flag shows, do we need to create new textFormat object
			if (m_changedFontStates & StateFontFlags::fontSize || m_changedFontStates & StateFontFlags::fontFamily) {
				isCriticalChanges = true;
			}
	
			if (isCriticalChanges) 
				CreateNewTextFormat();
			m_changedFontStates = 0;
		}
	}

	

	ComPtr<IDWriteTextLayout> FontD2D::GetFormattedTextLayout(const std::wstring text, D2D_RECT_F textRect) {
		ChangeFontState(); //Create new textFormat object if user make some changes
		ComPtr<IDWriteTextLayout> textLayout;
		FLOAT maxWidth = textRect.right - textRect.left;
		FLOAT maxHeight = textRect.bottom - textRect.top;
		HRESULT hRes = _pWriteFactory->CreateTextLayout(text.c_str(), text.size(), m_textFormat.Get(),
			maxWidth, maxHeight, textLayout.GetAddressOf());
		if (hRes != S_OK)
			throw CreateFontException{};
		return textLayout;
	}

	FLOAT FontD2D::GetTextMaxHeight(const std::wstring& text, FLOAT maxWidth) {
		ChangeFontState(); //Create new textFormat object if user make some changes
		ComPtr<IDWriteTextLayout> textLayout;
		HRESULT hRes = _pWriteFactory->CreateTextLayout(text.c_str(), text.size(), m_textFormat.Get(),
			maxWidth, 0, textLayout.GetAddressOf());
		if (hRes != S_OK)
			throw CreateFontException{};
		DWRITE_TEXT_METRICS textMetrix;
		textLayout->GetMetrics(&textMetrix);
		return textMetrix.height;
	}

}