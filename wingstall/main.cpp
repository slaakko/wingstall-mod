// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

import wing_package;
import wing;
import dom;
import xpath;
import xml;
import util;
import std.filesystem;
import std.core;

void InitApplication()
{
    util::Init();
    wing::Init(nullptr);
}

enum class Command
{
    none, createPackage, installPackage, makeSetup, installPackageFromVector, setCompression, setContent
};

std::string WingstallVersionStr()
{
    return "2.0.0";
}

void PrintHelp()
{
    std::cout << "wingstall installer generator version " << WingstallVersionStr() << std::endl;
    std::cout << "usage: wingstall OPTIONS" << std::endl;
    std::cout << "OPTIONS:" << std::endl;
    std::cout << "--verbose (-v)" << std::endl;
    std::cout << "  Be verbose." << std::endl;
    std::cout << "--help (-h)" << std::endl;
    std::cout << "  Print help and exit." << std::endl;
    std::cout << "--create-package (-c) PACKAGE.package.xml" << std::endl;
    std::cout << "  Create binary package PACKAGE.package.bin, package info file PACKAGE.package.info.xml and package index PACKAGE.index.xml from package description file PACKAGE.package.xml." << std::endl;
    std::cout << "--make-setup (-m) PACKAGE.bin" << std::endl;
    std::cout << "  Create Visual C++ setup program from PACKAGE.package.bin and package info file PACKAGE.package.info.xml." << std::endl;
}

class PackageFileContentPositionObserver : public wing_package::PackageObserver
{
public:
    enum class Kind
    {
        read, write
    };
    PackageFileContentPositionObserver(Kind kind_, bool verbose_);
    void StatusChanged(wing_package::Package* package) override;
    void LogError(wing_package::Package* package, const std::string& error) override;
    void FileContentPositionChanged(wing_package::Package* package) override;
private:
    Kind kind;
    bool verbose;
    bool firstFileContent;
    bool inFileContent;
    int numBackspaces;
    int prevPercent;
};

PackageFileContentPositionObserver::PackageFileContentPositionObserver(Kind kind_, bool verbose_) :
    kind(kind_), verbose(verbose_), numBackspaces(0), prevPercent(-1), firstFileContent(true), inFileContent(false)
{
}

void PackageFileContentPositionObserver::StatusChanged(wing_package::Package* package)
{
    if (verbose)
    {
        if (inFileContent)
        {
            inFileContent = false;
            std::cout << std::endl;
        }
        std::cout << package->GetStatusStr() << std::endl;
        if (package->GetStatus() == wing_package::Status::failed)
        {
            std::cout << package->GetErrorMessage() << std::endl;
        }
    }
}

void PackageFileContentPositionObserver::LogError(wing_package::Package* package, const std::string& error)
{
    std::cout << package->GetErrorMessage() << std::endl;
}

void PackageFileContentPositionObserver::FileContentPositionChanged(wing_package::Package* package)
{
    if (verbose)
    {
        if (firstFileContent)
        {
            firstFileContent = false;
            if (kind == Kind::write)
            {
                std::cout << "writing files ";
            }
            else if (kind == Kind::read)
            {
                std::cout << "reading files ";
            }
        }
        int percent = 0;
        if (package->FileContentPosition() == package->FileContentSize())
        {
            percent = 100;
        }
        else
        {
            percent = static_cast<int>((100.0 * package->FileContentPosition()) / package->FileContentSize());
        }
        if (percent != prevPercent)
        {
            util::WritePercent(std::cout, percent, numBackspaces);
            prevPercent = percent;
        }
        inFileContent = true;
    }
}

