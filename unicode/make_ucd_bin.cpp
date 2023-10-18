import xml;
import util;
import std.core;
import std.filesystem;
import lexer;

using lexer::SourcePos;

class UnicodeCharacterDatabaseContentHandler : public xml::XmlContentHandler
{
public:
    UnicodeCharacterDatabaseContentHandler();
    void StartElement(const std::u32string& namespaceUri, const std::u32string& localName, const std::u32string& qualifiedName, const xml::Attributes& attributes, const SourcePos& sourcePos) override;
    void EndElement(const std::u32string& namespaceUri, const std::u32string& localName, const std::u32string& qualifiedName) override;
private:
    char32_t codePoint;
    util::ExtendedCharacterInfo* extendedCharacterInfo;
};

UnicodeCharacterDatabaseContentHandler::UnicodeCharacterDatabaseContentHandler() : codePoint(-1), extendedCharacterInfo(nullptr)
{
}

std::set<std::u32string> elementNames;
std::set<std::u32string> attributeNames;
std::set<std::u32string> booleanAttributeNames;
std::set<std::string> unseenPropertyNames;
std::vector<std::unique_ptr<util::Block>> blocks;
std::map<std::string, util::Block*>  blockMap;
std::ofstream iso88591;

uint32_t FromHex(const std::string& hex)
{
    uint32_t c = 0;
    std::stringstream s;
    s.str(hex);
    s >> std::hex >> c;
    return c;
}

