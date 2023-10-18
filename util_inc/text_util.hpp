// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef UTIL_TEXT_UTIL_INCLUDED
#define UTIL_TEXT_UTIL_INCLUDED
#include <string>
#include <stdexcept>
#include <vector>

namespace util_inc {

class UnicodeException : public std::runtime_error
{
public:
    UnicodeException(const std::string& message_);
};

std::string PlatformStringToUtf8(const std::string& platformString);
std::string ToHexString(uint8_t x);
std::string ToHexString(uint16_t x);
std::string ToHexString(uint32_t x);
std::string ToHexString(uint64_t x);
std::string ToUpperNarrow(const std::string& s);
std::string ToLowerNarrow(const std::string& s);
uint8_t ParseHexByte(const std::string& hexByteStr);

template<typename StringT>
std::vector<StringT> Split(const StringT& s, typename StringT::value_type c)
{
    std::vector<StringT> v;
    int start = 0;
    int n = int(s.length());
    for (int i = 0; i < n; ++i)
    {
        if (s[i] == c)
        {
            v.push_back(s.substr(start, i - start));
            start = i + 1;
        }
    }
    if (start < n)
    {
        v.push_back(s.substr(start, n - start));
    }
    return v;
}

} // namespace util_inc

#endif // UTIL_TEXT_UTIL_INCLUDED
