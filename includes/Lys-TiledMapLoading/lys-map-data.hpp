#ifndef _MAP_DATA_HPP
#define _MAP_DATA_HPP 1

#include <cstdint>
#include <lys-array-2d.hpp>
#include <lys-color.hpp>
#include <lys-multikey-container.hpp>
#include <lys-tileset.hpp>
#include <lys-vec2.hpp>
#include <map>
#include <string>
#include <vector>

namespace lys
{

struct TiledTypes
{
    enum class TileType
    {
        None,
        Orthogonal,
        Hexagonal
    };

    enum class StaggerAxis
    {
        None,
        X,
        Y
    };

    enum class StaggerIndex
    {
        None,
        Odd,
        Even
    };
};

template<typename __Vec2f, typename __Vec2u, typename __Color>
struct __MapData
{
    // template<typename __Vec2f>
    struct Object
    {
        uint32_t objectId          = 0;
        uint32_t objectGid         = 0;
        std::string objectName     = "";
        std::string objectClass    = "";
        __Vec2f objectPixPos       = {};
        __Vec2f objectPixSize      = {};
        float objectRotationDegres = 0.0f;

        template<typename T>
        bool GetProperty(const std::string& property_name, T& property_value, const std::map<std::string, T>& object_properties_pool) const
        {
            auto it = object_properties_pool.find(property_name);
            if (it != object_properties_pool.end())
            {
                property_value = it->second;
                return true;
            }
            return false;
        }

        bool GetProperty(const std::string& property_name, bool& property_value) const
        {
            return GetProperty(property_name, property_value, objectPropertiesBool);
        }

        bool GetProperty(const std::string& property_name, int& property_value) const
        {
            return GetProperty(property_name, property_value, objectPropertiesInt);
        }

        bool GetProperty(const std::string& property_name, float& property_value) const
        {
            return GetProperty(property_name, property_value, objectPropertiesFloat);
        }

        bool GetProperty(const std::string& property_name, __Color& property_value) const
        {
            return GetProperty(property_name, property_value, objectPropertiesColor);
        }

        bool GetProperty(const std::string& property_name, std::string& property_value) const
        {
            return GetProperty(property_name, property_value, objectPropertiesString);
        }

        std::map<std::string, bool> objectPropertiesBool          = {};
        std::map<std::string, int> objectPropertiesInt            = {};
        std::map<std::string, float> objectPropertiesFloat        = {};
        std::map<std::string, __Color> objectPropertiesColor      = {};
        std::map<std::string, std::string> objectPropertiesString = {};
        // std::map<std::string, std::string> m_Object_Properties_File;
        std::vector<__Vec2f> polygon = {};
    };

    struct ObjectLayer
    {
        uint32_t objectLayerId            = -1;
        std::string objectLayerName       = {};
        MultiKeyContainer<Object> objects = {};
    };

    struct TileLayer
    {
        uint32_t tileLayerId      = -1;
        std::string tileLayerName = {};
        __Vec2u tileLayerSize     = { 0, 0 };
        Array2D<uint32_t> tiles   = {};
    };

    struct TilesetData
    {
        uint32_t firstGid                  = -1;
        __TilesetInfo<__Vec2u> tilesetInfo = {};
    };

    MultiKeyContainer<TileLayer> tilesLayers     = {};
    MultiKeyContainer<ObjectLayer> objectsLayers = {};
    std::vector<TilesetData> tilesetData         = {};
    TiledTypes::TileType tileType                = TiledTypes::TileType::None;
    TiledTypes::StaggerAxis staggerAxis          = TiledTypes::StaggerAxis::None;
    TiledTypes::StaggerIndex staggerIndex        = TiledTypes::StaggerIndex::None;
};

using MapData = __MapData<Vec2f32, Vec2u32, Color>;

}  // namespace lys

#endif  // _MAP_DATA_HPP