int main(int argc, const char** argv)
{
    try
    {
        InitApplication();
        Command command = Command::none;
        bool verbose = false;
        std::vector<std::string> packagesToCreate;
        std::vector<std::string> packagesToInstall;
        std::vector<std::string> packagesToInstallFromVec;
        std::vector<std::string> setupsToCreate;
        wing_package::Content content = wing_package::Content::all;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg == "--verbose")
                {
                    verbose = true;
                }
                else if (arg == "--help")
                {
                    PrintHelp();
                    return 1;
                }
                else if (arg == "--create-package")
                {
                    command = Command::createPackage;
                }
                else if (arg == "--install-package")
                {
                    command = Command::installPackage;
                }
                else if (arg == "--install-package-from-vector")
                {
                    command = Command::installPackageFromVector;
                }
                else if (arg == "--make-setup")
                {
                    command = Command::makeSetup;
                }
                else if (arg == "--compression")
                {
                    command = Command::setCompression;
                }
                else if (arg == "--content")
                {
                    command = Command::setContent;
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
                    case 'c':
                    {
                        command = Command::createPackage;
                        break;
                    }
                    case 'i':
                    {
                        command = Command::installPackage;
                        break;
                    }
                    case 'e':
                    {
                        command = Command::installPackageFromVector;
                        break;
                    }
                    case 'm':
                    {
                        command = Command::makeSetup;
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
                switch (command)
                {
                case Command::createPackage:
                {
                    packagesToCreate.push_back(util::GetFullPath(arg));
                    break;
                }
                case Command::installPackage:
                {
                    packagesToInstall.push_back(util::GetFullPath(arg));
                    break;
                }
                case Command::installPackageFromVector:
                {
                    packagesToInstallFromVec.push_back(util::GetFullPath(arg));
                    break;
                }
                case Command::makeSetup:
                {
                    setupsToCreate.push_back(util::GetFullPath(arg));
                    break;
                }
                case Command::setContent:
                {
                    if (arg == "preinstall")
                    {
                        content = wing_package::Content::preinstall;
                    }
                    else if (arg == "index")
                    {
                        content = wing_package::Content::index;
                    }
                    else if (arg == "data")
                    {
                        content = wing_package::Content::data;
                    }
                    else if (arg == "all")
                    {
                        content = wing_package::Content::all;
                    }
                    else
                    {
                        throw std::runtime_error("unknown content type '" + arg + "'");
                    }
                    break;
                }
                case Command::none:
                {
                    throw std::runtime_error("command argument not set");
                }
                }
            }
        }
        for (const std::string& packageXmlFilePath : packagesToCreate)
        {
            if (verbose)
            {
                std::cout << "creating package '" << packageXmlFilePath + "'..." << std::endl;
            }
            std::unique_ptr<xml::Document> packageDoc = xml::ReadDocument(packageXmlFilePath);
            wing_package::PathMatcher pathMatcher(packageXmlFilePath);
            std::unique_ptr<wing_package::Package> package(new wing_package::Package(pathMatcher, packageDoc.get()));
            std::string xmlIndexFilePath = util::Path::ChangeExtension(packageXmlFilePath, ".index.xml");
            package->WriteIndexToXmlFile(xmlIndexFilePath);
            if (verbose)
            {
                std::cout << "==> " << xmlIndexFilePath << std::endl;
            }
            std::string packageBinFilePath = util::Path::ChangeExtension(packageXmlFilePath, ".bin");
            {
                PackageFileContentPositionObserver observer(PackageFileContentPositionObserver::Kind::write, verbose);
                package->AddObserver(&observer);
                package->Create(packageBinFilePath, content);
                package->RemoveObserver(&observer);
            }
            if (verbose)
            {
                std::cout << "==> " << packageBinFilePath << std::endl;
            }
            std::string xmlInfoFilePath = util::Path::ChangeExtension(packageXmlFilePath, ".info.xml");
            package->WriteInfoXmlFile(xmlInfoFilePath);
            if (verbose)
            {
                std::cout << "==> " << xmlInfoFilePath << std::endl;
            }
        }
        for (const std::string& packageBinFilePath : packagesToInstall)
        {
            if (verbose)
            {
                std::cout << "installing package '" << packageBinFilePath << "'..." << std::endl;
            }
            std::unique_ptr<wing_package::Package> package(new wing_package::Package());
            {
                PackageFileContentPositionObserver observer(PackageFileContentPositionObserver::Kind::read, verbose);
                package->AddObserver(&observer);
                package->Install(wing_package::DataSource::file, packageBinFilePath, nullptr, 0, content);
                package->RemoveObserver(&observer);
            }
            std::string xmlIndexFilePath = util::Path::ChangeExtension(packageBinFilePath, ".read.index.xml");
            package->WriteIndexToXmlFile(xmlIndexFilePath);
            if (verbose)
            {
                std::cout << "==> " << xmlIndexFilePath << std::endl;
            }
            if (verbose)
            {
                std::cout << "package '" << packageBinFilePath << "' installed to directory '" << package->TargetRootDir() << "'" << std::endl;
            }
        }
        for (const std::string& packageBinFilePath : packagesToInstallFromVec)
        {
            if (verbose)
            {
                std::cout << "installing package '" << packageBinFilePath << "'..." << std::endl;
            }
            std::unique_ptr<wing_package::Package> package(new wing_package::Package());
            std::vector<uint8_t> vec;
            util::FileStream fileStream(packageBinFilePath, util::OpenMode::read | util::OpenMode::binary);
            util::BufferedStream bufferedStream(fileStream);
            util::BinaryStreamReader reader(bufferedStream);
            int64_t n = std::filesystem::file_size(packageBinFilePath);
            for (int64_t i = 0; i < n; ++i)
            {
                uint8_t x = reader.ReadByte();
                vec.push_back(x);
            }
            package->Install(wing_package::DataSource::memory, std::string(), vec.data(), vec.size(), content);
            std::string xmlIndexFilePath = util::Path::ChangeExtension(packageBinFilePath, ".read.index.xml");
            package->WriteIndexToXmlFile(xmlIndexFilePath);
            if (verbose)
            {
                std::cout << "==> " << xmlIndexFilePath << std::endl;
            }
            if (verbose)
            {
                std::cout << "package '" << packageBinFilePath << "' installed to directory '" << package->TargetRootDir() << "'" << std::endl;
            }
        }
        for (const std::string& packageBinFilePath : setupsToCreate)
        {
            if (verbose)
            {
                std::cout << "creating setup from package '" << packageBinFilePath << "'..." << std::endl;
            }
            if (verbose)
            {
                wing_package::MakeSetup(packageBinFilePath, verbose);
            }
            if (verbose)
            {
                std::cout << "setup for package '" << packageBinFilePath << "' created" << std::endl;
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
