#pragma once
#pragma warning(disable : 26495)

#include "../../../include/core/render/vdirectxfactory.h"

VLIB_BEGIN_NAMESPACE

namespace Core {
	VNativeDirectX2D2Factory         VDirectXD2DFactory;
	VNativeDirectXIWICImagingFactory VDirectXIWICImagingFactory;
	VNativeDirectXWriteFactory       VDirectXWriteFactory;
	
	VDirectXD2DFactory::VDirectXD2DFactory(const VDirectXFactoryThreadMode& FactoryMode) {
		D2D1_FACTORY_TYPE FactoryType;
		switch (FactoryMode) {
		case VDirectXFactoryThreadMode::SignleThreaded: {
			FactoryType = D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_MULTI_THREADED;

			break;
		}
		default: {
			FactoryType = D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED;

			break;
		}
		}

		VLIB_REPORT_IF_FAILED_INFO(D2D1CreateFactory(FactoryType, &Factory), L"In VDirectXD2DFactory::VDirectXD2DFactory, failed to create ID2D1Factory");
	}
	VDirectXD2DFactory::~VDirectXD2DFactory() {
		VDXObjectSafeFree(&Factory);
	}
	VDirectXIWICImagingFactory::VDirectXIWICImagingFactory() {
		HRESULT ResultHandle = CoInitialize(nullptr);
		VLIB_CHECK_REPORT(FAILED(ResultHandle), L"Failed to initialize COM component!");
	
		ResultHandle = CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS((&(this->Factory)))
		);
		VLIB_CHECK_REPORT(FAILED(ResultHandle), L"DirectX Create COM Instance Factory Failed!");
	}
	VDirectXIWICImagingFactory::~VDirectXIWICImagingFactory() {
		VDXObjectSafeFree(&Factory);
	}
	
	VNativeDirectX2D2Factory::VNativeDirectX2D2Factory() {
		HRESULT ResultHandle = D2D1CreateFactory(
			D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_MULTI_THREADED,
			&Factory
		);
		VLIB_CHECK_REPORT(FAILED(ResultHandle), L"In VNativeDirectX2D2Factory::VNativeDirectX2D2Factory, failed to create native factory object");
	}
	VNativeDirectX2D2Factory::~VNativeDirectX2D2Factory() {
		VDXObjectSafeFree(&Factory);
	}
	
	VNativeDirectXIWICImagingFactory::VNativeDirectXIWICImagingFactory() {
		HRESULT ResultHandle = CoInitialize(nullptr);
		VLIB_CHECK_REPORT(FAILED(ResultHandle), L"Failed to initialize COM component!");
	
		ResultHandle = CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS((&(this->Factory)))
		);
		VLIB_CHECK_REPORT(FAILED(ResultHandle), L"In VNativeDirectXIWICImagingFactory::VNativeDirectXIWICImagingFactory, failed to create ID2D1Factory");
	}
	VNativeDirectXIWICImagingFactory::~VNativeDirectXIWICImagingFactory() {
		VDXObjectSafeFree(&Factory);
	}
	
	VDirectXWriteFactory::VDirectXWriteFactory() {
		HRESULT Result = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&Factory)
		);
	
		VLIB_CHECK_REPORT(FAILED(Result), L"DirectWrite Factory Created Failed!");
	}
	VDirectXWriteFactory::~VDirectXWriteFactory() {
		VDXObjectSafeFree(&Factory);
	}
	
	VNativeDirectXWriteFactory::VNativeDirectXWriteFactory() {
		HRESULT Result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&Factory));
	
		VLIB_CHECK_REPORT(FAILED(Result), L"Native DirectWrite Factory Created Failed!");
	}
	VNativeDirectXWriteFactory::~VNativeDirectXWriteFactory() {
		VDXObjectSafeFree(&Factory);
	}

	VRenderHandle::VRenderHandle() :
		_ID2D1Factory  (nullptr), _IWICImagingFactory(nullptr),
		_IDWriteFactory(nullptr), _IRenderTarget(nullptr), Allocator(nullptr) {

	}
	VRenderHandle::VRenderHandle(const int& NullValue) :
		_ID2D1Factory(nullptr), _IWICImagingFactory(nullptr),
		_IDWriteFactory(nullptr), _IRenderTarget(nullptr) {

	}
	VRenderHandle::VRenderHandle(const VRenderHandle& RenderHandle) {
		_ID2D1Factory		= RenderHandle._ID2D1Factory;
		_IWICImagingFactory = RenderHandle._IWICImagingFactory;
		_IDWriteFactory		= RenderHandle._IDWriteFactory;
		_IRenderTarget		= RenderHandle._IRenderTarget;
		Allocator			= RenderHandle.Allocator;
	}
	VRenderHandle::VRenderHandle(ID2D1Factory* ID2D1Factory, IWICImagingFactory* IWICImagingFactory,
								 IDWriteFactory* IDWriteFactory, ID2D1RenderTarget* IRenderTarget,
								 VKits::VAllocator* VAllocator) {
		_ID2D1Factory		= ID2D1Factory;
		_IWICImagingFactory = IWICImagingFactory;
		_IDWriteFactory		= IDWriteFactory;
		_IRenderTarget		= IRenderTarget;
		Allocator			= VAllocator;
	}
	bool VRenderHandle::CheckHandleValid() const {
		if (!_ID2D1Factory || !_IWICImagingFactory || !_IDWriteFactory || !_IRenderTarget) {
			return false;
		}

		return true;
	}
}

VLIB_END_NAMESPACE