// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.memory_reader;

namespace util {

MemoryReader::MemoryReader(const uint8_t* ptr_, int64_t count_) : ptr(ptr_), pos(ptr), count(count_)
{
}

uint8_t MemoryReader::ReadByte()
{
    if (pos - ptr >= count)
    {
        throw std::runtime_error("memory reader: unexpected end of data");
    }
    return *pos++;
}

bool MemoryReader::ReadBool()
{
    uint8_t x = ReadByte();
    return x != 0;
}

int8_t MemoryReader::ReadSByte()
{
    return static_cast<int8_t>(ReadByte());
}

uint16_t MemoryReader::ReadUShort()
{
    uint8_t b0 = ReadByte();
    uint8_t b1 = ReadByte();
    return (static_cast<uint16_t>(b0) << 8u) | static_cast<uint16_t>(b1);
}

int16_t MemoryReader::ReadShort()
{
    return static_cast<int16_t>(ReadUShort());
}

uint32_t MemoryReader::ReadUInt()
{
    uint8_t b0 = ReadByte();
    uint8_t b1 = ReadByte();
    uint8_t b2 = ReadByte();
    uint8_t b3 = ReadByte();
    return (static_cast<uint32_t>(b0) << 24u) | (static_cast<uint32_t>(b1) << 16u) | (static_cast<uint32_t>(b2) << 8u) | static_cast<uint32_t>(b3);
}

int32_t MemoryReader::ReadInt()
{
    return static_cast<int32_t>(ReadUInt());
}

uint64_t MemoryReader::ReadULong()
{
    uint8_t b0 = ReadByte();
    uint8_t b1 = ReadByte();
    uint8_t b2 = ReadByte();
    uint8_t b3 = ReadByte();
    uint8_t b4 = ReadByte();
    uint8_t b5 = ReadByte();
    uint8_t b6 = ReadByte();
    uint8_t b7 = ReadByte();
    return (static_cast<uint64_t>(b0) << 56u) | (static_cast<uint64_t>(b1) << 48u) | (static_cast<uint64_t>(b2) << 40u) | (static_cast<uint64_t>(b3) << 32u) |
        (static_cast<uint64_t>(b4) << 24u) | (static_cast<uint64_t>(b5) << 16u) | (static_cast<uint64_t>(b6) << 8u) | static_cast<uint64_t>(b7);
}

int64_t MemoryReader::ReadLong()
{
    return static_cast<int64_t>(ReadULong());
}

float MemoryReader::ReadFloat()
{
    uint32_t x = ReadUInt();
    return *reinterpret_cast<float*>(&x);
}

double MemoryReader::ReadDouble()
{
    uint64_t x = ReadULong();
    return *reinterpret_cast<double*>(&x);
}

char MemoryReader::ReadChar()
{
    uint8_t x = ReadByte();
    return static_cast<char>(x);
}

char16_t MemoryReader::ReadWChar()
{
    uint16_t x = ReadUShort();
    return static_cast<char16_t>(x);
}

char32_t MemoryReader::ReadUChar()
{
    uint32_t x = ReadUInt();
    return static_cast<char32_t>(x);
}

Date MemoryReader::ReadDate()
{
    int16_t year = ReadShort();
    Month month = static_cast<Month>(ReadSByte());
    int8_t day = ReadSByte();
    Date date(year, month, day);
    return date;
}

DateTime MemoryReader::ReadDateTime()
{
    Date date = ReadDate();
    int32_t secs = ReadInt();
    DateTime dt(date, secs);
    return dt;
}

std::string MemoryReader::ReadString()
{
    std::string result;
    uint8_t b = ReadByte();
    while (b != 0u)
    {
        result.append(1, static_cast<char>(b));
        b = ReadByte();
    }
    return result;
}

uuid MemoryReader::ReadUuid()
{
    uuid x;
    for (auto& b : x)
    {
        b = ReadByte();
    }
    return x;
}

uint32_t MemoryReader::ReadULEB128UInt()
{
    uint32_t result = 0;
    uint32_t shift = 0;
    while (true)
    {
        uint8_t b = ReadByte();
        result |= ((b & 0x7F) << shift);
        if ((b & 0x80) == 0) break;
        shift += 7;
    }
    return result;
}

} // namespace util
