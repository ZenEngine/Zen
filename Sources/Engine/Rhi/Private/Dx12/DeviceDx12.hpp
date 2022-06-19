#pragma once
#include "./../HandlePool.hpp"
#include "./../RefCountPtr.hpp"
#include "BufferDx12.hpp"
#include "TextureDx12.hpp"
#include <Rhi/Rhi.hpp>
#include <dxgi1_6.h>

struct ID3D12Device;

namespace D3D12MA
{
    class Allocator;
}

namespace zen::rhi
{
    class DeviceDx12 : public Device
    {
    public:
        bool initialize(const DeviceDesc& desc);

        Handle<Buffer> createBuffer(const BufferDesc& desc) override;
        Handle<Texture> createTexture(const TextureDesc& desc) override;
        Handle<Texture> createTextureView(const TextureViewDesc& desc) override;
        void destroyBuffer(Handle<Buffer> handle) override;
        void destroyTexture(Handle<Texture> handle) override;

        D3D12MA::Allocator* getAllocator() const noexcept { return _allocator.get(); }
        ID3D12Device* getDevice() const noexcept { return _device.get(); }
    private:
        TRefCountPtr<IDXGIFactory4> _dxgiFactory = nullptr;
        TRefCountPtr<ID3D12Device> _device = nullptr;
        TRefCountPtr<D3D12MA::Allocator> _allocator = nullptr;

        THandlePool<Buffer, BufferDx12> _buffers = {};
        THandlePool<Texture, TextureDx12> _textures = {};
    };
}