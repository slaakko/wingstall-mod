// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module package_editor.error;

import dom;
import std.core;

export namespace package_editor {

class PackageXMLException : public std::runtime_error
{
public:
    PackageXMLException(const std::string& message_, const std::string& xmlFilePath_, xml::Element* element);
    const std::string& Message() const { return message; }
    const std::string& XMLFilePath() const { return xmlFilePath; }
    int Line() const { return line; }
    int Coumn() const { return column; }
private:
    std::string message;
    std::string xmlFilePath;
    int line;
    int column;
};

} // package_editor
