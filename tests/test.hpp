#ifndef _TEST_HPP
#define _TEST_HPP 1

#include <gtest/gtest.h>

#include <lys-map-data.hpp>
#include <lys-map-loader-tiled-1.9.hpp>
#include <lys-map-loader-tiled.hpp>
#include <lys-tileset.hpp>

struct Vf
{
    float x, y;
};

struct Vu
{
    uint16_t x, y;
};

struct Vi
{
    uint16_t x, y;
};

struct C
{
    uint8_t r, g, b, a;
};

#endif  // _TEST_HPP
