#pragma once

#include "config.hpp"
#include <array>
#include <cstdint>

// ----------------------------------------------------------------------------------------------------
// TYPES
// ----------------------------------------------------------------------------------------------------

// SIGNED
typedef std::int8_t i8;
typedef std::int16_t i16;
typedef std::int32_t i32;
typedef std::int64_t i64;

// UNSIGNED
typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

// FLOATS
typedef float f32;
typedef double f64;

// ----------------------------------------------------------------------------------------------------
// DEFINES
// ----------------------------------------------------------------------------------------------------

#define FAILURE -1
#define SUCCESS 0

constexpr i32 win_width = 1920;
constexpr i32 win_height = 1080;
constexpr const char *win_title = "sfml_win";
constexpr i32 targetFPS = 0; // 0 => unlimited

extern f32 zoom;
constexpr f32 zoomStep = 0.1f;

constexpr std::array<const char *, 5> imgPaths = {
    ASSETS_PATH "cpp256.png",     ASSETS_PATH "cpp512.png", ASSETS_PATH "7300_8_05.jpg",
    ASSETS_PATH "chibi_ashe.png", ASSETS_PATH "boobs.png",
};

extern u8 idx;
extern bool isRepulsing;

constexpr f32 repulsion_radius = 1000.f;
constexpr f32 repulsion_strength = 1500.f;
constexpr f32 return_force = 0.15f;
constexpr f32 friction = 0.99f;
constexpr u32 gap = 1;
