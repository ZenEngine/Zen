#include "TextureDx12.hpp"
#include "DeviceDx12.hpp"
#include "./../DxgiUtility.hpp"
#include <D3D12MemAlloc.h>

namespace zen::rhi
{
    namespace
    {
        [[nodiscard]]
        D3D12_RESOURCE_DIMENSION toNativeResourceDimension(const TextureDimension dimension)
        {
            switch (dimension)
            {
            case TextureDimension::Texture1D:
                return D3D12_RESOURCE_DIMENSION_TEXTURE1D;
            case TextureDimension::Texture2D:
                return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            case TextureDimension::Texture3D:
                return D3D12_RESOURCE_DIMENSION_TEXTURE3D;
            case TextureDimension::TextureCube:
                return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            }
            return D3D12_RESOURCE_DIMENSION_UNKNOWN;
        }

        [[nodiscard]]
        D3D12_RESOURCE_FLAGS toNativeResourceFlags(const TextureUsageFlags usage)
        {
            D3D12_RESOURCE_FLAGS result = D3D12_RESOURCE_FLAG_NONE;
            if (!!(TextureUsageFlags::RenderTarget & usage))
            {
                result |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
            }
            if (!!(TextureUsageFlags::DepthStencil & usage))
            {
                result |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
            }
            return result;
        }

        [[nodiscard]]
        D3D12_SHADER_RESOURCE_VIEW_DESC toNativeShaderResoureViewDesc(const TextureViewDesc& desc)
        {
            D3D12_SHADER_RESOURCE_VIEW_DESC result
            {
                .Format = toNativeFormat(desc.format),
            };
            return result;
        }
    }
    bool TextureDx12::initialize(DeviceDx12& device, const TextureDesc& desc)
    {
        const D3D12_RESOURCE_DESC resourceDesc
        {
            .Dimension = toNativeResourceDimension(desc.dimension),
            .Alignment = 0,
            .Width = desc.width,
            .Height = desc.height,
            .DepthOrArraySize = desc.depthOrArraySize,
            .MipLevels = desc.mipLevels,
            .Format = toNativeFormat(desc.format),
            .SampleDesc = { .Count = 1, .Quality = 0 },
            .Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
            .Flags = toNativeResourceFlags(desc.usage)
        };

        D3D12MA::ALLOCATION_DESC allocationDesc
        {
            .HeapType = D3D12_HEAP_TYPE_DEFAULT
        };

        D3D12_CLEAR_VALUE clearValue = {};

        bool hasTarget = false;
        if ((resourceDesc.Flags & D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) != 0)
        {
            clearValue.DepthStencil.Depth = 1.0f;
            clearValue.DepthStencil.Stencil = 0;
            hasTarget = true;
        }

        if ((resourceDesc.Flags & D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) != 0)
        {
            clearValue.Color[0] = 0.0f;
            clearValue.Color[1] = 0.0f;
            clearValue.Color[2] = 0.0f;
            clearValue.Color[3] = 0.0f;
            hasTarget = true;
        }

        constexpr D3D12_RESOURCE_STATES state = { D3D12_RESOURCE_STATE_COPY_DEST };

        const HRESULT hr = device.getAllocator()->CreateResource(
            &allocationDesc,
            &resourceDesc,
            state,
            (hasTarget) ? &clearValue : nullptr,
            &_allocation,
            IID_PPV_ARGS(_resource.getInitAddress()));

        if (FAILED(hr))
        {
            return false;
        }

        _device = &device;

        return true;
    }

    bool TextureDx12::createShaderResourceView(
        const TextureViewDesc& desc,
        D3D12_CPU_DESCRIPTOR_HANDLE srvHandle)
    {
        if (!_device)
        {
            return false;
        }

        ID3D12Device* const nativeDevice = _device->getDevice();
        if (!nativeDevice)
        {
            return false;
        }

        const D3D12_SHADER_RESOURCE_VIEW_DESC shaderResoureceViewDesc = toNativeShaderResoureViewDesc(desc);
        nativeDevice->CreateShaderResourceView(_resource.get(), &shaderResoureceViewDesc, srvHandle);

        return true;
    }
}