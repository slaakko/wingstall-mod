#include <Windows.h>

import data;
import wing_gui;
import wing_package;
import wing;
import util;
import std.core;

void InitApplication(HINSTANCE instance)
{
    util::Init();
    wing::Init(instance);
}

int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
    try
    {
        InitApplication(instance);
        std::string currentExecutableName = util::Path::GetFileName(util::GetFullPath(util::GetPathToExecutable()));
        wing::BinaryResourcePtr unicodeDBResource(currentExecutableName, setup::UnicodeDBResourceName());
        util::CharacterTable::Instance().SetDeflateData(unicodeDBResource.Data(), unicodeDBResource.Size(), setup::UncompressedUnicodeDBSize());
        wing::BinaryResourcePtr packageResource(currentExecutableName, setup::PackageResourceName());
        wing_package::SetInfoItem(wing_package::InfoItemKind::appName, new wing_package::StringItem(setup::AppName()));
        wing_package::SetInfoItem(wing_package::InfoItemKind::appVersion, new wing_package::StringItem(setup::AppVersion()));
        wing_package::SetInfoItem(wing_package::InfoItemKind::installDirName, new wing_package::StringItem(setup::InstallDirName()));
        wing_package::SetInfoItem(wing_package::InfoItemKind::defaultContainingDirPath, new wing_package::StringItem(setup::DefaultContainingDirPath()));
        wing_package::SetInfoItem(wing_package::InfoItemKind::compression, new wing_package::IntegerItem(static_cast<int64_t>(setup::Compression())));
        wing_package::SetInfoItem(wing_package::InfoItemKind::dataSource, new wing_package::IntegerItem(static_cast<int64_t>(wing_package::DataSource::memory)));
        wing_package::SetInfoItem(wing_package::InfoItemKind::packageDataAddress, new wing_package::IntegerItem(reinterpret_cast<int64_t>(packageResource.Data())));
        wing_package::SetInfoItem(wing_package::InfoItemKind::compressedPackageSize, new wing_package::IntegerItem(packageResource.Size()));
        wing_package::SetInfoItem(wing_package::InfoItemKind::uncompressedPackageSize, new wing_package::IntegerItem(setup::UncompressedPackageSize()));
        wing::Icon& setupIcon = wing::Application::GetResourceManager().GetIcon(setup::SetupIconResourceName());
        wing_package::Package package;
        wing_gui::InstallWindow installWindow;
        installWindow.SetPackage(&package);
        installWindow.SetIcon(setupIcon);
        installWindow.SetSmallIcon(setupIcon);
        wing::Application::Run(installWindow);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(nullptr, ex.what());
        return 1;
    }
    return 0;
}

