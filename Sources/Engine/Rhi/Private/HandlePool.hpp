#pragma once
#include "slot_map.h"
#include <Rhi/Handle.hpp>
#include <utility>

namespace zen::rhi
{
    template<typename VirtualType, typename PlatformType>
    class THandlePool
    {
    public:
        template <class... Args>
        Handle<VirtualType> emplace(Args&&... args)
        {
            const dod::slot_map_key32 key = _slotMap.emplace(std::forward<Args>(args)...);
            return Handle<VirtualType>();
        }

        PlatformType* get(Handle<VirtualType> handle)
        {
            return _slotMap.get(dod::slot_map_key32<PlatformType>{ handle.getRaw() });
        }

        void erase(Handle<VirtualType> handle)
        {
            _slotMap.erase(dod::slot_map_key32<PlatformType>{ handle.getRaw() });
        }

    private:
        dod::slot_map32<PlatformType> _slotMap = {};
    };
}