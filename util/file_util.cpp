// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.file_util;

import util.file_stream;
import util.buffered_stream;
import util.binary_stream_reader;
import util.binary_stream_writer;
import util.path;
import util.text_util;
import std.filesystem;

namespace util {

void CoutCopyFileObserver::WriteLine(const std::string& line)
{
    std::cout << line << std::endl;
}

void CopyFile(const std::string& source, const std::string& dest, bool force, bool makeDir, bool verbose)
{
    CoutCopyFileObserver observer;
    CopyFile(source, dest, force, makeDir, verbose, &observer);
}

void CopyFile(const std::string& source, const std::string& dest, bool force, bool makeDir, bool verbose, CopyFileObserver* observer)
{
    if (!std::filesystem::exists(source))
    {
        if (verbose && observer)
        {
            observer->WriteLine("source file '" + source + "' does not exist");
        }
        return;
    }
    if (makeDir)
    {
        std::error_code ec;
        std::filesystem::create_directories(Path::GetDirectoryName(dest), ec);
        if (ec)
        {
            throw std::runtime_error("could not create directory '" + dest + "': " + PlatformStringToUtf8(ec.message()));
        }
    }
    if (force || !std::filesystem::exists(dest) || std::filesystem::last_write_time(source) > std::filesystem::last_write_time(dest))
    {
        int64_t size = std::filesystem::file_size(source);
        if (observer)
        {
            observer->FileCopyProgress(0, size);
        }
        {
            FileStream sourceFile(source, OpenMode::read | OpenMode::binary);
            BufferedStream bufferedSource(sourceFile);
            BinaryStreamReader reader(bufferedSource);
            FileStream destFile(dest, OpenMode::write | OpenMode::binary);
            BufferedStream bufferedDest(destFile);
            BinaryStreamWriter writer(bufferedDest);
            for (int64_t i = 0; i < size; ++i)
            {
                uint8_t x = reader.ReadByte();
                writer.Write(x);
                if (observer)
                {
                    observer->FileCopyProgress(i, size);
                }
            }
        }
        std::error_code ec;
        std::filesystem::last_write_time(dest, std::filesystem::last_write_time(source), ec);
        if (ec)
        {
            throw std::runtime_error("could not set write time of file '" + dest + "': " + PlatformStringToUtf8(ec.message()));
        }
        if (verbose && observer)
        {
            observer->WriteLine(source + " -> " + dest);
        }
    }
}

} // namespace util
