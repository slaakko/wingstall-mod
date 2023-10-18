// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing_package.info;

namespace wing_package {

std::map<InfoItemKind, std::unique_ptr<InfoItem>> infoItemMap;

InfoItem::InfoItem(InfoItemType type_) : type(type_)
{
}

InfoItem::~InfoItem()
{
}

StringItem::StringItem(const std::string& value_) : InfoItem(InfoItemType::string), value(value_)
{
}

IntegerItem::IntegerItem(int64_t value_) : InfoItem(InfoItemType::integer), value(value_)
{
}

InfoItem* GetInfoItem(InfoItemKind infoItemKind)
{
    auto it = infoItemMap.find(infoItemKind);
    if (it != infoItemMap.cend())
    {
        return it->second.get();
    }
    return nullptr;
}

void SetInfoItem(InfoItemKind infoItemKind, InfoItem* item)
{
    infoItemMap[infoItemKind] = std::unique_ptr<InfoItem>(item);
}

} // namespace wing_package
