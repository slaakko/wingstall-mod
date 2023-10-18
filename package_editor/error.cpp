// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.error;

import lexer;

namespace package_editor {

std::string MakePackageXmlErrorMessage(const std::string& message, const std::string& filePath, const lexer::SourcePos& sourcePos)
{
    std::string errorMessage = message;
    errorMessage.append(" in file '").append(filePath).append("'").append(" line ").append(std::to_string(sourcePos.line)).append(", column ").append(std::to_string(sourcePos.col));
    return errorMessage;
}

PackageXMLException::PackageXMLException(const std::string& message_, const std::string& xmlFilePath_, xml::Element* element) :
    std::runtime_error(MakePackageXmlErrorMessage(message_, xmlFilePath_, element->GetSourcePos())),
    message(message_), xmlFilePath(xmlFilePath_), line(element->GetSourcePos().line), column(element->GetSourcePos().col)
{
}

} // ::package_editor
