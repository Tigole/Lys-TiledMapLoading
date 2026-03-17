#ifndef _LYS_MAP_LAYOUT_HPP
#define _LYS_MAP_LAYOUT_HPP 1

#include <lys-vec2.hpp>

namespace lys
{

template<typename __Vec2f, typename __Vec2i>
__Vec2f __MapLayout_Hexagonal<__Vec2f, __Vec2i>::CoordToNormalizedSpace(__Vec2i coord, bool center) const
{
    hex::Layout l_Layout(HexTileMode::Pointy_Top, 52.0f, 52.0f, 0.0f, 0.0f);
    // hex::Layout l_Layout(HexTileMode::Pointy_Top, 52.0f, 52.0f, tile_size.x / 2.0f, tile_size.y / 2.0f);
    // hex::Point l_Point = hex::fn_Hex_To_Pixel(l_Layout, hex::fn_From_Offset(hex::OffsetCoordMode::Odd, hex::OffsetCoordType::r, {coord.x,
    // coord.y}));

    __Vec2f l_Pix;

    l_Pix.x = (coord.x + (0.5f * (coord.y & 1)));
    l_Pix.y = coord.y * 0.75f;

    if (center == true)
    {
        l_Pix.x += 0.5f;
        l_Pix.y += 0.5f;
    }

    return l_Pix * mt_Get_Cell_Size_Normalized_Space();
}

template<typename __Vec2f, typename __Vec2i>
__Vec2f __MapLayout_Hexagonal<__Vec2f, __Vec2i>::GetCellSizeNormalizedSpace(void) const
{
    return __Vec2f(sqrt(3) / 2.0f, 1.0f);
}

}  // namespace lys

#endif  // _LYS_MAP_LAYOUT_HPP
