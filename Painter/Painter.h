#pragma once
#include "../Config.h"
#include <wrl.h>
#include <d2d1.h>
#include <map>
#include <string>

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

		virtual void SetBrushColor(COLOR color) = 0;
		virtual void Rectangle(RECT rect) = 0;
		virtual void DrawImage(BITMAP_HANDLE bmpIndex,RECT distRect) = 0;
		virtual void InvalidateDrawArea() = 0;
	};


	class PainterD2D1 final: public Painter  {
	private:
		HWND m_hWnd;
		ComPtr<ID2D1HwndRenderTarget> m_renderTarget;

		static BITMAP_HANDLE max_free_handle;
		static std::map<BITMAP_HANDLE, ComPtr<ID2D1Bitmap>> loadedImages;

		ComPtr<ID2D1SolidColorBrush> m_brush;
		D2D1_COLOR_F m_windowColor;
	public:
		PainterD2D1(HWND hWnd);
		void StartDraw() override;
		void EndDraw() override;
		void SetBrushColor(COLOR color) override;
		void Rectangle(RECT rect) override;
		void Resize(uint32_t width, uint32_t height) override;
		BITMAP_HANDLE LoadImageFromFile(std::wstring fileName) override;
		void DrawImage(BITMAP_HANDLE bmpIndex, RECT distRect) override;
		void InvalidateDrawArea() override;
	};
}