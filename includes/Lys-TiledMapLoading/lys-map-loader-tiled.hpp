#ifndef _MAP_LOADER_TILED_HPP
#define _MAP_LOADER_TILED_HPP 1

#include <lys-map-data.hpp>
#include <lys-map-layout.hpp>
#include <string>

namespace lys
{

template<typename __Vec2i>
class __MapLoaderTiled
{
public:
    template<typename ___Vec2f, typename ___Vec2u, typename ___Color>
    bool LoadMap(const char* file_path, __MapData<___Vec2f, ___Vec2u, ___Color>& map_data,
                 std::unique_ptr<__MapLayout<___Vec2f, __Vec2i>>& map_layout);

private:
    struct TiledHeader
    {
        std::string m_Version       = {};
        std::string m_Tiled_Version = {};
        std::string m_Orientation   = {};
        __Vec2i m_Map_Dimension     = { 0, 0 };
        __Vec2i m_Tile_Dimension    = { 0, 0 };
        bool m_Is_Infinite          = false;
    };
};

}  // namespace lys

#include <lys-map-loader-tiled.inl>

#endif  // _MAP_LOADER_TILED_HPP
