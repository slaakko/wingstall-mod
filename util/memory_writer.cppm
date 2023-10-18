// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.memory_writer;

import std.core;
import util.time;
import util.uuid;

export namespace util {

class MemoryWriter
{
public:
    MemoryWriter(uint8_t* ptr_, int64_t count_);
    void Write(bool x);
    void Write(uint8_t x);
    void Write(int8_t x);
    void Write(uint16_t x);
    void Write(int16_t x);
    void Write(uint32_t x);
    void Write(int32_t x);
    void Write(uint64_t x);
    void Write(int64_t x);
    void Write(float x);
    void Write(double x);
    void Write(char x);
    void Write(char16_t x);
    void Write(char32_t x);
    void Write(Date x);
    void Write(const DateTime& dt);
    void Write(const std::string& str);
    void Write(const uuid& x);
    void WriteULEB128UInt(uint32_t x);
private:
    uint8_t* ptr;
    uint8_t* pos;
    int64_t count;
};

} // namespace util
