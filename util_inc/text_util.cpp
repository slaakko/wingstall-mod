// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <util_inc/text_util.hpp>
#include <memory>
#include <Windows.h>
#include <sstream>

namespace util_inc {

UnicodeException::UnicodeException(const std::string& message_) : std::runtime_error(message_)
{
}

void ThrowUnicodeException(const std::string& message_)
{
    throw UnicodeException(message_);
}

void ThrowInvalidUtf8Sequence()
{
    throw UnicodeException("invalid UTF-8 sequence");
}

std::u32string ToUtf32(const std::string& utf8Str)
{
    std::u32string result;
    const char* p = utf8Str.c_str();
    int bytesRemaining = int(utf8Str.length());
    while (bytesRemaining > 0)
    {
        char8_t c = *p;
        uint8_t x = static_cast<uint8_t>(c);
        if ((x & 0x80u) == 0u)
        {
            result.append(1, static_cast<char32_t>(static_cast<uint32_t>(x)));
            --bytesRemaining;
            ++p;
        }
        else if ((x & 0xE0u) == 0xC0u)
        {
            if (bytesRemaining < 2)
            {
                ThrowInvalidUtf8Sequence();
            }
            char32_t u = static_cast<char32_t>(static_cast<uint32_t>(0u));
            uint8_t b1 = static_cast<uint8_t>(p[1]);
            if ((b1 & 0xC0u) != 0x80u)
            {
                ThrowInvalidUtf8Sequence();
            }
            uint8_t shift = 0u;
            for (uint8_t i = 0u; i < 6u; ++i)
            {
                uint8_t bit = b1 & 1u;
                b1 = b1 >> 1u;
                u = static_cast<char32_t>(static_cast<uint32_t>(u) | (static_cast<uint32_t>(bit) << shift));
                ++shift;
            }
            uint8_t b0 = x;
            for (uint8_t i = 0u; i < 5u; ++i)
            {
                uint8_t bit = b0 & 1u;
                b0 = b0 >> 1u;
                u = static_cast<char32_t>(static_cast<uint32_t>(u) | (static_cast<uint32_t>(bit) << shift));
                ++shift;
            }
            result.append(1, u);
            bytesRemaining = bytesRemaining - 2;
            p = p + 2;
        }
        else if ((x & 0xF0u) == 0xE0u)
        {
            if (bytesRemaining < 3)
            {
                ThrowInvalidUtf8Sequence();
            }
            char32_t u = static_cast<char32_t>(static_cast<uint32_t>(0u));
            uint8_t b2 = static_cast<uint8_t>(p[2]);
            if ((b2 & 0xC0u) != 0x80u)
            {
                ThrowInvalidUtf8Sequence();
            }
            uint8_t shift = 0u;
            for (uint8_t i = 0u; i < 6u; ++i)
            {
                uint8_t bit = b2 & 1u;
                b2 = b2 >> 1u;
                u = static_cast<char32_t>(static_cast<uint32_t>(u) | (static_cast<uint32_t>(bit) << shift));
                ++shift;
            }
            uint8_t b1 = static_cast<uint8_t>(p[1]);
            if ((b1 & 0xC0u) != 0x80u)
            {
                ThrowInvalidUtf8Sequence();
            }
            for (uint8_t i = 0u; i < 6u; ++i)
            {
                uint8_t bit = b1 & 1u;
                b1 = b1 >> 1u;
                u = static_cast<char32_t>(static_cast<uint32_t>(u) | (static_cast<uint32_t>(bit) << shift));
                ++shift;
            }
            uint8_t b0 = x;
            for (uint8_t i = 0u; i < 4u; ++i)
            {
                uint8_t bit = b0 & 1u;
                b0 = b0 >> 1u;
                u = static_cast<char32_t>(static_cast<uint32_t>(u) | (static_cast<uint32_t>(bit) << shift));
                ++shift;
            }
            result.append(1, u);
            bytesRemaining = bytesRemaining - 3;
            p = p + 3;
        }
        else if ((x & 0xF8u) == 0xF0u)
        {
            if (bytesRemaining < 4)
            {
                ThrowInvalidUtf8Sequence();
            }
            char32_t u = static_cast<char32_t>(static_cast<uint32_t>(0u));
            uint8_t b3 = static_cast<uint8_t>(p[3]);
            if ((b3 & 0xC0u) != 0x80u)
            {
                ThrowInvalidUtf8Sequence();
            }
            uint8_t shift = 0u;
            for (uint8_t i = 0u; i < 6u; ++i)
            {
                uint8_t bit = b3 & 1u;
                b3 = b3 >> 1u;
                u = static_cast<char32_t>(static_cast<uint32_t>(u) | (static_cast<uint32_t>(bit) << shift));
                ++shift;
            }
            uint8_t b2 = static_cast<uint8_t>(p[2]);
            if ((b2 & 0xC0u) != 0x80u)
            {
                ThrowInvalidUtf8Sequence();
            }
            for (uint8_t i = 0u; i < 6u; ++i)
            {
                uint8_t bit = b2 & 1u;
                b2 = b2 >> 1u;
                u = static_cast<char32_t>(static_cast<uint32_t>(u) | (static_cast<uint32_t>(bit) << shift));
                ++shift;
            }
            uint8_t b1 = static_cast<uint8_t>(p[1]);
            if ((b1 & 0xC0u) != 0x80u)
            {
                ThrowInvalidUtf8Sequence();
            }
            for (uint8_t i = 0u; i < 6u; ++i)
            {
                uint8_t bit = b1 & 1u;
                b1 = b1 >> 1u;
                u = static_cast<char32_t>(static_cast<uint32_t>(u) | (static_cast<uint32_t>(bit) << shift));
                ++shift;
            }
            uint8_t b0 = x;
            for (uint8_t i = 0u; i < 3u; ++i)
            {
                uint8_t bit = b0 & 1u;
                b0 = b0 >> 1u;
                u = static_cast<char32_t>(static_cast<uint32_t>(u) | (static_cast<uint32_t>(bit) << shift));
                ++shift;
            }
            result.append(1, u);
            bytesRemaining = bytesRemaining - 4;
            p = p + 4;
        }
        else
        {
            ThrowInvalidUtf8Sequence();
        }
    }
    return result;
}

std::u32string ToUtf32(const std::u16string& utf16Str)
{
    std::u32string result;
    const char16_t* w = utf16Str.c_str();
    int remaining = int(utf16Str.length());
    while (remaining > 0)
    {
        char16_t w1 = *w++;
        --remaining;
        if (static_cast<uint16_t>(w1) < 0xD800u || static_cast<uint16_t>(w1) > 0xDFFFu)
        {
            result.append(1, w1);
        }
        else
        {
            if (static_cast<uint16_t>(w1) < 0xD800u || static_cast<uint16_t>(w1) > 0xDBFFu)
            {
                ThrowUnicodeException("invalid UTF-16 sequence");
            }
            if (remaining > 0)
            {
                char16_t w2 = *w++;
                --remaining;
                if (static_cast<uint16_t>(w2) < 0xDC00u || static_cast<uint16_t>(w2) > 0xDFFFu)
                {
                    ThrowUnicodeException("invalid UTF-16 sequence");
                }
                else
                {
                    char32_t uprime = static_cast<char32_t>(((0x03FFu & static_cast<uint32_t>(w1)) << 10u) | (0x03FFu & static_cast<uint32_t>(w2)));
                    char32_t u = static_cast<char32_t>(static_cast<uint32_t>(uprime) + 0x10000u);
                    result.append(1, u);
                }
            }
            else
            {
                ThrowUnicodeException("invalid UTF-16 sequence");
            }
        }
    }
    return result;
}


std::u16string ToUtf16(const std::u32string& utf32Str)
{
    std::u16string result;
    for (char32_t u : utf32Str)
    {
        if (static_cast<uint32_t>(u) > 0x10FFFFu)
        {
            ThrowUnicodeException("invalid UTF-32 code point");
        }
        if (static_cast<uint32_t>(u) < 0x10000u)
        {
            if (static_cast<uint32_t>(u) >= 0xD800 && static_cast<uint32_t>(u) <= 0xDFFF)
            {
                ThrowUnicodeException("invalid UTF-32 code point (reserved for UTF-16)");
            }
            char16_t x = static_cast<char16_t>(u);
            result.append(1, x);
        }
        else
        {
            char32_t uprime = static_cast<char32_t>(static_cast<uint32_t>(u) - 0x10000u);
            char16_t w1 = static_cast<char16_t>(0xD800u);
            char16_t w2 = static_cast<char16_t>(0xDC00u);
            for (uint16_t i = 0u; i < 10u; ++i)
            {
                uint16_t bit = static_cast<uint16_t>(static_cast<uint32_t>(uprime) & (static_cast<uint32_t>(0x1u) << i));
                w2 = static_cast<char16_t>(static_cast<uint16_t>(w2) | bit);
            }
            for (uint16_t i = 10u; i < 20u; ++i)
            {
                uint16_t bit = static_cast<uint16_t>((static_cast<uint32_t>(uprime) & (static_cast<uint32_t>(0x1u) << i)) >> 10u);
                w1 = static_cast<char16_t>(static_cast<uint16_t>(w1) | bit);
            }
            result.append(1, w1);
            result.append(1, w2);
        }
    }
    return result;

}

std::u16string ToUtf16(const std::string& utf8Str)
{
    return ToUtf16(ToUtf32(utf8Str));
}

std::string ToUtf8(const std::u32string& utf32Str)
{
    std::string result;
    for (char32_t c : utf32Str)
    {
        uint32_t x = static_cast<uint32_t>(c);
        if (x < 0x80u)
        {
            result.append(1, static_cast<char>(x & 0x7Fu));
        }
        else if (x < 0x800u)
        {
            uint8_t b1 = 0x80u;
            for (uint8_t i = 0u; i < 6u; ++i)
            {
                b1 = b1 | (static_cast<uint8_t>(x & 1u) << i);
                x = x >> 1u;
            }
            uint8_t b0 = 0xC0u;
            for (uint8_t i = 0u; i < 5u; ++i)
            {
                b0 = b0 | (static_cast<uint8_t>(x & 1u) << i);
                x = x >> 1u;
            }
            result.append(1, static_cast<char8_t>(b0));
            result.append(1, static_cast<char8_t>(b1));
        }
        else if (x < 0x10000u)
        {
            uint8_t b2 = 0x80u;
            for (uint8_t i = 0u; i < 6u; ++i)
            {
                b2 = b2 | (static_cast<uint8_t>(x & 1u) << i);
                x = x >> 1u;
            }
            uint8_t b1 = 0x80u;
            for (uint8_t i = 0u; i < 6u; ++i)
            {
                b1 = b1 | (static_cast<uint8_t>(x & 1u) << i);
                x = x >> 1u;
            }
            uint8_t b0 = 0xE0u;
            for (uint8_t i = 0u; i < 4u; ++i)
            {
                b0 = b0 | (static_cast<uint8_t>(x & 1u) << i);
                x = x >> 1u;
            }
            result.append(1, static_cast<char8_t>(b0));
            result.append(1, static_cast<char8_t>(b1));
            result.append(1, static_cast<char8_t>(b2));
        }
        else if (x < 0x110000u)
        {
            uint8_t b3 = 0x80u;
            for (uint8_t i = 0u; i < 6u; ++i)
            {
                b3 = b3 | (static_cast<uint8_t>(x & 1u) << i);
                x = x >> 1u;
            }
            uint8_t b2 = 0x80u;
            for (uint8_t i = 0u; i < 6u; ++i)
            {
                b2 = b2 | (static_cast<uint8_t>(x & 1u) << i);
                x = x >> 1u;
            }
            uint8_t b1 = 0x80u;
            for (uint8_t i = 0u; i < 6u; ++i)
            {
                b1 = b1 | (static_cast<uint8_t>(x & 1u) << i);
                x = x >> 1u;
            }
            uint8_t b0 = 0xF0u;
            for (uint8_t i = 0u; i < 3u; ++i)
            {
                b0 = b0 | (static_cast<uint8_t>(x & 1u) << i);
                x = x >> 1u;
            }
            result.append(1, static_cast<char8_t>(b0));
            result.append(1, static_cast<char8_t>(b1));
            result.append(1, static_cast<char8_t>(b2));
            result.append(1, static_cast<char8_t>(b3));
        }
        else
        {
            ThrowUnicodeException("invalid UTF-32 code point");
        }
    }
    return result;
}

std::string ToUtf8(const std::u16string& utf16Str)
{
    return ToUtf8(ToUtf32(utf16Str));
}

std::string PlatformStringToUtf8(const std::string& platformString)
{
    if (platformString.empty()) return std::string();
    int bufSize = 4096;
    std::unique_ptr<char16_t> wbuf(new char16_t[bufSize]);
    int result = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, platformString.c_str(), -1, (LPWSTR)wbuf.get(), bufSize);
    if (result == 0)
    {
        return platformString; // conversion failed, maybe it's UTF-8 already
    }
    return ToUtf8(wbuf.get());
}

