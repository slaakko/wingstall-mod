// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module dom.parser;

import dom.document;
import util;
import xml;
import std.core;

export namespace xml {

std::unique_ptr<Document> ParseDocument(const std::u32string& content, const std::string& systemId);
std::unique_ptr<Document> ParseDocument(const std::u32string& content, const std::string& systemId, Flags flags);
std::unique_ptr<Document> ReadDocument(const std::string& fileName);
std::unique_ptr<Document> ReadDocument(const std::string& fileName, Flags flags);
void SendDocument(util::TcpSocket& socket, Document& document);
std::unique_ptr<Document> ReceiveDocument(util::TcpSocket& socket);

} // namespace xml
