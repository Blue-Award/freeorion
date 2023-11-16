//! GiGi - A GUI for OpenGL
//!
//!  Copyright (C) 2003-2008 T. Zachary Laine <whatwasthataddress@gmail.com>
//!  Copyright (C) 2013-2020 The FreeOrion Project
//!
//! Released under the GNU Lesser General Public License 2.1 or later.
//! Some Rights Reserved.  See COPYING file or https://www.gnu.org/licenses/lgpl-2.1.txt
//! SPDX-License-Identifier: LGPL-2.1-or-later

//! @file GG/Clr.h
//!
//! Contains the utility class Clr, which represents colors in GG.

#ifndef _GG_Clr_h_
#define _GG_Clr_h_

#include <array>
#include <cstdint>
#include <limits>
#include <sstream>
#include <string>
#include <GG/Export.h>


namespace GG {

/** \brief A simple 32-bit structure that can act as a packed 32-bit unsigned
    integer representation of a RGBA color, a vector of the four unsigned
    bytes that compose an RGBA color, or the individual unsigned bytes "a",
    "r", "g", and "b".

    You should not use literals to initialize Color objects; depending on the
    endian-ness of the machine, 0x00FFFFFF would be transparent white
    (little-endian) or opaque yellow (big-endian).*/
struct Clr
{
    [[nodiscard]] constexpr Clr() = default;

    /** ctor that constructs a Clr from four ints that represent the color channels */
    [[nodiscard]] constexpr Clr(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_) noexcept :
        r(r_), g(g_), b(b_), a(a_)
    {}

    /** ctor that constructs a Clr from std::array that represents the color channels */
    [[nodiscard]] constexpr Clr(std::array<uint8_t, 4> clr) noexcept :
        Clr(clr[0], clr[1], clr[2], clr[3])
    {}

    /** ctor that constructs a Clr from a string that represents the color
        channels in the format 'RRGGBB', 'RRGGBBAA' where each channel value
        ranges from 00 to FF. When the alpha component is left out, the alpha
        value FF is assumed. When characters out of the range 0-9 and A-F are
        passed, results are undefined.
    */
    [[nodiscard]] constexpr Clr(std::string_view hex_colour)
    {
        const auto sz = hex_colour.size();
        r = (sz >= 2) ? ValFromTwoHexChars(hex_colour.substr(0, 2)) : 0;
        g = (sz >= 4) ? ValFromTwoHexChars(hex_colour.substr(2, 2)) : 0;
        b = (sz >= 6) ? ValFromTwoHexChars(hex_colour.substr(4, 2)) : 0;
        a = (sz >= 8) ? ValFromTwoHexChars(hex_colour.substr(6, 2)) : 255;
    }

    [[nodiscard]] explicit constexpr operator uint32_t() const noexcept
    { return (r << 24) + (g << 16) + (b << 8) + a; }

    [[nodiscard]] constexpr auto ToCharArray() const noexcept
    {
        //                                 "255"  ' '   0
        std::array<std::string::value_type, 4*3 + 3*1 + 1> buf{};
        auto it = buf.data();

        it = UInt8ToChars(it, r);
        *it++ = ' ';
        it = UInt8ToChars(it, g);
        *it++ = ' ';
        it = UInt8ToChars(it, b);
        *it++ = ' ';
        it = UInt8ToChars(it, a);

        return buf;
    }

    [[nodiscard]] explicit operator std::string() const
    {
        const auto data = ToCharArray();
        return std::string{data.data()};
    }

    [[nodiscard]] constexpr std::array<uint8_t, 4> RGBA() const noexcept
    { return {r, g, b, a}; }

#if defined(__cpp_impl_three_way_comparison)
    [[nodiscard]] constexpr auto operator<=>(const Clr&) const noexcept = default;
#else
    [[nodiscard]] constexpr bool operator==(const Clr& rhs) const noexcept
    { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; };
    [[nodiscard]] constexpr bool operator!=(const Clr& rhs) const noexcept
    { return r != rhs.r || g != rhs.g || b != rhs.b || a != rhs.a; };
#endif

    uint8_t r = 0;    ///< the red channel
    uint8_t g = 0;    ///< the green channel
    uint8_t b = 0;    ///< the blue channel
    uint8_t a = 0;    ///< the alpha channel

    [[nodiscard]] static constexpr uint8_t ValFromTwoHexChars(std::string_view chars) noexcept
    {
        auto digit0 = chars[0];
        auto digit1 = chars[1];
        uint8_t val0 = 16 * (digit0 >= 'A' ? (digit0 - 'A' + 10) : (digit0 - '0'));
        uint8_t val1 = (digit1 >= 'A' ? (digit1 - 'A' + 10) : (digit1 - '0'));
        return val0 + val1;
    };

    static constexpr std::string::value_type* UInt8ToChars(
        std::string::value_type* out_it, const uint8_t num) noexcept
    {
        const uint8_t hundreds = num / 100u;
        const uint8_t less_than_100 = num - hundreds*100u;
        const uint8_t tens = less_than_100 / 10u;
        const uint8_t ones = less_than_100 - tens*10u;
        if (hundreds > 0)
            *out_it++ = (hundreds + '0');
        if (tens > 0 || hundreds > 0)
            *out_it++ = (tens + '0');
        *out_it++ = (ones + '0');
        return out_it;
    };

