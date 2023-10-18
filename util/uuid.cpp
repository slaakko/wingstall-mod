// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.uuid;

import util.text_util;
import util.rand;

namespace util {

uuid::uuid()
{
    for (auto& byte : data)
    {
        byte = 0;
    }
}

uuid::uuid(const uuid& that)
{
    for (int i = 0; i < uuid::static_size(); ++i)
    {
        data[i] = that.data[i];
    }
}

uuid::uuid(uuid&& that)
{
    for (int i = 0; i < uuid::static_size(); ++i)
    {
        data[i] = that.data[i];
    }
}

uuid& uuid::operator=(const uuid& that)
{
    for (int i = 0; i < uuid::static_size(); ++i)
    {
        data[i] = that.data[i];
    }
    return *this;
}

uuid& uuid::operator=(uuid&& that)
{
    for (int i = 0; i < uuid::static_size(); ++i)
    {
        data[i] = that.data[i];
    }
    return *this;
}

uuid uuid::random()
{
    uuid rand_uuid;
    for (int i = 0; i < uuid::static_size(); ++i)
    {
        rand_uuid.data[i] = get_random_byte();
    }
    return rand_uuid;
}

bool uuid::is_nil() const
{
    for (const auto& x : data)
    {
        if (x != 0) return false;
    }
    return true;
}

bool operator==(const uuid& left, const uuid& right)
{
    for (int i = 0; i < uuid::static_size(); ++i)
    {
        if (left.data[i] != right.data[i]) return false;
    }
    return true;
}

bool operator<(const uuid& left, const uuid& right)
{
    for (int i = 0; i < uuid::static_size(); ++i)
    {
        if (left.data[i] < right.data[i]) return true;
        if (left.data[i] > right.data[i]) return false;
    }
    return false;
}

void UuidToInts(const uuid& id, uint64_t& int1, uint64_t& int2)
{
    const uint8_t* i = &id.data[0];
    const uint64_t* i64 = reinterpret_cast<const uint64_t*>(i);
    int1 = *i64++;
    int2 = *i64;
}

void IntsToUuid(uint64_t int1, uint64_t int2, uuid& id)
{
    uint8_t* i = &id.data[0];
    uint64_t* i64 = reinterpret_cast<uint64_t*>(i);
    *i64++ = int1;
    *i64 = int2;
}

void RandomUuid(uuid& id)
{
    id = uuid::random();
}

std::string ToString(const uuid& uuid)
{
    std::string s;
    int index = 0;
    for (uint8_t x : uuid)
    {
        s.append(ToLowerNarrow(ToHexString(x)));
        if (index == 3 || index == 5 || index == 7 || index == 9)
        {
            s.append(1, '-');
        }
        ++index;
    }
    return s;
}

uuid ParseUuid(const std::string& str)
{
    if (str.length() != 2 * uuid::static_size() + 4)
    {
        throw std::runtime_error("wrong number of hex bytes in uuid string '" + str + "'." + std::to_string(uuid::static_size()) + " hex bytes + 4 hyphens expected.");
    }
    uuid uuid;
    int index = 0;
    for (long i = 0; i < uuid::static_size(); ++i)
    {
        std::string hexByteStr = str.substr(index, 2);
        uint8_t hexByte = ParseHexByte(hexByteStr);
        uuid.data[i] = hexByte;
        ++index;
        ++index;
        if (i == 3 || i == 5 || i == 7 || i == 9)
        {
            ++index;
        }
    }
    return uuid;
}

std::size_t HashValue(const uuid& u)
{
    std::size_t seed = 0;
    for (auto x : u)
    {
        seed ^= static_cast<std::size_t>(x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

std::size_t UuidHash::operator()(const uuid& u) const
{
    return HashValue(u);
}

} // namespace util
