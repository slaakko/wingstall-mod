// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module dom.processing_instruction;

import dom.visitor;

namespace xml {

ProcessingInstruction::ProcessingInstruction(const lexer::SourcePos& sourcePos_) :
    Node(NodeKind::processingInstructionNode, sourcePos_, "processing_instruction")
{
}

ProcessingInstruction::ProcessingInstruction(const lexer::SourcePos& sourcePos_, const std::string& target_, const std::string& data_) :
    Node(NodeKind::processingInstructionNode, sourcePos_, "processing_instruction"), target(target_), data(data_)
{
}

void ProcessingInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ProcessingInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.WriteLine("<?" + target + " " + data + "?>");
}

ProcessingInstruction* MakeProcessingInstruction(const std::string& target, const std::string& data)
{
    return new ProcessingInstruction(lexer::SourcePos(), target, data);
}

} // namespace xml
