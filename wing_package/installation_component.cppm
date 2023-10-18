// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing_package.installation_component;

import wing_package.component;
import wing_package.directory;
import wing_package.file;
import util;

export namespace wing_package {

class InstallationComponent : public Component
{
public:
    InstallationComponent();
    void CreateInstallationInfo() override;
    void RemoveInstallationInfo() override;
private:
    std::string SoftwareKey(const util::uuid& packageId) const;
};

} // namespace wing_package