inline char HexNibble(uint8_t n)
{
    static const char* h = "0123456789ABCDEF";
    return h[n];
}

std::string ToHexString(uint8_t x)
{
    std::string s;
    s.append(1, HexNibble(x >> 4)).append(1, HexNibble(x & 0x0F));
    return s;
}

std::string ToHexString(uint16_t x)
{
    std::string s;
    s.append(ToHexString(uint8_t((x >> 8) & 0xFF)));
    s.append(ToHexString(uint8_t((x & 0xFF))));
    return s;
}

std::string ToHexString(uint32_t x)
{
    std::string s;
    s.append(ToHexString(uint8_t((x >> 24) & 0xFF)));
    s.append(ToHexString(uint8_t((x >> 16) & 0xFF)));
    s.append(ToHexString(uint8_t((x >> 8) & 0xFF)));
    s.append(ToHexString(uint8_t((x & 0xFF))));
    return s;
}

std::string ToHexString(uint64_t x)
{
    std::string s;
    s.append(ToHexString(uint8_t((x >> 56) & 0xFF)));
    s.append(ToHexString(uint8_t((x >> 48) & 0xFF)));
    s.append(ToHexString(uint8_t((x >> 40) & 0xFF)));
    s.append(ToHexString(uint8_t((x >> 32) & 0xFF)));
    s.append(ToHexString(uint8_t((x >> 24) & 0xFF)));
    s.append(ToHexString(uint8_t((x >> 16) & 0xFF)));
    s.append(ToHexString(uint8_t((x >> 8) & 0xFF)));
    s.append(ToHexString(uint8_t((x & 0xFF))));
    return s;
}

std::string ToUpperNarrow(const std::string& s)
{
    std::string result;
    int n = int(s.size());
    result.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        result.append(1, std::toupper(s[i]));
    }
    return result;
}

std::string ToLowerNarrow(const std::string& s)
{
    std::string result;
    int n = int(s.size());
    result.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        result.append(1, std::tolower(s[i]));
    }
    return result;
}

uint8_t ParseHexByte(const std::string& hexByteStr)
{
    std::string hex;
    if (hexByteStr.starts_with("0x") || hexByteStr.starts_with("0X"))
    {
        hex = hexByteStr;
    }
    else
    {
        hex = "0x" + hexByteStr;
    }
    std::stringstream s;
    s.str(hex);
    uint64_t value = 0;
    s >> std::hex >> value;
    return static_cast<uint8_t>(value);
}

} // namespace util_inc
