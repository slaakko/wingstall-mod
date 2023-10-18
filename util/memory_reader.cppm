// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.memory_reader;

import std.core;
import util.time;
import util.uuid;

export namespace util {

class MemoryReader
{
public:
    MemoryReader(const uint8_t* ptr_, int64_t count_);
    bool ReadBool();
    uint8_t ReadByte();
    int8_t ReadSByte();
    uint16_t ReadUShort();
    int16_t ReadShort();
    uint32_t ReadUInt();
    int32_t ReadInt();
    uint64_t ReadULong();
    int64_t ReadLong();
    float ReadFloat();
    double ReadDouble();
    char ReadChar();
    char16_t ReadWChar();
    char32_t ReadUChar();
    Date ReadDate();
    DateTime ReadDateTime();
    std::string ReadString();
    uuid ReadUuid();
    uint32_t ReadULEB128UInt();
private:
    const uint8_t* ptr;
    const uint8_t* pos;
    int64_t count;
};

} // namespace util
