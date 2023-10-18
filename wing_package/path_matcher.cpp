// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing_package.path_matcher;

import xpath;
import util;
import std.filesystem;

namespace wing_package {

FileInfo::FileInfo(const std::string& name_, uintmax_t size_, const std::chrono::time_point<std::chrono::file_clock>& time_) : name(name_), size(size_), time(time_)
{
}

DirectoryInfo::DirectoryInfo(const std::string& name_, const std::chrono::time_point<std::chrono::file_clock>& time_, xml::Element* element_) : 
    name(name_), time(time_), element(element_)
{
}

PathRule::PathRule(PathMatcher& pathMatcher, std::string name_, RuleKind ruleKind_, PathKind pathKind_) :
    element(nullptr), name(name_), ruleKind(ruleKind_), pathKind(pathKind_)
{
    nfa = rex::CompileFilePattern(pathMatcher.GetContext(), util::ToUtf32(name));
}

PathRule::PathRule(PathMatcher& pathMatcher, xml::Element* element_) :
    element(element_), ruleKind(RuleKind::none), pathKind(PathKind::none)
{
    if (element->Name() == "include" || element->Name() == "directory" || element->Name() == "file")
    {
        ruleKind = ruleKind | RuleKind::include;
        if (element->Name() == "include")
        {
            std::string cascadeAttr = element->GetAttribute("cascade");
            if (!cascadeAttr.empty())
            {
                if (cascadeAttr == "true")
                {
                    ruleKind = ruleKind | RuleKind::cascade;
                }
            }
        }
    }
    else if (element->Name() == "exclude")
    {
        ruleKind = ruleKind | RuleKind::exclude;
        std::string cascadeAttr = element->GetAttribute("cascade");
        if (!cascadeAttr.empty())
        {
            if (cascadeAttr == "true")
            {
                ruleKind = ruleKind | RuleKind::cascade;
            }
        }
    }
    else
    {
        throw std::runtime_error("unknown path matching rule '" + element->Name() + "' in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
    }
    if (element->Name() == "directory" || element->Name() == "file")
    {
        std::string nameAttr = element->GetAttribute("name");
        if (!nameAttr.empty())
        {
            if (element->Name() == "directory")
            {
                pathKind = PathKind::dir;
                if (nameAttr.find('/') != std::string::npos ||
                    (nameAttr.find('\\') != std::string::npos))
                {
                    throw std::runtime_error("the value of the 'name' attribute may not have '/' or '\\' characters in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
                        std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
                }
                nfa = rex::CompileFilePattern(pathMatcher.GetContext(), util::ToUtf32(nameAttr));
                name = nameAttr;
            }
            else if (element->Name() == "file")
            {
                pathKind = PathKind::file;
                if (nameAttr.find('/') != std::string::npos ||
                    (nameAttr.find('\\') != std::string::npos))
                {
                    throw std::runtime_error("the value of the 'name' attribute may not have '/' or '\\' characters in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
                        std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
                }
                nfa = rex::CompileFilePattern(pathMatcher.GetContext(), util::ToUtf32(nameAttr));
                name = nameAttr;
            }
        }
        else
        {
            throw std::runtime_error("path matching rule element 'directory' or 'file' must have 'name' attribute in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
                std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
        }
    }
    else
    {
        std::string dirAttr = element->GetAttribute("dir");
        if (!dirAttr.empty())
        {
            pathKind = PathKind::dir;
            if (dirAttr.find('/') != std::string::npos ||
                (dirAttr.find('\\') != std::string::npos))
            {
                throw std::runtime_error("the value of the 'dir' attribute may not have '/' or '\\' characters in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
                    std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
            }
            nfa = rex::CompileFilePattern(pathMatcher.GetContext(), util::ToUtf32(dirAttr));
            name = dirAttr;
        }
        std::string fileAttr = element->GetAttribute("file");
        if (!fileAttr.empty())
        {
            if (pathKind == PathKind::dir)
            {
                throw std::runtime_error("path matching rule element 'include' or 'exclude' cannot have both 'dir' and 'file' attribute in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
                    std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
            }
            pathKind = PathKind::file;
            if (fileAttr.find('/') != std::string::npos ||
                (fileAttr.find('\\') != std::string::npos))
            {
                throw std::runtime_error("the value of the 'file' attribute may not have '/' or '\\' characters in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
                    std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
            }
            nfa = rex::CompileFilePattern(pathMatcher.GetContext(), util::ToUtf32(fileAttr));
            name = fileAttr;
        }
        if (dirAttr.empty() && fileAttr.empty())
        {
            throw std::runtime_error("path matching rule element 'include' or 'exclude' must have either 'dir' or 'file' attribute in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
                std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
        }
    }
}

bool PathRule::Matches(const std::string& name)
{
    return rex::PatternMatch(util::ToUtf32(name), nfa);
}

PathRuleSet::PathRuleSet(PathRuleSet* parentRuleSet_) : parentRuleSet(parentRuleSet_)
{
}

void PathRuleSet::AddRule(PathRule* rule)
{
    rules.push_back(std::unique_ptr<PathRule>(rule));
    ruleMap[rule->Name()] = rule;
}

PathRule* PathRuleSet::GetRule(const std::string& name) const
{
    auto it = ruleMap.find(name);
    if (it != ruleMap.cend())
    {
        return it->second;
    }
    return nullptr;
}

bool PathRuleSet::IncludeDir(const std::string& dirName) const
{
    bool include = true;
    bool matched = false;
    for (const auto& rule : rules)
    {
        if (rule->GetPathKind() == PathKind::dir)
        {
            if ((rule->GetRuleKind() & RuleKind::include) != RuleKind::none && rule->Matches(dirName))
            {
                include = true;
                matched = true;
            }
            else if ((rule->GetRuleKind() & RuleKind::exclude) != RuleKind::none && rule->Matches(dirName))
            {
                include = false;
                matched = true;
            }
        }
    }
    PathRuleSet* parent = parentRuleSet;
    while (parent && !matched)
    {
        for (const auto& rule : parent->rules)
        {
            if (rule->GetPathKind() == PathKind::dir)
            {
                if ((rule->GetRuleKind() & RuleKind::cascade) != RuleKind::none)
                {
                    if ((rule->GetRuleKind() & RuleKind::include) != RuleKind::none && rule->Matches(dirName))
                    {
                        include = true;
                        matched = true;
                    }
                    else if ((rule->GetRuleKind() & RuleKind::exclude) != RuleKind::none && rule->Matches(dirName))
                    {
                        include = false;
                        matched = true;
                    }
                }
            }
        }
        parent = parent->parentRuleSet;
    }
    return include;
}

bool PathRuleSet::IncludeFile(const std::string& fileName) const
{
    bool include = true;
    bool matched = false;
    for (const auto& rule : rules)
    {
        if (rule->GetPathKind() == PathKind::file)
        {
            if ((rule->GetRuleKind() & RuleKind::include) != RuleKind::none && rule->Matches(fileName))
            {
                include = true;
                matched = true;
            }
            else if ((rule->GetRuleKind() & RuleKind::exclude) != RuleKind::none && rule->Matches(fileName))
            {
                include = false;
                matched = true;
            }
        }
    }
    PathRuleSet* parent = parentRuleSet;
    while (parent && !matched)
    {
        for (const auto& rule : parent->rules)
        {
            if (rule->GetPathKind() == PathKind::file)
            {
                if ((rule->GetRuleKind() & RuleKind::cascade) != RuleKind::none)
                {
                    if ((rule->GetRuleKind() & RuleKind::include) != RuleKind::none && rule->Matches(fileName))
                    {
                        include = true;
                        matched = true;
                    }
                    else if ((rule->GetRuleKind() & RuleKind::exclude) != RuleKind::none && rule->Matches(fileName))
                    {
                        include = false;
                        matched = true;
                    }
                }
            }
        }
        parent = parent->parentRuleSet;
    }
    return include;
}

PathMatcher::PathMatcher(const std::string& xmlFilePath_) : xmlFilePath(xmlFilePath_), rootDir(util::Path::GetDirectoryName(util::GetFullPath(xmlFilePath)))
{
}

void PathMatcher::SetSourceRootDir(const std::string& sourceRootDir_)
{
    sourceRootDir = util::GetFullPath(util::Path::Combine(rootDir, sourceRootDir_));
    currentDir = sourceRootDir;
}

void PathMatcher::BeginFiles(xml::Element* element)
{
    PathRuleSet* parentRuleSet = ruleSet.get();
    ruleSetStack.push(std::move(ruleSet));
    ruleSet.reset(new PathRuleSet(parentRuleSet));
    ruleSet->AddRule(new PathRule(*this, "*", RuleKind::exclude, PathKind::file));
    std::unique_ptr<xpath::XPathObject> ruleObject = xpath::Evaluate("file", element);
    if (ruleObject)
    {
        if (ruleObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(ruleObject.get());
            int n = nodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* node = (*nodeSet)[i];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* childElement = static_cast<xml::Element*>(node);
                    PathRule* rule = new PathRule(*this, childElement);
                    ruleSet->AddRule(rule);
                }
            }
        }
    }
}

void PathMatcher::EndFiles()
{
    ruleSet = std::move(ruleSetStack.top());
    ruleSetStack.pop();
}

void PathMatcher::BeginDirectory(const std::string& name, xml::Element* element)
{
    PathRuleSet* parentRuleSet = ruleSet.get();
    ruleSetStack.push(std::move(ruleSet));
    ruleSet.reset(new PathRuleSet(parentRuleSet));
    dirStack.push(currentDir);
    currentDir = util::Path::Combine(currentDir, name);
    if (element)
    {
        std::unique_ptr<xpath::XPathObject> ruleObject = xpath::Evaluate("*", element);
        if (ruleObject)
        {
            if (ruleObject->Type() == xpath::XPathObjectType::nodeSet)
            {
                xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(ruleObject.get());
                int n = nodeSet->Length();
                for (int i = 0; i < n; ++i)
                {
                    xml::Node* node = (*nodeSet)[i];
                    if (node->Kind() == xml::NodeKind::elementNode)
                    {
                        xml::Element* childElement = static_cast<xml::Element*>(node);
                        PathRule* rule = new PathRule(*this, childElement);
                        ruleSet->AddRule(rule);
                    }
                }
            }
        }
    }
}

void PathMatcher::EndDirectory()
{
    ruleSet = std::move(ruleSetStack.top());
    ruleSetStack.pop();
    currentDir = std::move(dirStack.top());
    dirStack.pop();
}

std::vector<DirectoryInfo> PathMatcher::Directories() const
{
    std::vector<DirectoryInfo> directories;
    std::error_code ec;
    std::filesystem::directory_iterator it(currentDir, ec);
    if (ec)
    {
        throw std::runtime_error("could not iterate directory '" + currentDir + "': " + util::PlatformStringToUtf8(ec.message()));
    }
    while (it != std::filesystem::directory_iterator())
    {
        if (std::filesystem::is_directory(it->status()))
        {
            if (it->path().filename() != "." && it->path().filename() != "..")
            {
                std::string name = it->path().filename().generic_string();
                if (ruleSet->IncludeDir(name))
                {
                    xml::Element* element = nullptr;
                    PathRule* rule = ruleSet->GetRule(name);
                    if (rule)
                    {
                        element = rule->GetElement();
                    }
                    std::chrono::time_point<std::chrono::file_clock> time = std::filesystem::last_write_time(it->path());
                    DirectoryInfo directoryInfo(name, time, element);
                    directories.push_back(directoryInfo);
                }
            }
        }
        ++it;
    }
    return directories;
}

std::vector<FileInfo> PathMatcher::Files() const
{
    std::vector<FileInfo> files;
    std::error_code ec;
    std::filesystem::directory_iterator it(currentDir, ec);
    if (ec)
    {
        throw std::runtime_error("could not iterate directory '" + currentDir + "': " + util::PlatformStringToUtf8(ec.message()));
    }
    while (it != std::filesystem::directory_iterator())
    {
        if (std::filesystem::is_regular_file(it->status()))
        {
            std::string name = it->path().filename().generic_string();
            if (ruleSet->IncludeFile(name))
            {
                FileInfo fileInfo(name, std::filesystem::file_size(it->path()), std::filesystem::last_write_time(it->path()));
                files.push_back(fileInfo);
            }
        }
        ++it;
    }
    return files;
}

} // namespace wing_package
