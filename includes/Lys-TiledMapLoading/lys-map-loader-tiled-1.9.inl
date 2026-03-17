#include <lys-map-layout.hpp>
#include <lys-map-loader-tiled-1.9.hpp>
#include <lys-vec2-traits.hpp>

#ifndef LYS_LOG_CORE_ERROR
#define LYS_LOG_CORE_ERROR(...)
#endif

namespace lys
{

template<typename __Vec2f, typename __Vec2u, typename __Color>
template<typename ___Vec2f, typename ___Vec2i>
bool __MapLoaderTiled_1_9<__Vec2f, __Vec2u, __Color>::Load(std::filesystem::path file_path, __MapData<__Vec2f, __Vec2u, __Color>& map_data,
                                                           std::unique_ptr<__MapLayout<___Vec2f, ___Vec2i>>& map_layout)
{
    auto l_fn_Orientation = [](const std::string& value, TiledTypes::TileType& res) -> bool
    {
        if (value == "orthogonal")
        {
            res = TiledTypes::TileType::Orthogonal;
            return true;
        }
        if (value == "hexagonal")
        {
            res = TiledTypes::TileType::Hexagonal;
            return true;
        }

        return false;
    };
    auto l_fn_Axis = [](const std::string& value, TiledTypes::StaggerAxis& res) -> bool
    {
        if (value == "x")
        {
            res = TiledTypes::StaggerAxis::X;
            return true;
        }
        if (value == "y")
        {
            res = TiledTypes::StaggerAxis::Y;
            return true;
        }
        return false;
    };
    auto l_fn_Index = [](const std::string& value, TiledTypes::StaggerIndex& res) -> bool
    {
        if (value == "even")
        {
            res = TiledTypes::StaggerIndex::Even;
            return true;
        }
        if (value == "odd")
        {
            res = TiledTypes::StaggerIndex::Odd;
            return true;
        }
        return false;
    };
    XMLFileLoader l_Loader;

    l_Loader.mt_Add_On_Entry_Callback("/map", [&](const XML_Element& map)
    {
        if (map.mt_Get_Attribute("orientation", map_data.m_Tile_Type, l_fn_Orientation) == false)
        {
            return false;
        }
        if (map_data.m_Tile_Type == TiledTypes::TileType::Hexagonal)
        {
            if (map.mt_Get_Attribute("staggeraxis", map_data.m_Stagger_Axis, l_fn_Axis) == false)
            {
                return false;
            }
            if (map.mt_Get_Attribute("staggerindex", map_data.m_Stagger_Index, l_fn_Index) == false)
            {
                return false;
            }
        }

        return true;
    });
    l_Loader.mt_Add_On_Entry_Callback("/map/tileset", [&](const XML_Element& tileset)
    {
        std::string l_Source_File;
        typename __MapData<__Vec2f, __Vec2u, __Color>::TilesetData l_Tileset_Data;

        if (tileset.mt_Get_Attribute("firstgid", l_Tileset_Data.firstGid) == false)
        {
            return false;
        }
        if (tileset.mt_Get_Attribute("source", l_Source_File) == false)
        {
            return false;
        }

        if (LoadTileset(file_path / l_Source_File, l_Tileset_Data) == false)
        {
            return false;
        }

        /// fixme multiple images
        map_data.m_Tileset_Data.push_back(l_Tileset_Data);

        return true;
    });
    l_Loader.mt_Add_On_Entry_Callback("/map/layer", [&](const XML_Element& layer) { return LoadLayer(layer); });
    l_Loader.mt_Add_On_Entry_Callback("/map/layer/data", [&](const XML_Element& layer) { return LoadLayerData(layer); });
    l_Loader.mt_Add_On_Exit_Callback("/map/layer", [&](const XML_Element&)
    {
        map_data.m_Tiles_Layers.mt_Add_Element(m_Tile_Layer, m_Tile_Layer.m_Tile_Layer_Id, m_Tile_Layer.m_Tile_Layer_Name);

        return true;
    });
    l_Loader.mt_Add_On_Entry_Callback("/map/objectgroup", [&](const XML_Element& objectgroup)
    {
        m_Object_Layer = __MapData<__Vec2f, __Vec2u, __Color>::ObjectLayer();

        if (objectgroup.mt_Get_Attribute("name", m_Object_Layer.m_Object_Layer_Name) == false)
        {
            return false;
        }
        if (objectgroup.mt_Get_Attribute("id", m_Object_Layer.m_Object_Layer_Id) == false)
        {
            return false;
        }

        return true;
    });
    l_Loader.mt_Add_On_Entry_Callback("/map/objectgroup/object", [&](const XML_Element& object) { return LoadObject(object); });
    l_Loader.mt_Add_On_Entry_Callback("/map/objectgroup/object/polygon",
                                      [&](const XML_Element& polygon) { return LoadObjectPolygon(polygon); });
    l_Loader.mt_Add_On_Entry_Callback("/map/objectgroup/object/properties/property",
                                      [&](const XML_Element& object) { return LoadObjectProperty(object); });
    l_Loader.mt_Add_On_Exit_Callback("/map/objectgroup/object", [&](const XML_Element&)
    {
        m_Object_Layer.m_Objects.mt_Add_Element(m_Object, m_Object.m_Object_Id, m_Object.m_Object_Name);
        return true;
    });
    l_Loader.mt_Add_On_Exit_Callback("/map/objectgroup", [&](const XML_Element&)
    {
        map_data.m_Objects_Layers.mt_Add_Element(m_Object_Layer, m_Object_Layer.m_Object_Layer_Id, m_Object_Layer.m_Object_Layer_Name);
        return true;
    });
    /*l_Loader.mt_Add_On_Entry_Callback("", [&](const XML_Element& )
    {
        return true;
    });
    l_Loader.mt_Add_On_Entry_Callback("", [&](const XML_Element& )
    {
        return true;
    });
    l_Loader.mt_Add_On_Entry_Callback("", [&](const XML_Element& )
    {
        return true;
    });*/

    if (l_Loader.mt_Load(file_path.string()) == false)
    {
        LYS_LOG_CORE_ERROR("%s", l_Loader.mt_Get_Error_Description().c_str());
        return false;
    }

    if (map_data.m_Tile_Type == TiledTypes::TileType::Hexagonal)
    {
        /// fixme Multiple layouts (1 by tileset ?) ?
        map_layout.reset(new __MapLayout<___Vec2f, ___Vec2i>());
    }

    return true;
}

template<typename __Vec2f, typename __Vec2u, typename __Color>
bool __MapLoaderTiled_1_9<__Vec2f, __Vec2u, __Color>::LoadLayer(const XML_Element& layer)
{
    if (layer.mt_Get_Attribute("name", m_Tile_Layer.m_Tile_Layer_Name) == false)
    {
        return false;
    }
    if (layer.mt_Get_Attribute("id", m_Tile_Layer.m_Tile_Layer_Id) == false)
    {
        return false;
    }
    if (layer.mt_Get_Attribute("width", m_Tile_Layer.m_Tile_Layer_Size.x) == false)
    {
        return false;
    }
    if (layer.mt_Get_Attribute("height", m_Tile_Layer.m_Tile_Layer_Size.y) == false)
    {
        return false;
    }

    m_Tile_Layer.m_Tiles.mt_Resize(m_Tile_Layer.m_Tile_Layer_Size.x, m_Tile_Layer.m_Tile_Layer_Size.y, 0);

    return true;
}

template<typename __Vec2f, typename __Vec2u, typename __Color>
bool __MapLoaderTiled_1_9<__Vec2f, __Vec2u, __Color>::LoadLayerData(const XML_Element& data)
{
    std::string l_Data_Encoding_Type;

    if (data.mt_Get_Attribute("encoding", l_Data_Encoding_Type) == false)
    {
        return false;
    }

    if (l_Data_Encoding_Type == "csv")
    {
        std::string l_CSV_Content;

        if (data.mt_Get_Text(l_CSV_Content) == false)
        {
            return false;
        }

        return LoadLayerDataCSV(l_CSV_Content.c_str());
    }

    return false;
}

template<typename __Vec2f, typename __Vec2u, typename __Color>
bool __MapLoaderTiled_1_9<__Vec2f, __Vec2u, __Color>::LoadLayerDataCSV(const char* csv_encoded_data)
{
    bool l_b_Ret          = false;
    uint32_t l_Tile_Index = 0;
    Index2D l_Tile_Array_Index(0, 0);

    if (csv_encoded_data != nullptr)
    {
        while ((*csv_encoded_data != '\0') && (m_Tile_Layer.m_Tiles.mt_Is_Index_Valid(l_Tile_Array_Index)))
        {
            if (*csv_encoded_data == ',')
            {
                m_Tile_Layer.m_Tiles[l_Tile_Array_Index] = l_Tile_Index;
                l_Tile_Index                             = 0;
                m_Tile_Layer.m_Tiles.mt_Increment_Index(l_Tile_Array_Index);
            }
            else if (*csv_encoded_data != ' ')
            {
                l_Tile_Index *= 10;
                l_Tile_Index += *csv_encoded_data - '0';
            }

            csv_encoded_data++;
        }
        l_b_Ret = true;
    }

    return l_b_Ret;
}

template<typename __Vec2f, typename __Vec2u, typename __Color>
bool __MapLoaderTiled_1_9<__Vec2f, __Vec2u, __Color>::LoadObject(const XML_Element& object)
{
    m_Object = MapData::Object();

    if (object.mt_Get_Attribute("id", m_Object.m_Object_Id) == false)
    {
        return false;
    }
    if (object.mt_Get_Attribute("x", m_Object.m_Object_Pix_Pos.x) == false)
    {
        return false;
    }
    if (object.mt_Get_Attribute("y", m_Object.m_Object_Pix_Pos.y) == false)
    {
        return false;
    }

    object.mt_Get_Attribute("name", m_Object.m_Object_Name);
    object.mt_Get_Attribute("class", m_Object.m_Object_Class);
    object.mt_Get_Attribute("width", m_Object.m_Object_Pix_Size.x);
    object.mt_Get_Attribute("height", m_Object.m_Object_Pix_Size.y);
    object.mt_Get_Attribute("rotation", m_Object.m_Object_Rotation_Degres);
    object.mt_Get_Attribute("gid", m_Object.m_Object_Gid);

    return true;
}

template<typename __Vec2f, typename __Vec2u, typename __Color>
bool __MapLoaderTiled_1_9<__Vec2f, __Vec2u, __Color>::LoadObjectProperty(const XML_Element& property)
{
    std::string l_Property_Name;
    std::string l_Property_Type;
    bool l_Bool_Value   = false;
    int l_Int_Value     = 0;
    float l_Float_Value = 0.0f;
    std::string l_String_Value;
    Color l_Color;

    if (property.mt_Get_Attribute("name", l_Property_Name) == false)
    {
        return false;
    }
    if (property.mt_Get_Attribute("type", l_Property_Type) == true)
    {
        if (l_Property_Type == "bool")
        {
            property.mt_Get_Attribute("value", l_Bool_Value);
            m_Object.m_Object_Properties_Bool.emplace(l_Property_Name, l_Bool_Value);
            return true;
        }
        else if (l_Property_Type == "int")
        {
            property.mt_Get_Attribute("value", l_Int_Value);
            m_Object.m_Object_Properties_Int.emplace(l_Property_Name, l_Int_Value);
            return true;
        }
        else if (l_Property_Type == "float")
        {
            property.mt_Get_Attribute("value", l_Float_Value);
            m_Object.m_Object_Properties_Float.emplace(l_Property_Name, l_Float_Value);
            return true;
        }
        else if (l_Property_Type == "color")
        {
            property.mt_Get_Attribute("value", l_String_Value);
            m_Object.m_Object_Properties_Color.emplace(l_Property_Name, l_Color);
            return true;
        }
        else if (l_Property_Type == "file")
        {
            property.mt_Get_Attribute("value", l_String_Value);
            m_Object.m_Object_Properties_String.emplace(l_Property_Name, l_String_Value);
            return true;
        }
        else if (l_Property_Type == "object")
        {
            property.mt_Get_Attribute("value", l_Int_Value);
            m_Object.m_Object_Properties_Int.emplace(l_Property_Name, l_Int_Value);
            return true;
        }
    }
    else
    {
        property.mt_Get_Attribute("value", l_String_Value);
        m_Object.m_Object_Properties_String.emplace(l_Property_Name, l_String_Value);
        return true;
    }

    LYS_LOG_CORE_ERROR("Failed to load property '%s' of type '%s' (unsupported)", l_Property_Name.c_str(), l_Property_Type.c_str());

    return false;
}

template<typename __Vec2f, typename __Vec2u, typename __Color>
bool __MapLoaderTiled_1_9<__Vec2f, __Vec2u, __Color>::LoadObjectPolygon(const XML_Element& polygon)
{
    std::stringstream l_ss;
    std::string l_Polygon_Points;

    if (polygon.mt_Get_Attribute("points", l_Polygon_Points) == false)
    {
        return false;
    }
    l_ss.str(l_Polygon_Points);

    while (std::getline(l_ss, l_Polygon_Points, ' '))
    {
        std::stringstream l_ss2;
        __Vec2f l_Offset = Vec2Traits<__Vec2f>::Construct(0.0f, 0.0f);
        char l_Coma;

        l_ss2.str(l_Polygon_Points);

        l_ss2 >> l_Offset.x >> l_Coma >> l_Offset.y;

        m_Object.m_Polygon.push_back(l_Offset);
    }

    return true;
}

template<typename __Vec2f, typename __Vec2u, typename __Color>
bool __MapLoaderTiled_1_9<__Vec2f, __Vec2u, __Color>::LoadTileset(const char* file_path,
                                                                  typename __MapData<__Vec2f, __Vec2u, __Color>::TilesetData& tileset_data)
{
    XMLFileLoader l_Loader;

    l_Loader.mt_Add_On_Entry_Callback("/tileset", [&](const XML_Element& tileset)
    {
        unsigned int l_Tile_Count;
        unsigned int l_Column_Count;

        if (tileset.mt_Get_Attribute("tilewidth", tileset_data.m_Tileset_Info.m_Tile_Size.x) == false)
        {
            return false;
        }
        if (tileset.mt_Get_Attribute("tileheight", tileset_data.m_Tileset_Info.m_Tile_Size.y) == false)
        {
            return false;
        }
        if (tileset.mt_Get_Attribute("tilecount", l_Tile_Count) == false)
        {
            return false;
        }
        if (tileset.mt_Get_Attribute("columns", l_Column_Count) == false)
        {
            return false;
        }

        tileset_data.m_Tileset_Info.m_Tile_Count.x = l_Column_Count;
        tileset_data.m_Tileset_Info.m_Tile_Count.y = (l_Column_Count == 0) ? 1 : (l_Tile_Count / l_Column_Count);
        tileset_data.m_Tileset_Info.m_Texture_Size.x =
            tileset_data.m_Tileset_Info.m_Tile_Size.x * tileset_data.m_Tileset_Info.m_Tile_Count.x;
        tileset_data.m_Tileset_Info.m_Texture_Size.y =
            tileset_data.m_Tileset_Info.m_Tile_Size.y * tileset_data.m_Tileset_Info.m_Tile_Count.y;

        return true;
    });

    if (l_Loader.mt_Load(file_path) == false)
    {
        LYS_LOG_CORE_ERROR("'%s': %s", file_path, l_Loader.mt_Get_Error_Description().c_str());
        return false;
    }

    return true;
}

}  // namespace lys
