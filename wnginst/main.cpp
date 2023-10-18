// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

import rex;
import util;
import std.filesystem;
import std.core;

std::string version()
{
    return "1.1.0";
}

void PrintHelp()
{
    std::cout << "wnginst file installation utility version " << version() << std::endl;
    std::cout << std::endl;
    std::cout << "usage: wnginst [options] (SOURCEFILE DESTFILE | SOURCEFILES... DIR)" << std::endl;
    std::cout << std::endl;
    std::cout << "copies SOURCEFILE to DESTFILE or SOURCEFILES to DIR for each file if:" << std::endl;
    std::cout << "- destination file does not exist or" << std::endl;
    std::cout << "- destination file is older than source file or" << std::endl;
    std::cout << "- forced" << std::endl;
    std::cout << std::endl;
    std::cout << "wild cards ok for SOURCEFILES" << std::endl;
    std::cout << std::endl;
    std::cout << "options:" << std::endl;
    std::cout << std::endl;
    std::cout << "--verbose | -v:" << std::endl;
    std::cout << "  be verbose" << std::endl;
    std::cout << std::endl;
    std::cout << "--help | -h:" << std::endl;
    std::cout << "  print help and exit" << std::endl;
    std::cout << std::endl;
    std::cout << "--force | -f:" << std::endl;
    std::cout << "  force copy although source file is older than destination file" << std::endl;
    std::cout << std::endl;
    std::cout << "--dir | -d" << std::endl;
    std::cout << "  make destination directory" << std::endl;
    std::cout << std::endl;
}

void InitApplication()
{
    util::Init();
}

int main(int argc, const char** argv)
{
    try
    {
        InitApplication();
        bool verbose = false;
        bool force = false;
        bool makeDir = false;
        std::vector<std::string> paths;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg == "--verbose")
                {
                    verbose = true;
                }
                else if (arg == "--force")
                {
                    force = true;
                }
                else if (arg == "--make-dir")
                {
                    makeDir = true;
                }
                else if (arg == "--help")
                {
                    PrintHelp();
                    return 1;
                }
                else
                {
                    throw std::runtime_error("unknown option '" + arg + "'");
                }
            }
            else if (arg.starts_with("-"))
            {
                std::string options = arg.substr(1);
                for (char o : options)
                {
                    switch (o)
                    {
                    case 'v':
                    {
                        verbose = true;
                        break;
                    }
                    case 'h':
                    {
                        PrintHelp();
                        return 1;
                    }
                    case 'f':
                    {
                        force = true;
                        break;
                    }
                    case 'd':
                    {
                        makeDir = true;
                        break;
                    }
                    default:
                    {
                        throw std::runtime_error("unknown option '-" + std::string(1, o) + "'");
                    }
                    }
                }
            }
            else
            {
                std::string path = util::GetFullPath(arg);
                if (std::filesystem::exists(path) || i == argc - 1 && makeDir)
                {
                    paths.push_back(path);
                }
                else
                {
                    std::string directory = util::Path::GetDirectoryName(path);
                    if (std::filesystem::exists(directory))
                    {
                        std::string fileMask = util::Path::GetFileName(path);
                        std::filesystem::directory_iterator it(directory);
                        while (it != std::filesystem::directory_iterator())
                        {
                            std::filesystem::directory_entry entry(*it);
                            if (std::filesystem::is_regular_file(entry.path()))
                            {
                                std::string fileName = util::Path::GetFileName(entry.path().generic_string());
                                if (rex::FilePatternMatch(util::ToUtf32(fileName), util::ToUtf32(fileMask)))
                                {
                                    std::string path = util::Path::Combine(directory, fileName);
                                    paths.push_back(path);
                                }
                            }
                            ++it;
                        }
                    }
                    else
                    {
                        if (verbose)
                        {
                            std::cout << "source directory '" + directory + "' does not exist" << std::endl;
                        }
                    }
                }
            }
        }
        if (paths.size() < 2)
        {
            PrintHelp();
            return 1;
        }
        if (paths.size() == 2 && std::filesystem::is_regular_file(paths.front()) && !makeDir)
        {
            std::string source = util::GetFullPath(paths.front());
            std::string dest = util::GetFullPath(paths.back());
            util::CopyFile(source, dest, force, makeDir, verbose);
        }
        else if (std::filesystem::is_directory(paths.back()) || makeDir)
        {
            std::string destDir = paths.back();
            for (int i = 0; i < paths.size() - 1; ++i)
            {
                std::string source = util::GetFullPath(paths[i]);
                std::string dest = util::GetFullPath(util::Path::Combine(destDir, util::Path::GetFileName(source)));
                util::CopyFile(source, dest, force, makeDir, verbose);
            }
        }
        else
        {
            PrintHelp();
            return 1;
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
