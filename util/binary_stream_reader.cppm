// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.binary_stream_reader;

import util.uuid;
import util.stream;
import std.core;

export namespace util {

class BinaryStreamReader
{
public:
    BinaryStreamReader(Stream& stream_);
    Stream& GetStream() { return stream; }
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
    std::string ReadUtf8String();
    std::u16string ReadUtf16String();
    std::u32string ReadUtf32String();
    uint32_t ReadULEB128UInt();
    uint64_t ReadULEB128ULong();
    int32_t ReadSLEB128Int();
    int64_t ReadSLEB128Long();
    void ReadUuid(uuid& uuid);
    time_t ReadTime();
    std::chrono::file_clock::time_point ReadFileTime();
    int64_t Position() const { return stream.Position(); }
private:
    Stream& stream;
};

} // namespace util
