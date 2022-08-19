#pragma once
#pragma warning(disable : 26495)

#include "../../../include/core/render/vdirectxfactory.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

VNativeDirectX2D2Factory         VDirectXD2DFactory;
VNativeDirectXIWICImagingFactory VDirectXIWICImagingFactory;
VNativeDirectXWriteFactory       VDirectXWriteFactory;

VDirectXD2DFactory::VDirectXD2DFactory(const VDirectXFactoryThreadMode& FactoryMode)  {
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

	HRESULT ResultHandle = D2D1CreateFactory(
		FactoryType,
		&Factory
	);
	VLIB_CHECK_REPORT(FAILED(ResultHandle), L"DirectX Create Factory Failed!");
}
VDirectXD2DFactory::~VDirectXD2DFactory() {
	VDXObjectSafeFree(&Factory);
}
VDirectXIWICImagingFactory::VDirectXIWICImagingFactory() {
	HRESULT ResultHandle = CoInitialize(nullptr);
	VLIB_CHECK_REPORT(FAILED(ResultHandle), L"Init COM Failed!");

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
	VLIB_CHECK_REPORT(FAILED(ResultHandle), L"DirectX Create Native Factory Failed!");
}
VNativeDirectX2D2Factory::~VNativeDirectX2D2Factory() {
	VDXObjectSafeFree(&Factory);
}

VNativeDirectXIWICImagingFactory::VNativeDirectXIWICImagingFactory() {
	HRESULT ResultHandle = CoInitialize(nullptr);
	VLIB_CHECK_REPORT(FAILED(ResultHandle), L"Init COM Failed!");

	ResultHandle = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS((&(this->Factory)))
	);
	VLIB_CHECK_REPORT(FAILED(ResultHandle), L"DirectX Create Native COM Instance Factory Failed!");
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
	HRESULT Result = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&Factory)
	);

	VLIB_CHECK_REPORT(FAILED(Result), L"Native DirectWrite Factory Created Failed!");
}
VNativeDirectXWriteFactory::~VNativeDirectXWriteFactory() {
	VDXObjectSafeFree(&Factory);
}

}

VLIB_END_NAMESPACE