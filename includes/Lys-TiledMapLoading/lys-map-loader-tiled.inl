#ifndef _MAP_LOADER_TILED_HPP
#define _MAP_LOADER_TILED_HPP 1

#include <lys-map-loader-tiled-1.9.hpp>
#include <lys-map-loader-tiled.hpp>

#ifndef LYS_LOG_CORE_ERROR
#define LYS_LOG_CORE_ERROR(...)
#endif

namespace lys
{

template<typename __Vec2i>
template<typename ___Vec2f, typename ___Vec2u, typename ___Color>
bool __MapLoader<__Vec2i>::LoadMap(const char* file_path, __MapData<___Vec2f, ___Vec2u, ___Color>& map_data,
                                   std::unique_ptr<__MapLayout<___Vec2f, __Vec2i>>& map_layout)
{
    XMLFileLoader l_Loader;
    TiledHeader l_Tiled_Header;

    l_Loader.mt_Add_On_Entry_Callback("/map", [&](const XML_Element& map)
    {
        if (map.mt_Get_Attribute("infinite", l_Tiled_Header.m_Is_Infinite) == false)
        {
            return false;
        }
        if (map.mt_Get_Attribute("version", l_Tiled_Header.m_Version) == false)
        {
            return false;
        }
        if (map.mt_Get_Attribute("tiledversion", l_Tiled_Header.m_Tiled_Version) == false)
        {
            return false;
        }
        if (map.mt_Get_Attribute("orientation", l_Tiled_Header.m_Orientation) == false)
        {
            return false;
        }
        if (map.mt_Get_Attribute("width", l_Tiled_Header.m_Map_Dimension.x) == false)
        {
            return false;
        }
        if (map.mt_Get_Attribute("height", l_Tiled_Header.m_Map_Dimension.y) == false)
        {
            return false;
        }
        if (map.mt_Get_Attribute("tilewidth", l_Tiled_Header.m_Tile_Dimension.x) == false)
        {
            return false;
        }
        if (map.mt_Get_Attribute("tileheight", l_Tiled_Header.m_Tile_Dimension.y) == false)
        {
            return false;
        }

        return true;
    });

    if (l_Loader.mt_Load(file_path.mt_Get_Path_Name_Ext()) == false)
    {
        return false;
    }

    if (l_Tiled_Header.m_Version == "1.9")
    {
        __MapLoaderTiled_1_9<___Vec2f, ___Vec2u, ___Color> l_Map_Loader;

        map_data.m_Tiles_Layers.mt_Clear();
        map_data.m_Objects_Layers.mt_Clear();

        return l_Map_Loader.mt_Load<___Vec2f, __Vec2i>(file_path, map_data, map_layout);
    }

    LYS_LOG_CORE_ERROR("Tiled version not handled: '%s'", l_Tiled_Header.m_Version.c_str());

    return false;
}

}  // namespace lys

#endif  // _MAP_LOADER_TILED_HPP
