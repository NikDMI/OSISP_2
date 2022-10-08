#pragma once
#include "Painter.h"


namespace LAB2 {

	class FontD2D;

	class PainterD2D final : public Painter {
	private:
		HWND m_hWnd;
		ComPtr<ID2D1HwndRenderTarget> m_renderTarget;

		static BITMAP_HANDLE max_free_handle;
		static std::map<BITMAP_HANDLE, ComPtr<ID2D1Bitmap>> loadedImages;

		ComPtr<ID2D1SolidColorBrush> m_brush;
		ComPtr<ID2D1SolidColorBrush> m_textBrushDefault;
		FontD2D* m_currentFont = nullptr;
		D2D1_COLOR_F m_windowColor;

		bool m_isSetClipRect = false;
	public:
		PainterD2D(HWND hWnd);
		//System methods
		void StartDraw() override;
		void EndDraw() override;
		void SetClipRect(const D2D_RECT_F& clipRect) override;
		void SetNormalClipRect() override;

		//View methods
		void SetBrushColor(COLOR color) override;
		void SetDefaultTextColor(COLOR color) override;
		void SetFontObject(IFont* font) override;

		IFont* GetCurrentFontObject() override;

		//Draw methods
		void Rectangle(RECT rect) override;
		void Line(D2D1_POINT_2F p1, D2D1_POINT_2F p2, UINT width) override;
		void DrawImage(BITMAP_HANDLE bmpIndex, RECT distRect) override;
		void DrawTextLayout(const std::wstring& text, D2D1_RECT_F layoutRect, INT xTextOffset = 0, INT yTextOffset = 0) override;

		//Load resource methods
		BITMAP_HANDLE LoadImageFromFile(std::wstring fileName) override;
		IFont* CreateIFontObject() override;

		//Config methods
		void Resize(uint32_t width, uint32_t height) override;
		void InvalidateDrawArea() override;
	};

}