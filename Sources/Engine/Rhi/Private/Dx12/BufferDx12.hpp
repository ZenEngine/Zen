#pragma once
#include "./../RefCountPtr.hpp"
#include <Rhi/Rhi.hpp>

struct ID3D12Resource;

namespace D3D12MA
{
    class Allocation;
}

namespace zen::rhi
{
    class DeviceDx12;
    class BufferDx12
    {
    public:
        bool initialize(DeviceDx12& device, const BufferDesc& desc);

    private:
        TRefCountPtr<ID3D12Resource> _resource = nullptr;
        D3D12MA::Allocation* _allocation = nullptr;
    };
}