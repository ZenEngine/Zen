#pragma once

#include <dxgiformat.h>
#include <Rhi/Rhi.hpp>

namespace zen::rhi
{
	DXGI_FORMAT toNativeFormat(const PixelFormat format);
}
