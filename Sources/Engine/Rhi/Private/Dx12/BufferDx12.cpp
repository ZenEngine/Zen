#include "BufferDx12.hpp"
#include "DeviceDx12.hpp"
#include <D3D12MemAlloc.h>

namespace zen::rhi
{
    bool BufferDx12::initialize(DeviceDx12& device, const BufferDesc& desc)
    {
        const D3D12_RESOURCE_DESC resourceDesc
        {
            .Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
            .Alignment = 0,
            .Width = desc.byteSize,
            .Height = 1,
            .DepthOrArraySize = 1,
            .MipLevels = 1,
            .Format = DXGI_FORMAT_UNKNOWN,
            .SampleDesc = { .Count = 1, .Quality = 0 },
            .Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
        };

        D3D12MA::ALLOCATION_DESC allocationDesc
        {
            .HeapType = D3D12_HEAP_TYPE_DEFAULT
        };

        constexpr D3D12_RESOURCE_STATES state = { D3D12_RESOURCE_STATE_COPY_DEST };

        const HRESULT hr = device.getAllocator()->CreateResource(
            &allocationDesc,
            &resourceDesc,
            state,
            nullptr,
            &_allocation,
            IID_PPV_ARGS(_resource.getInitAddress()));

        if (FAILED(hr))
        {
            return false;
        }

        return true;
    }
}


