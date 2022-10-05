#pragma once
#include "../Config.h"
#include <wrl.h>
#include <d2d1.h>
#include <map>
#include <string>
#include "../Font/IFont.h"

namespace LAB2 {

	using Microsoft::WRL::ComPtr;

	using COLOR = D2D1_COLOR_F;
	using BITMAP_HANDLE = int;
	const BITMAP_HANDLE INVALID_BITMAP_HANDLE = -1;

	/// <summary>
	/// Abstraction to paint device
	/// </summary>
	class Painter {
	public:
		virtual void StartDraw() = 0;
		virtual void EndDraw() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual BITMAP_HANDLE LoadImageFromFile(std::wstring fileName) = 0;
		virtual IFont* CreateIFontObject() = 0;

		virtual void SetBrushColor(COLOR color) = 0;
		virtual void SetDefaultTextColor(COLOR color) = 0;
		virtual void SetFontObject(IFont* font) = 0;

		virtual IFont* GetCurrentFontObject() = 0;

		virtual void Rectangle(RECT rect) = 0;
		virtual void DrawImage(BITMAP_HANDLE bmpIndex, RECT distRect) = 0;
		virtual void DrawTextLayout(const std::wstring& text, RECT layoutRect, INT xTextOffset = 0, INT yTextOffset = 0) = 0;
		virtual void InvalidateDrawArea() = 0;
	};


	class BadArgumentsPainterException : public Exception {
	public:
		BadArgumentsPainterException() :Exception(L"Painter method get bad args") {}
	};
}