/*
 * File name	: vdirectxfactory.h
 * Author		: Margoo
 * Date			: 11/11/2022
 * Description	: This head provide a wrapper for DirectX2D factory and
 *				  global factory instance for renderer
 */
#pragma once

#include "vrenderbasic.h"

VLIB_BEGIN_NAMESPACE

namespace Core
{
/*
 * VDirectXFactoryType enum:
 *	Description : This enum mark the type of a factory object
 */
VRENDER_HELPER enum class VDirectXFactoryType
{
	IWICImageingFactory,
	ID2D1Factory
};

/*
 * VDirectXFcatoryThreadMode enum:
 *	Description : This enum mark the thread mode of a factory object
 */
VRENDER_HELPER enum class VDirectXFactoryThreadMode
{
	SignleThreaded,
	MultiThreaded
};

/*
 * VDirectXFactory class:
 *	Description : The basic class of factory
 */
VRENDER_HELPER class VDirectXFactory
{
public:
	VDirectXFactory() = default;

public:
	virtual VDirectXFactoryType GetFactoryType()
	{
		return VDirectXFactoryType::ID2D1Factory;
	}
};

/*
 * VDirectXD2DFactory class:
 *	Description : The wrapper of ID2D1Factory.
 */
VRENDER_HELPER class VDirectXD2DFactory : public VDirectXFactory
{
public:
	explicit VDirectXD2DFactory(const VDirectXFactoryThreadMode &FactoryMode);
	~VDirectXD2DFactory();

public:
	VDirectXFactoryType GetFactoryType() override
	{
		return VDirectXFactoryType::ID2D1Factory;
	}

public:
	ID2D1Factory *GetDXFactory()
	{
		return Factory;
	}

private:
	ID2D1Factory *Factory;
};

/*
 * VDirectXIWICImagingFactory class:
 *	Description : The wrapper of IWICImageingFactory
 */
VRENDER_HELPER class VDirectXIWICImagingFactory : public VDirectXFactory
{
public:
	VDirectXIWICImagingFactory();
	~VDirectXIWICImagingFactory();

public:
	VDirectXFactoryType GetFactoryType() override
	{
		return VDirectXFactoryType::IWICImageingFactory;
	}

public:
	IWICImagingFactory *GetDXFactory()
	{
		return Factory;
	}

private:
	IWICImagingFactory *Factory{};
};

/*
 * VDirectXIWICImagingFactory class:
 *	Description : The wrapper of IDWriteFactory
 */
VRENDER_HELPER class VDirectXWriteFactory : public VDirectXFactory
{
public:
	VDirectXWriteFactory();
	~VDirectXWriteFactory();

public:
	VDirectXFactoryType GetFactoryType() override
	{
		return VDirectXFactoryType::ID2D1Factory;
	}

public:
	IDWriteFactory *GetDXFactory()
	{
		return Factory;
	}

private:
	IDWriteFactory *Factory{};
};

/*
 * VNativeDirectX2D2Factory class:
 *	Description : The wrapper of ID2D1Factory
 *	Tips		: Not same as VDirectXD2DFactory, this factory is the global variable class for factory
 */
VRENDER_HELPER class VNativeDirectX2D2Factory
{
public:
	VNativeDirectX2D2Factory();
	~VNativeDirectX2D2Factory();

public:
	ID2D1Factory *GetInstance()
	{
		return Factory;
	}

private:
	ID2D1Factory *Factory{};
};
/*
 * VNativeDirectXIWICImagingFactory class:
 *	Description : The wrapper of IWICImagingFactory
 *	Tips		: Not same as VDirectXIWICImagingFactory, this factory is the global variable class for factory
 */
VRENDER_HELPER class VNativeDirectXIWICImagingFactory
{
public:
	VNativeDirectXIWICImagingFactory();
	~VNativeDirectXIWICImagingFactory();

public:
	IWICImagingFactory *GetInstance()
	{
		return Factory;
	}

private:
	IWICImagingFactory *Factory;
};
/*
 * VNativeDirectXWriteFactory class:
 *	Description : The wrapper of IDWriteFactory
 *	Tips		: Not same as VDirectXWriteFactory, this factory is the global variable class for factory
 */
VRENDER_HELPER class VNativeDirectXWriteFactory : public VDirectXFactory
{
public:
	VNativeDirectXWriteFactory();

	~VNativeDirectXWriteFactory();

public:
	IDWriteFactory *GetInstance()
	{
		return Factory;
	}

public:
	VDirectXFactoryType GetFactoryType() override
	{
		return VDirectXFactoryType::ID2D1Factory;
	}

private:
	IDWriteFactory *Factory;
};

/*
 * Following variables are the instance of native factory
 */

VRENDER_HELPER extern VNativeDirectX2D2Factory		   VDirectXD2DFactory;
VRENDER_HELPER extern VNativeDirectXIWICImagingFactory VDirectXIWICImagingFactory;
VRENDER_HELPER extern VNativeDirectXWriteFactory	   VDirectXWriteFactory;

/*
 * VRenderHandle class:
 *	Description : The handle of a render environment
 *	Tips		: A render environment handle is required for any environment-related render component's creation
 */
class VRenderHandle
{
public:
	explicit VRenderHandle();
	VRenderHandle(const int &NullValue) VRENDER_HELPER;
	VRenderHandle(const VRenderHandle &RenderHandle);
	VRenderHandle(ID2D1Factory *ID2D1Factory, IWICImagingFactory *IWICImagingFactory, IDWriteFactory *IDWriteFactory,
				  ID2D1RenderTarget *IRenderTarget, VKits::VAllocator *VAllocator) VRENDER_HELPER;

	~VRenderHandle() = default;

public:
	/*
	 * CheckHandleValid function:
	 *		Description : Check this handle is valid or not, if handle is valid, return true, nor false
	 *		Tips		: This function will return false when any class member is a null pointer
	 */
	bool CheckHandleValid() const;

public:
	ID2D1Factory *_ID2D1Factory				VRENDER_HELPER;
	IWICImagingFactory *_IWICImagingFactory VRENDER_HELPER;
	IDWriteFactory *_IDWriteFactory			VRENDER_HELPER;
	ID2D1RenderTarget *_IRenderTarget		VRENDER_HELPER;

	VKits::VAllocator *Allocator;
};
} // namespace Core

VLIB_END_NAMESPACE