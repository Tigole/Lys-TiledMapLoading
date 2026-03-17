#ifndef _LYS__MAP_LOADER_TILED_1_9_HPP
#define _LYS__MAP_LOADER_TILED_1_9_HPP 1

#include <XMLFileLoader.hpp>
#include <filesystem>
#include <lys-map-data.hpp>
#include <lys-map-layout.hpp>
#include <memory>

namespace lys
{

template<typename __Vec2f, typename __Vec2u, typename __Color>
class __MapLoaderTiled_1_9
{
public:
    template<typename ___Vec2f = __Vec2f, typename ___Vec2i = lys::Vec2i32>
    bool Load(std::filesystem::path file_path, __MapData<__Vec2f, __Vec2u, __Color>& map_data,
              std::unique_ptr<__MapLayout<___Vec2f, ___Vec2i>>& map_layout);

private:
    bool LoadLayer(const XML_Element& layer);
    bool LoadLayerData(const XML_Element& data);
    bool LoadLayerDataCSV(const char* csv_encoded_data);

    bool LoadObject(const XML_Element& object);
    bool LoadObjectProperty(const XML_Element& property);
    bool LoadObjectPolygon(const XML_Element& polygon);

    bool LoadTileset(const char* file_path, typename __MapData<__Vec2f, __Vec2u, __Color>::TilesetData& tileset_data);

    typename __MapData<__Vec2f, __Vec2u, __Color>::TileLayer m_Tile_Layer     = {};
    typename __MapData<__Vec2f, __Vec2u, __Color>::ObjectLayer m_Object_Layer = {};
    typename __MapData<__Vec2f, __Vec2u, __Color>::Object m_Object            = {};
};

using MapLoaderTiled_1_9 = __MapLoaderTiled_1_9<lys::Vec2f32, lys::Vec2u32, lys::Color>;

}  // namespace lys

#include <lys-map-loader-tiled-1.9.inl>

#endif  // _LYS__MAP_LOADER_TILED_1_9_HPP
