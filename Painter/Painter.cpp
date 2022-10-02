#include "Painter.h"
#include <d2d1.h>
#include <wincodec.h>

#pragma comment(lib, "d2d1.lib")


namespace LAB2 {
	bool isComInit = false;
	std::map<BITMAP_HANDLE, ComPtr<ID2D1Bitmap>> PainterD2D1::loadedImages;
	BITMAP_HANDLE PainterD2D1::max_free_handle;


	PainterD2D1::PainterD2D1(HWND hWnd) : m_hWnd{hWnd} {
		if (!isComInit) {
			if (CoInitialize(NULL) != S_OK) 
				throw Exception(L"Can't init COM library");
			isComInit = true;
		}
		ComPtr<ID2D1Factory> d2d1Factory;
		if(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(d2d1Factory.GetAddressOf())) != S_OK)
			throw Exception(L"D2D1Factory: can't create");

		RECT rc;GetClientRect(hWnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);
		if(d2d1Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, size),
			m_renderTarget.GetAddressOf())!=S_OK) 
				throw Exception(L"CreateHWNDRender target failed");
		m_renderTarget->SetDpi(96.0f, 96.0f);
		m_windowColor = { 0.7f, 0.7f, 0.7f, 1.0f };
	}

	void PainterD2D1::StartDraw() {
		m_renderTarget->BeginDraw();
		if (!m_brush.Get()) {
			D2D1_COLOR_F color{ 0.0f, 0.0f, 0.0f, 1.0f };
			m_renderTarget->CreateSolidColorBrush(color, m_brush.GetAddressOf());
		}
		m_renderTarget->Clear(m_windowColor);
	}

	void PainterD2D1::EndDraw() {
		m_renderTarget->EndDraw();
		ValidateRect(m_hWnd, NULL);
	}

	void PainterD2D1::SetBrushColor(COLOR color) {
		if(m_renderTarget->CreateSolidColorBrush(color, m_brush.GetAddressOf())!=S_OK)
			throw Exception(L"Create brush failed");
	}

	void PainterD2D1::Rectangle(RECT rect) {
		D2D1_RECT_F rectF{ rect.left,rect.top, rect.right,rect.bottom };
		m_renderTarget->FillRectangle(rectF, m_brush.Get());
	}

	void PainterD2D1::Resize(uint32_t width, uint32_t height) {
		D2D1_SIZE_U windowSize = { width, height };
		m_renderTarget->Resize(windowSize);
	}

	BITMAP_HANDLE PainterD2D1::LoadImageFromFile(std::wstring fileName) {
		HRESULT hr;
		ComPtr<IWICImagingFactory> WICFactory;
		hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(WICFactory.GetAddressOf()));
		if (!SUCCEEDED(hr)) {
			throw Exception(L"WICFactory: bad alloc");
		}
		ComPtr<IWICBitmapDecoder> bmpDecoder;
		hr = WICFactory->CreateDecoderFromFilename(fileName.c_str(), NULL, GENERIC_READ,
			WICDecodeMetadataCacheOnLoad, bmpDecoder.GetAddressOf());
		ComPtr<IWICBitmapFrameDecode> pSource;
		if (SUCCEEDED(hr)) {
			bmpDecoder->GetFrame(0, pSource.GetAddressOf());
		}
		else {
			
		}
		ComPtr<IWICFormatConverter> pConverter;
		hr = WICFactory->CreateFormatConverter(pConverter.GetAddressOf());
		if (SUCCEEDED(hr)) {
			hr = pConverter->Initialize(pSource.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone,
				NULL, 0, WICBitmapPaletteTypeMedianCut);
		}
		if (SUCCEEDED(hr)) {
			
			ComPtr<ID2D1Bitmap> bmp;
			hr = m_renderTarget->CreateBitmapFromWicBitmap(pConverter.Get(), NULL, bmp.GetAddressOf());
			if (SUCCEEDED(hr)) {
				loadedImages.insert({ max_free_handle, bmp });
			}
			else {
				//return NULL;
			}
			
		}
		else {
			throw Exception(L"IWICFormatConverter: bad args");
		}
		return max_free_handle++;
	}
	
	void PainterD2D1::DrawImage(BITMAP_HANDLE bmpIndex, RECT distRect) {
		auto bmpIter = loadedImages.find(bmpIndex);
		if (bmpIter != loadedImages.end()) {
			D2D1_RECT_F distRectF{ distRect.left, distRect.top, distRect.right, distRect.bottom };
			m_renderTarget->DrawBitmap((*bmpIter).second.Get(), distRectF);
		}
	}

	void PainterD2D1::InvalidateDrawArea() {
		InvalidateRect(m_hWnd, NULL, FALSE);
	}

}