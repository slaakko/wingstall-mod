module xml.rules;

std::vector<const char*> rules = {
"XmlParser.Document"
, "XmlParser.Char"
, "XmlParser.S"
, "XmlParser.NameStartChar"
, "XmlParser.NameChar"
, "XmlParser.Name"
, "XmlParser.Names"
, "XmlParser.Nmtoken"
, "XmlParser.Nmtokens"
, "XmlParser.Prolog"
, "XmlParser.XMLDecl"
, "XmlParser.VersionInfo"
, "XmlParser.VersionNum"
, "XmlParser.VersionNumDQ"
, "XmlParser.VersionNumSQ"
, "XmlParser.VersionNumber"
, "XmlParser.EncodingDecl"
, "XmlParser.EncodingName"
, "XmlParser.EncNameDQ"
, "XmlParser.EncNameSQ"
, "XmlParser.EncName"
, "XmlParser.SDDecl"
, "XmlParser.DocTypeDecl"
, "XmlParser.ExternalID"
, "XmlParser.SystemLiteral"
, "XmlParser.PubidLiteral"
, "XmlParser.PubidChar"
, "XmlParser.IntSubset"
, "XmlParser.MarkupDecl"
, "XmlParser.DeclSep"
, "XmlParser.ElementDecl"
, "XmlParser.ContentSpec"
, "XmlParser.Children"
, "XmlParser.CP"
, "XmlParser.Choice"
, "XmlParser.Seq"
, "XmlParser.Mixed"
, "XmlParser.AttlistDecl"
, "XmlParser.AttDef"
, "XmlParser.AttType"
, "XmlParser.StringType"
, "XmlParser.TokenizedType"
, "XmlParser.EnumeratedType"
, "XmlParser.NotationType"
, "XmlParser.Enumeration"
, "XmlParser.DefaultDecl"
, "XmlParser.EntityDecl"
, "XmlParser.GEDecl"
, "XmlParser.PEDecl"
, "XmlParser.EntityDef"
, "XmlParser.PEDef"
, "XmlParser.EntityValue"
, "XmlParser.NDataDecl"
, "XmlParser.PEReference"
, "XmlParser.NotationDecl"
, "XmlParser.PublicID"
, "XmlParser.Element"
, "XmlParser.ETag"
, "XmlParser.Content"
, "XmlParser.CharDataChar"
, "XmlParser.CharData"
, "XmlParser.CDSect"
, "XmlParser.Attribute"
, "XmlParser.AttValueDQ"
, "XmlParser.AttValueSQ"
, "XmlParser.AttValue"
, "XmlParser.EntityRef"
, "XmlParser.DecCodePoint"
, "XmlParser.HexCodePoint"
, "XmlParser.CharRef"
, "XmlParser.Reference"
, "XmlParser.Misc"
, "XmlParser.Comment"
, "XmlParser.PI"
, "XmlParser.PITarget"
, "XmlParser.Xml"
, "XmlParser.Eq"
, "XmlParser.YesNo"
};

std::vector<const char*>* GetRuleNameVecPtr()
{
    return &rules;
}
