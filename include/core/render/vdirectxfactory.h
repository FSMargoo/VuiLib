// License(MIT)
// Athuor: Margoo
// A DirectX Factory wrapper
#pragma once

#include "vrenderbasic.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

enum class VDirectXFactoryType {
	IWICImageingFactory, ID2D1Factory
};

enum class VDirectXFactoryThreadMode {
	SignleThreaded, MultiThreaded
};

class VDirectXFactory {
 public:
	 VDirectXFactory() = default;

	 virtual VDirectXFactoryType GetFactoryType() {
		 return VDirectXFactoryType::ID2D1Factory;
	 }
};

class VDirectXD2DFactory : public VDirectXFactory {
 private:
	 ID2D1Factory* Factory{};

 public:
	 VDirectXFactoryType GetFactoryType() override {
		 return VDirectXFactoryType::ID2D1Factory;
	 }

 public:
	 ID2D1Factory* GetDXFactory() {
		 return Factory;
	 }

 public:
	 explicit VDirectXD2DFactory(const VDirectXFactoryThreadMode& FactoryMode);
	 ~VDirectXD2DFactory();
};
class VDirectXIWICImagingFactory : public VDirectXFactory {
 private:
	IWICImagingFactory* Factory{};

 public:
	VDirectXFactoryType GetFactoryType() override {
		return VDirectXFactoryType::IWICImageingFactory;
	}

 public:
	IWICImagingFactory* GetDXFactory() {
		return Factory;
	}

 public:
	VDirectXIWICImagingFactory();
	~VDirectXIWICImagingFactory();
};
class VDirectXWriteFactory : public VDirectXFactory {
 private:
	 IDWriteFactory* Factory{};
	 
 public:
	VDirectXFactoryType GetFactoryType() override {
		return VDirectXFactoryType::ID2D1Factory;
	}

 public:
	 IDWriteFactory* GetDXFactory() {
		return Factory;
	}

 public:
	 VDirectXWriteFactory();
	 ~VDirectXWriteFactory();
};

class VNativeDirectX2D2Factory {
 private:
	 ID2D1Factory* Factory{};

 public:
	 VNativeDirectX2D2Factory();
	 ~VNativeDirectX2D2Factory();

	 ID2D1Factory* GetInstance() {
		 return Factory;
	 }
};
class VNativeDirectXIWICImagingFactory {
private:
	IWICImagingFactory* Factory{};

public:
	VNativeDirectXIWICImagingFactory();
	~VNativeDirectXIWICImagingFactory();

	IWICImagingFactory* GetInstance() {
		return Factory;
	}
};
class VNativeDirectXWriteFactory : public VDirectXFactory {
private:
	IDWriteFactory* Factory{};

public:
	VDirectXFactoryType GetFactoryType() override {
		return VDirectXFactoryType::ID2D1Factory;
	}

public:
	VNativeDirectXWriteFactory();
	~VNativeDirectXWriteFactory();

	IDWriteFactory* GetInstance() {
		return Factory;
	}
};

extern VNativeDirectX2D2Factory         VDirectXD2DFactory;
extern VNativeDirectXIWICImagingFactory VDirectXIWICImagingFactory;
extern VNativeDirectXWriteFactory       VDirectXWriteFactory;

}

VLIB_END_NAMESPACE