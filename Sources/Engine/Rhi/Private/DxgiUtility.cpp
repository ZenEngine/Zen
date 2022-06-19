#include "DxgiUtility.hpp"
#include <Core/Misc/Assert.hpp>

namespace zen::rhi
{
	DXGI_FORMAT toNativeFormat(const PixelFormat format)
	{
		switch (format)
		{
		case PixelFormat::Unknown:
			return DXGI_FORMAT_UNKNOWN;
		case PixelFormat::R8_Unorm:
			return DXGI_FORMAT_R8_UNORM;
		case PixelFormat::R16_Float:
			return DXGI_FORMAT_R16_FLOAT;
		case PixelFormat::R16_Uint:
			return DXGI_FORMAT_R16_UINT;
		case PixelFormat::R32_Float:
			return DXGI_FORMAT_R32_FLOAT;
		case PixelFormat::R32_Uint:
			return DXGI_FORMAT_R32_UINT;
		case PixelFormat::R8G8_Unorm:
			return DXGI_FORMAT_R8G8_UNORM;
		case PixelFormat::R16G16_Float:
			return DXGI_FORMAT_R16G16_FLOAT;
		case PixelFormat::R32G32_Float:
			return DXGI_FORMAT_R32G32_FLOAT;
		case PixelFormat::R8G8B8_Unorm:
			// DXGI_FORMAT_R8G8B8_UNORM doesn't exist.
			ZEN_ASSERT_NO_ENTRY();
			break;
		case PixelFormat::R32G32B32_Float:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case PixelFormat::R8G8B8A8_Unorm:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case PixelFormat::R8G8B8A8_Srgb:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case PixelFormat::B8G8R8A8_Unorm:
			return DXGI_FORMAT_B8G8R8A8_UNORM;
		case PixelFormat::B8G8R8A8_Srgb:
			return DXGI_FORMAT_B8G8R8A8_UNORM;
		case PixelFormat::D24_Unorm_S8_Uint:
			return DXGI_FORMAT_D24_UNORM_S8_UINT;
		case PixelFormat::D32_Float:
			return DXGI_FORMAT_D32_FLOAT;
		case PixelFormat::D32_Float_S8_Uint:
			return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		default:
			break;
		}
		ZEN_ASSERT_NO_ENTRY();
		return DXGI_FORMAT_UNKNOWN;
	}
}
