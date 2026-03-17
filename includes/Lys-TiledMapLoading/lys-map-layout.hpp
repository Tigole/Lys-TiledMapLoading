#ifndef _LYS_MAP_LAYOUT_HPP
#define _LYS_MAP_LAYOUT_HPP 1

#include <lys-vec2.hpp>

namespace lys
{

template<typename __Vec2f, typename __Vec2i>
class __MapLayout
{
public:
    virtual ~__MapLayout() = default;

    virtual __Vec2f CoordToNormalizedSpace(__Vec2i coord, bool center) const = 0;
    virtual __Vec2f GetCellSizeNormalizedSpace(void) const                   = 0;
};

template<typename __Vec2f, typename __Vec2i>
class __MapLayout_Hexagonal: public __MapLayout<__Vec2f, __Vec2i>
{
public:
    __Vec2f CoordToNormalizedSpace(__Vec2i coord, bool center) const override;
    __Vec2f GetCellSizeNormalizedSpace(void) const override;
};

// using MapLayout_Hexagonal = __MapLayout_Hexagonal<lys::Vec2f32, lys::Vec2i32>;

}  // namespace lys

#include <lys-map-layout.hpp>

#endif  // _LYS_MAP_LAYOUT_HPP
