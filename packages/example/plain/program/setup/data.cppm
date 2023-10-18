export module data;

import std.core;

export namespace setup {

std::string PackageResourceName();

int64_t UncompressedPackageSize();

std::string UnicodeDBResourceName();

int64_t UncompressedUnicodeDBSize();

std::string SetupIconResourceName();

std::string AppName();

std::string AppVersion();

int Compression();

std::string DefaultContainingDirPath();

std::string InstallDirName();

} // setup