    [[nodiscard]] static constexpr auto UInt8ToCharArray(uint8_t num) noexcept
    {
        std::array<std::string::value_type, 4> buf{};
        UInt8ToChars(buf.data(), num);
        return buf;
    };
};

namespace ClrStaticTests {
    static_assert(uint32_t{Clr{0,0,0,1}} == 1u);
    static_assert(uint32_t{Clr{0,0,2,3}} == 2*256u + 3u);
    static_assert(uint32_t{Clr{255,1,0,0}} == 256*256*256*255u + 256*256*1u);

    static_assert(Clr("A0FF01") == Clr{160, 255, 1, 255});
    static_assert(Clr("12345678") == Clr{16*1+2, 16*3+4, 16*5+6, 16*7+8});

    static_assert(Clr::ValFromTwoHexChars("01") == 1);
    static_assert(Clr::ValFromTwoHexChars("FF") == 255);
    static_assert(Clr::ValFromTwoHexChars("A0") == 160);
    static_assert(Clr::ValFromTwoHexChars("!.") == 14u);

    using sva4 = std::array<std::string::value_type, 4>;
    constexpr bool ArrEq(sva4 l, sva4 r) noexcept // workaround for operator==(array, array) not being constexpr in C++17
    { return l[0] == r[0] && l[1] == r[1] && l[2] == r[2] && l[3] == r[3]; }
    constexpr bool TestUint8ToCharArray(uint8_t num, sva4 expected_result) noexcept
    { return ArrEq(Clr::UInt8ToCharArray(num), expected_result); }

    static_assert(TestUint8ToCharArray(0, sva4{"0\0\0"}));
    static_assert(TestUint8ToCharArray(1, sva4{"1\0\0"}));
    static_assert(TestUint8ToCharArray(20, sva4{"20\0"}));
    static_assert(TestUint8ToCharArray(21, sva4{"21\0"}));
    static_assert(TestUint8ToCharArray(200, sva4{"200"}));
    static_assert(TestUint8ToCharArray(210, sva4{"210"}));
    static_assert(TestUint8ToCharArray(201, sva4{"201"}));
    static_assert(TestUint8ToCharArray(255, sva4{"255"}));
}

inline std::ostream& operator<<(std::ostream& os, Clr clr)
{
    os << "(" << +clr.r << ", " << +clr.g << ", " << +clr.b << ", " << +clr.a << ")";
    return os;
}

//! Returns the lightened version of color clr.  LightenClr leaves the alpha
//! channel unchanged, and multiplies the other channels by some factor.
constexpr Clr LightenClr(Clr clr, float factor = 2.0) noexcept
{
    return Clr(std::min<uint8_t>(clr.r * factor, 255), std::min<uint8_t>(clr.g * factor, 255),
               std::min<uint8_t>(clr.b * factor, 255), clr.a);
}

//! Returns the darkened version of color clr.  DarkenClr leaves the alpha
//! channel unchanged, and divides the other channels by some factor.
constexpr Clr DarkenClr(const Clr clr, float factor = 2.0) noexcept
{
    return Clr(
        static_cast<uint8_t>(clr.r / factor),
        static_cast<uint8_t>(clr.g / factor),
        static_cast<uint8_t>(clr.b / factor),
        clr.a);
}

constexpr Clr InvertClr(const Clr clr) noexcept {
    constexpr uint8_t MAX{std::numeric_limits<decltype(clr.a)>::max()};
    return Clr(MAX - clr.r, MAX - clr.g, MAX - clr.b, clr.a);
}

constexpr Clr BlendClr(Clr src, Clr dst, float factor) noexcept
{
    return Clr(static_cast<uint8_t>(src.r * factor + dst.r * (1 - factor)),
               static_cast<uint8_t>(src.g * factor + dst.g * (1 - factor)),
               static_cast<uint8_t>(src.b * factor + dst.b * (1 - factor)),
               static_cast<uint8_t>(src.a * factor + dst.a * (1 - factor)));
}

/** Named ctor that constructs a Clr from four floats that represent the color
    channels (each must be >= 0.0 and <= 1.0). */
constexpr Clr FloatClr(float r, float g, float b, float a) noexcept
{
    return Clr(static_cast<uint8_t>(r * 255),
               static_cast<uint8_t>(g * 255),
               static_cast<uint8_t>(b * 255),
               static_cast<uint8_t>(a * 255));
}

/** Returns the input Clr scaned by the input factor \a s. */
constexpr Clr operator*(Clr lhs, float s) noexcept
{
    return Clr(static_cast<uint8_t>(lhs.r * s),
               static_cast<uint8_t>(lhs.g * s),
               static_cast<uint8_t>(lhs.b * s),
               static_cast<uint8_t>(lhs.a * s));
}

/** Returns the component-wise sum of input Clrs. */
constexpr Clr operator+(Clr lhs, Clr rhs) noexcept
{ return Clr(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a); }

}


//! Calls the appropriate version of glColor*() with @a clr.
GG_API void glColor(GG::Clr clr);


#endif
