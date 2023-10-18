// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.memory_writer;

namespace util {

MemoryWriter::MemoryWriter(uint8_t* ptr_, int64_t count_) : ptr(ptr_), pos(ptr), count(count_)
{
}

void MemoryWriter::Write(uint8_t x)
{
    if (pos - ptr >= count)
    {
        throw std::runtime_error("memory writer: end of space");
    }
    *pos++ = x;
}

void MemoryWriter::Write(bool x)
{
    Write(x ? static_cast<uint8_t>(1) : static_cast<uint8_t>(0));
}

void MemoryWriter::Write(int8_t x)
{
    Write(static_cast<uint8_t>(x));
}

void MemoryWriter::Write(uint16_t x)
{
    uint8_t b0 = static_cast<uint8_t>(x >> 8u);
    uint8_t b1 = static_cast<uint8_t>(x);
    Write(b0);
    Write(b1);
}

void MemoryWriter::Write(int16_t x)
{
    Write(static_cast<uint16_t>(x));
}

void MemoryWriter::Write(uint32_t x)
{
    uint8_t b0 = static_cast<uint8_t>(x >> 24u);
    uint8_t b1 = static_cast<uint8_t>(x >> 16u);
    uint8_t b2 = static_cast<uint8_t>(x >> 8u);
    uint8_t b3 = static_cast<uint8_t>(x);
    Write(b0);
    Write(b1);
    Write(b2);
    Write(b3);
}

void MemoryWriter::Write(int32_t x)
{
    Write(static_cast<uint32_t>(x));
}

void MemoryWriter::Write(uint64_t x)
{
    uint8_t b0 = static_cast<uint8_t>(x >> 56u);
    uint8_t b1 = static_cast<uint8_t>(x >> 48u);
    uint8_t b2 = static_cast<uint8_t>(x >> 40u);
    uint8_t b3 = static_cast<uint8_t>(x >> 32u);
    uint8_t b4 = static_cast<uint8_t>(x >> 24u);
    uint8_t b5 = static_cast<uint8_t>(x >> 16u);
    uint8_t b6 = static_cast<uint8_t>(x >> 8u);
    uint8_t b7 = static_cast<uint8_t>(x);
    Write(b0);
    Write(b1);
    Write(b2);
    Write(b3);
    Write(b4);
    Write(b5);
    Write(b6);
    Write(b7);
}

void MemoryWriter::Write(int64_t x)
{
    Write(static_cast<uint64_t>(x));
}

void MemoryWriter::Write(float x)
{
    uint32_t* u = reinterpret_cast<uint32_t*>(&x);
    Write(*u);
}

void MemoryWriter::Write(double x)
{
    uint64_t* u = reinterpret_cast<uint64_t*>(&x);
    Write(*u);
}

void MemoryWriter::Write(char x)
{
    Write(static_cast<uint8_t>(x));
}

void MemoryWriter::Write(char16_t x)
{
    Write(static_cast<uint16_t>(x));
}

void MemoryWriter::Write(char32_t x)
{
    Write(static_cast<uint32_t>(x));
}

void MemoryWriter::Write(Date x)
{
    Write(x.Year());
    Write(static_cast<int8_t>(x.GetMonth()));
    Write(x.Day());
}

void MemoryWriter::Write(const DateTime& dt)
{
    Date date = dt.GetDate();
    Write(date);
    Write(dt.Seconds());
}

void MemoryWriter::Write(const std::string& str)
{
    for (char c : str)
    {
        Write(static_cast<uint8_t>(c));
    }
    Write(static_cast<uint8_t>(0u));
}

void MemoryWriter::Write(const uuid& x)
{
    for (auto b : x)
    {
        Write(static_cast<uint8_t>(b));
    }
}

void MemoryWriter::WriteULEB128UInt(uint32_t x)
{
    do
    {
        uint8_t b = x & 0x7F;
        x >>= 7;
        if (x != 0)
        {
            b |= 0x80;
        }
        Write(b);
    } while (x != 0);
}

} // namespace util
