// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/zlib_interface.h>

module util.compression;

//  ZLIB:

int32_t RtInitZlib(int32_t mode, int32_t level, void** handle)
{
    return zlib_init(mode, level, handle);
}

void RtDoneZlib(int32_t  mode, void* handle)
{
    zlib_done(mode, handle);
}

void RtSetInputZlib(void* inChunk, uint32_t inAvail, void* handle)
{
    zlib_set_input(inChunk, inAvail, handle);
}

int32_t RtDeflateZlib(void* outChunk, uint32_t outChunkSize, uint32_t* have, uint32_t* outAvail, void* handle, int32_t flush)
{
    return zlib_deflate(outChunk, outChunkSize, have, outAvail, handle, flush);
}

int32_t RtInflateZlib(void* outChunk, uint32_t outChunkSize, uint32_t* have, uint32_t* outAvail, uint32_t* inAvail, void* handle)
{
    return zlib_inflate(outChunk, outChunkSize, have, outAvail, inAvail, handle);
}

const char* RtRetvalStrZlib(int32_t retVal)
{
    return zlib_retval_str(retVal);
}
