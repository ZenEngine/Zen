#pragma once
#include "./../RefCountPtr.hpp"
#include <Rhi/Rhi.hpp>

struct ID3D12Resource;
struct D3D12_CPU_DESCRIPTOR_HANDLE;

namespace D3D12MA
{
    class Allocation;
}

namespace zen::rhi
{
    class DeviceDx12;
    class TextureDx12
    {
    public:
        bool initialize(DeviceDx12& device, const TextureDesc& desc);

    private:
        bool createShaderResourceView(const TextureViewDesc& desc, D3D12_CPU_DESCRIPTOR_HANDLE srvHandle);

        DeviceDx12* _device = nullptr;
        TRefCountPtr<ID3D12Resource> _resource = nullptr;
        D3D12MA::Allocation* _allocation = nullptr;
    };
}