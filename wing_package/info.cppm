// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing_package.info;

import std.core;

export namespace wing_package {

enum class InfoItemType
{
    string, integer
};

enum class InfoItemKind
{
    appName, appVersion, companyName, installDirName, defaultContainingDirPath, uncompressedPackageSize, compressedPackageSize, compression, dataSource, packageFilePath, packageDataAddress
};

class InfoItem
{
public:
    InfoItem(InfoItemType type_);
    InfoItemType Type() const { return type; }
    virtual ~InfoItem();
private:
    InfoItemType type;
};

class StringItem : public InfoItem
{
public:
    StringItem(const std::string& value_);
    const std::string& Value() const { return value; }
private:
    std::string value;
};

class IntegerItem : public InfoItem
{
public:
    IntegerItem(int64_t value_);
    int64_t Value() const { return value; }
private:
    int64_t value;
};

InfoItem* GetInfoItem(InfoItemKind infoItemKind);
void SetInfoItem(InfoItemKind infoItemKind, InfoItem* item);

} // namespace wing_package
