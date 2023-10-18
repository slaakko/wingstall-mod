// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.deflate_stream;

import util.stream;
import util.compression;
import std.core;

export namespace util {

const int defaultDeflateCompressionLevel = -1;
const int noDeflateCompression = 0;
const int fastestDeflateCompression = 1;
const int optimalDeflateCompression = 9;
const int Z_FINISH = 4;
const int Z_STREAM_END = 1;
const int Z_NO_FLUSH = 0;

class DeflateStream : public Stream
{
public:
    DeflateStream(CompressionMode mode_, Stream& underlyingStream_);
    DeflateStream(CompressionMode mode_, Stream& underlyingStream_, int64_t bufferSize_);
    DeflateStream(CompressionMode mode_, Stream& underlyingStream_, int64_t bufferSize_, int compressionLevel_);
    ~DeflateStream() override;
    int ReadByte() override;
    int64_t Read(uint8_t* buf, int64_t count) override;
    void Write(uint8_t x) override;
    void Write(uint8_t* buf, int64_t count) override;
private:
    void Finish();
    CompressionMode mode;
    Stream& underlyingStream;
    int64_t bufferSize;
    int compressionLevel;
    uint32_t inAvail;
    std::unique_ptr<uint8_t[]> in;
    uint32_t outAvail;
    int64_t outPos;
    uint32_t outHave;
    bool endOfInput;
    bool endOfStream;
    std::unique_ptr<uint8_t[]> out;
    void* handle;
};

} // namespace util
