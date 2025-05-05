//
// Created by gaetz on 05/05/25.
//

#ifndef DEFINES_HPP
#define DEFINES_HPP
#pragma warning(disable: 4514) // Not interesting: unreferenced inline function has been removed

#include <string>
#include <vector>
#include <array>
#include <memory>

#include "SDL3/SDL_stdinc.h"

/**
 * Types
 */

using u8 = Uint8;
using u16 = Uint16;
using u32 = Uint32;
using u64 = Uint64;

using i8 = Sint8;
using i16 = Sint16;
using i32 = Sint32;
using i64 = Sint64;

using f32 = float;
using f64 = double;

using str = std::string;

template <typename T>
using Vector = std::vector<T>;

template <typename T, size_t N>
using Array = std::array<T, N>;

template<class T>
using uptr = std::unique_ptr<T>;

template<class T>
using sptr = std::shared_ptr<T>;

#endif //DEFINES_HPP
