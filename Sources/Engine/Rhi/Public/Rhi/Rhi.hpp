#pragma once
#include "Handle.hpp"
#include <Core/Misc/EnumUtility.hpp>
#include <cstdint>
#include <span>

namespace zen::rhi
{
    class Buffer;
    class Texture;
   
    enum class PixelFormat
    {
        Unknown,

        R8_Unorm,
        R16_Float,
        R16_Uint,
        R32_Float,
        R32_Uint,
        R8G8_Unorm,
        R16G16_Float,
        R32G32_Float,
        R8G8B8_Unorm,
        R32G32B32_Float,
        R8G8B8A8_Unorm,
        R8G8B8A8_Srgb,
        B8G8R8A8_Unorm,
        B8G8R8A8_Srgb,
        R16G16B16A16_Float,
        R16G16B16A16_Unorm,
        R32G32B32A32_Float,

        D24_Unorm_S8_Uint,
        D32_Float,
        D32_Float_S8_Uint,
    };


    enum class FilterMode
    {
        Nearest = 0,
        Linear = 1,
    };
    enum class CompareOp
    {
        Never,
        Less,
        Equal,
        Lequal,
        Greater,
        Gequal,
        Always
    };

    enum class WrapMode
    {
        Wrap,
        Mirror,
        Clamp,
    };


    struct SamplerDesc final
    {
        CompareOp compare;
        float mipLodBias = 0.0f;
        float minLod = 0.0f;
        float maxLod = 64.0f;
    };

    enum class TextureDimension : uint8_t
    {
        Texture1D,
        Texture2D,
        Texture3D,
        TextureCube,
    };

    enum class TextureUsageFlags : uint8_t
    {
        None = 0,
        DepthStencil = 0b0000'0001,
        RenderTarget = 0b0000'0010,
    };
    ZEN_IMPLEMENT_FLAG_OPERATORS(TextureUsageFlags);

    struct TextureDesc final
    {
        TextureDimension dimension = TextureDimension::Texture2D;
        uint32_t width = 1; ///< 横幅
        uint32_t height = 1; ///< 縦幅
        uint16_t depthOrArraySize = 1; ///< 奥行あるいは配列のサイズ
        uint16_t mipLevels = 1; ///< ミップレベルの数
        PixelFormat format = PixelFormat::R8G8B8A8_Srgb;
        TextureUsageFlags usage = TextureUsageFlags::None; ///< TextureUsage
        const char8_t* debugName = nullptr;
    };

    struct TextureViewDesc final
    {
        PixelFormat format = PixelFormat::Unknown;
        uint16_t mipLevels = 1; ///< ミップレベルの数
        Handle<Texture> texture;
        SamplerDesc samplerDesc;
    };

    enum class BufferUsageFlags
    {
        Index,
        Vertex,
        Uniform,
        Storage,
        Indirect,
    };

    struct BufferDesc final
    {
        uint32_t byteSize = 0;
        BufferUsageFlags usage = BufferUsageFlags::Uniform;
        const char8_t* debugName = nullptr;
    };

    struct BindGroup final
    {

    };

    struct Shader final
    {
        std::span<uint8_t> byteCode;
        const char8_t* entryPoint = nullptr;
    };

    struct ShaderDesc final
    {
        Shader vertexShader;
        Shader pixelShader;
        Shader computeShader;
        Shader meshShader;
    };

    class SwapChain
    {
        virtual ~SwapChain() noexcept = default;
    };

    struct DeviceDesc final
    {
        bool enableDebug{ false };
    };

    class Device
    {
    public:
        virtual ~Device() noexcept = default;
        virtual Handle<Buffer> createBuffer(const BufferDesc& desc) = 0;
        virtual Handle<Texture> createTexture(const TextureDesc& desc) = 0;
        // virtual Handle<Texture> createTexture(const TextureViewDesc& desc, std::span<const uint8_t> data) = 0;
        virtual Handle<Texture> createTextureView(const TextureViewDesc& desc) = 0;
        virtual void destroyBuffer(Handle<Buffer> handle) = 0;
        virtual void destroyTexture(Handle<Texture> handle) = 0;
    };
}