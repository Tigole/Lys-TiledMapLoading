#include "test.hpp"

TEST(MapLoaderTiled, lys)
{
    lys::__MapLoaderTiled<lys::Vec2i32> loader;
}

TEST(MapLoaderTiled, other)
{
    lys::__MapLoaderTiled<Vi> loader;
}
