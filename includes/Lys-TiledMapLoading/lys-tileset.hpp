#ifndef _LYS_TILESET_HPP
#define _LYS_TILESET_HPP 1

// #include <lys-vec2-traits.hpp>
#include <lys-vec2.hpp>

namespace lys
{
template<typename __Vec2u>
struct __TilesetInfo
{
    __Vec2u tileCount   = {};
    __Vec2u tileSize    = {};
    __Vec2u textureSize = {};
};

using TilesetInfo = __TilesetInfo<lys::Vec2u32>;

}  // namespace lys

#endif  // _LYS_TILESET_HPP
