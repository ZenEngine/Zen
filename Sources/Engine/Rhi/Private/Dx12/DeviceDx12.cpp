#include "DeviceDx12.hpp"
#include <utility>

#include <d3d12sdklayers.h>

namespace zen::rhi
{
    bool DeviceDx12::initialize(const DeviceDesc& desc)
    {
        UINT dxgiFactoryFlags{ 0 };
        if (desc.enableDebug)
        {
            // デバッグレイヤーを有効化
            TRefCountPtr<ID3D12Debug1> debugInterface{};
            if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugInterface.getInitAddress()))))
            {
                debugInterface->EnableDebugLayer();
                dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
            }
            else
            {
                // Log
            }

            // GPUBasedValidationの有効化 
        }
        return true;
    }

    Handle<Buffer> DeviceDx12::createBuffer(const BufferDesc& desc)
    {
        BufferDx12 buffer{};
        if (buffer.initialize(*this, desc))
        {
            return _buffers.emplace(std::move(buffer));
        }
        return Handle<Buffer>{};
    }

    Handle<Texture> DeviceDx12::createTexture(const TextureDesc& desc)
    {
        TextureDx12 texture{};
        if (texture.initialize(*this, desc))
        {
            return _textures.emplace(std::move(texture));
        }

        return Handle<Texture>{};
    }

    Handle<Texture> DeviceDx12::createTextureView(const TextureViewDesc& desc)
    {
        TextureDx12* const texture = _textures.get(desc.texture);
        if (texture && texture->createView(desc))
        {
            return desc.texture;
        }
        return Handle<Texture>{};
    }

    void DeviceDx12::destroyBuffer(Handle<Buffer> handle)
    {
        _buffers.erase(handle);
    }

    void DeviceDx12::destroyTexture(Handle<Texture> handle)
    {
        _textures.erase(handle);
    }
}