void UnicodeCharacterDatabaseContentHandler::StartElement(const std::u32string& namespaceUri, const std::u32string& localName, const std::u32string& qualifiedName,
    const xml::Attributes& attributes, const SourcePos& sourcePos)
{
    auto it = elementNames.find(qualifiedName);
    if (it == elementNames.cend())
    {
        elementNames.insert(qualifiedName);
    }
    if (qualifiedName == U"char")
    {
        const std::u32string* cp = attributes.GetAttributeValue(U"cp");
        util::CharacterInfo* characterInfo = nullptr;
        extendedCharacterInfo = nullptr;
        uint32_t c = 0;
        if (cp)
        {
            std::string hex = util::ToUtf8(*cp);
            c = FromHex(hex);
            codePoint = c;
            characterInfo = &util::CreateCharacterInfo(c);
            extendedCharacterInfo = &util::CreateExtendedCharacterInfo(c);
        }
        for (const xml::Attribute& attribute : attributes)
        {
            auto it = attributeNames.find(attribute.QualifiedName());
            if (it == attributeNames.cend())
            {
                attributeNames.insert(attribute.QualifiedName());
            }
            if (cp && characterInfo && extendedCharacterInfo)
            {
                std::string propertyName = util::ToUtf8(attribute.QualifiedName());
                std::string attributeValue = util::ToUtf8(attribute.Value());
                if (util::BinaryPropertyTable::Instance().IsBinaryProperty(propertyName))
                {
                    const util::BinaryProperty& property = util::BinaryPropertyTable::Instance().GetBinaryPropertyByShortName(propertyName);
                    util::BinaryPropertyId id = property.Id();
                    bool value = false;
                    if (attributeValue == "Y")
                    {
                        value = true;
                    }
                    else if (attributeValue != "N")
                    {
                        throw std::runtime_error("binary property value not Y/N: line " + std::to_string(sourcePos.line) + ", column " + std::to_string(sourcePos.col));
                    }
                    characterInfo->SetBinaryPropery(id, value);
                }
                else if (propertyName == "age")
                {
                    const util::Age& age = util::AgeTable::Instance().GetAge(attributeValue);
                    characterInfo->SetAge(age.Id());
                }
                else if (propertyName == "gc")
                {
                    const util::GeneralCategory& generalCategory = util::GeneralCategoryTable::Instance().GetGeneralCategoryByShortName(attributeValue);
                    characterInfo->SetGeneralCategory(generalCategory.Id());
                }
                else if (propertyName == "blk")
                {
                    const util::Block& block = util::BlockTable::Instance().GetBlockByShortName(attributeValue);
                    characterInfo->SetBlock(block.Id());
                }
                else if (propertyName == "suc")
                {
                    if (attributeValue == "#")
                    {
                        characterInfo->SetUpper(c);
                    }
                    else
                    {
                        uint32_t upper = FromHex(attributeValue);
                        characterInfo->SetUpper(upper);
                    }
                }
                else if (propertyName == "slc")
                {
                    if (attributeValue == "#")
                    {
                        characterInfo->SetLower(c);
                    }
                    else
                    {
                        uint32_t lower = FromHex(attributeValue);
                        characterInfo->SetLower(lower);
                    }
                }
                else if (propertyName == "stc")
                {
                    if (attributeValue == "#")
                    {
                        characterInfo->SetTitle(c);
                    }
                    else
                    {
                        uint32_t title = FromHex(attributeValue);
                        characterInfo->SetTitle(title);
                    }
                }
                else if (propertyName == "scf")
                {
                    if (attributeValue == "#")
                    {
                        characterInfo->SetFolding(c);
                    }
                    else
                    {
                        uint32_t folding = FromHex(attributeValue);
                        characterInfo->SetFolding(folding);
                    }
                }
                else if (propertyName == "sc")
                {
                    util::ScriptId script = util::GetScriptByShortName(attributeValue).Id();
                    characterInfo->SetScript(script);
                }
                else if (propertyName == "bc")
                {
                    util::BidiClassId bidiClass = util::GetBidiClassByShortName(attributeValue).Id();
                    extendedCharacterInfo->SetBidiClass(bidiClass);
                }
                else if (propertyName == "uc")
                {
                    if (attributeValue == "#")
                    {
                        extendedCharacterInfo->FullUpper().append(1, c);
                    }
                    else
                    {
                        std::vector<std::string> v = util::Split(attributeValue, ' ');
                        for (const std::string& s : v)
                        {
                            uint32_t u = FromHex(s);
                            extendedCharacterInfo->FullUpper().append(1, u);
                        }
                    }
                }
                else if (propertyName == "lc")
                {
                    if (attributeValue == "#")
                    {
                        extendedCharacterInfo->FullLower().append(1, c);
                    }
                    else
                    {
                        std::vector<std::string> v = util::Split(attributeValue, ' ');
                        for (const std::string& s : v)
                        {
                            uint32_t u = FromHex(s);
                            extendedCharacterInfo->FullLower().append(1, u);
                        }
                    }
                }
                else if (propertyName == "tc")
                {
                    if (attributeValue == "#")
                    {
                        extendedCharacterInfo->FullTitle().append(1, c);
                    }
                    else
                    {
                        std::vector<std::string> v = util::Split(attributeValue, ' ');
                        for (const std::string& s : v)
                        {
                            uint32_t u = FromHex(s);
                            extendedCharacterInfo->FullTitle().append(1, u);
                        }
                    }
                }
                else if (propertyName == "cf")
                {
                    if (attributeValue == "#")
                    {
                        extendedCharacterInfo->FullFolding().append(1, c);
                    }
                    else
                    {
                        std::vector<std::string> v = util::Split(attributeValue, ' ');
                        for (const std::string& s : v)
                        {
                            uint32_t u = FromHex(s);
                            extendedCharacterInfo->FullFolding().append(1, u);
                        }
                    }
                }
                else if (propertyName == "ccc")
                {
                    int value = std::stoi(attributeValue);
                    if (value < 0 || value > 254)
                    {
                        throw std::runtime_error("invalid ccc attribute value");
                    }
                    extendedCharacterInfo->SetCanonicalCombiningClass(static_cast<uint8_t>(value));
                }
                else if (propertyName == "na")
                {
                    extendedCharacterInfo->SetCharacterName(attributeValue);
                }
                else if (propertyName == "na1")
                {
                    extendedCharacterInfo->SetUnicode1Name(attributeValue);
                }
                else if (propertyName == "nv")
                {
                    if (attributeValue != "NaN")
                    {
                        extendedCharacterInfo->SetNumericValue(attributeValue);
                    }
                }
                else if (propertyName == "nt")
                {
                    extendedCharacterInfo->SetNumericType(util::GetNumericTypeByShortName(attributeValue).Id());
                }
                else if (propertyName == "bmg")
                {
                    uint32_t bmg = FromHex(attributeValue);
                    extendedCharacterInfo->SetBidiMirroringGlyph(bmg);
                }
                else if (propertyName == "bpt")
                {
                    extendedCharacterInfo->SetBidiPairedBracketType(util::GetBidiPairedBracketTypeByShortName(attributeValue).Id());
                }
                else if (propertyName == "bpb")
                {
                    uint32_t bpb = FromHex(attributeValue);
                    extendedCharacterInfo->SetBidiPairedBracket(bpb);
                }
                else
                {
                    unseenPropertyNames.insert(propertyName);
                }
            }
        }
    }
    else if (qualifiedName == U"name-alias" && codePoint != -1 && extendedCharacterInfo != nullptr)
    {
        std::string alias;
        std::string type;
        for (const xml::Attribute& attribute : attributes)
        {
            std::string attributeName = util::ToUtf8(attribute.QualifiedName());
            if (attributeName == "alias")
            {
                alias = util::ToUtf8(attribute.Value());
            }
            else if (attributeName == "type")
            {
                type = util::ToUtf8(attribute.Value());
            }
        }
        const util::AliasType& aliasType = util::AliasTypeTable::Instance().GetAliasType(type);
        extendedCharacterInfo->Aliases().push_back(util::Alias(aliasType.Id(), alias));
    }
}

void UnicodeCharacterDatabaseContentHandler::EndElement(const std::u32string& namespaceUri, const std::u32string& localName, const std::u32string& qualifiedName)
{
    if (qualifiedName == U"char")
    {
        codePoint = -1;
        extendedCharacterInfo = nullptr;
    }
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
        std::string wingstall_root;
        const char* wingstall_root_env = getenv("WINGSTALL_ROOT");
        if (wingstall_root_env)
        {
            wingstall_root = wingstall_root_env;
        }
        if (wingstall_root.empty())
        {
            std::cerr << "please set 'WINGSTALL_ROOT' environment variable to contain /path/to/wingstall directory." << std::endl;
            return 2;
        }
        std::string xmlFileName = (std::filesystem::path(wingstall_root) / std::filesystem::path("unicode") / std::filesystem::path("ucd.all.flat.xml")).generic_string();
        std::cout << "processing " << xmlFileName << "...";
        UnicodeCharacterDatabaseContentHandler contentHandler;
        xml::ParseXmlFile(xmlFileName, &contentHandler);
        util::CharacterTable::Instance().Write();
        util::CharacterTable::Instance().WriteDeflate();
        std::cout << "\b\b\b, done." << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cerr << std::endl;
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
