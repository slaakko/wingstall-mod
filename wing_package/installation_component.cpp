// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing_package.installation_component;

import wing_package.package;
import wing_package.info;
import wing_package.links;
import wing_package.environment;
import wing;

namespace wing_package {

InstallationComponent::InstallationComponent() : Component(NodeKind::installation_component, "installation")
{
}

void InstallationComponent::CreateInstallationInfo()
{
    Package* package = GetPackage();
    if (package)
    {
        std::string softwareKey = SoftwareKey(package->Id());
        try
        {
            wing::RegistryKey key = wing::OpenOrCreateRegistryKeyForSoftwareKey(softwareKey);
            key.SetValue("DisplayName", package->AppName(), wing::RegistryValueKind::regSz);
            key.SetValue("DisplayVersion", package->Version(), wing::RegistryValueKind::regSz);
            if (!package->Publisher().empty())
            {
                key.SetValue("Publisher", package->Publisher(), wing::RegistryValueKind::regSz);
            }
            if (!package->IconFilePath().empty())
            {
                key.SetValue("DisplayIcon", package->ExpandPath(package->IconFilePath()), wing::RegistryValueKind::regSz);
            }
            int majorVersion = package->MajorVersion();
            if (majorVersion != -1)
            {
                key.SetIntegerValue("VersionMajor", majorVersion);
            }
            int minorVersion = package->MinorVersion();
            if (minorVersion != -1)
            {
                key.SetIntegerValue("VersionMinor", minorVersion);
            }
            int binaryVersion = package->BinaryVersion();
            if (binaryVersion != -1)
            {
                key.SetIntegerValue("Version", binaryVersion);
            }
            std::string installLocation = package->TargetRootDir();
            if (!installLocation.empty())
            {
                key.SetValue("InstallLocation", installLocation, wing::RegistryValueKind::regSz);
            }
            std::string uninstallString = package->UninstallString();
            if (!uninstallString.empty())
            {
                key.SetValue("UninstallString", uninstallString, wing::RegistryValueKind::regSz);
            }
            InfoItem* item = GetInfoItem(InfoItemKind::uncompressedPackageSize);
            if (item && item->Type() == InfoItemType::integer)
            {
                IntegerItem* sizeItem = static_cast<IntegerItem*>(item);
                int64_t sz = sizeItem->Value();
                int sizeKB = static_cast<int>(sz / 1024);
                key.SetIntegerValue("EstimatedSize", sizeKB);
            }
        }
        catch (const std::exception& ex)
        {
            throw std::runtime_error("could not create registry installation information for software key '" + softwareKey + "': " + ex.what());
        }
    }
    else
    {
        throw std::runtime_error("package not set");
    }
}

void InstallationComponent::RemoveInstallationInfo()
{
    Package* package = GetPackage();
    if (package)
    {
        std::string softwareKey = SoftwareKey(package->Id());
        try
        {
            wing::DeleteRegistryKeyForSoftwareKey(softwareKey);
        }
        catch (const std::exception& ex)
        {
            package->LogError("could not remove software installation key '" + softwareKey + "' from registry: " + ex.what());
        }
    }
    else
    {
        throw std::runtime_error("package not set");
    }
}

std::string InstallationComponent::SoftwareKey(const util::uuid& packageId) const
{
    std::string key = "{" + util::ToUpperNarrow(util::ToString(packageId)) + "}";
    return key;
}

} // namespace wing_package
