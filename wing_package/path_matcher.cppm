// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing_package.path_matcher;

import dom;
import rex;
import std.core;

export namespace wing_package {

struct FileInfo
{
    FileInfo(const std::string& name_, uintmax_t size_, const std::chrono::time_point<std::chrono::file_clock>& time_);
    std::string name;
    uintmax_t size;
    std::chrono::time_point<std::chrono::file_clock> time;
};

struct DirectoryInfo
{
    DirectoryInfo(const std::string& name_, const std::chrono::time_point<std::chrono::file_clock>& time_, xml::Element* element_);
    std::string name;
    std::chrono::time_point<std::chrono::file_clock> time;
    xml::Element* element;
};

class PathMatcher;

enum class RuleKind : int
{
    none = 0, include = 1 << 0, exclude = 1 << 1, cascade = 1 << 2
};

inline RuleKind operator|(RuleKind left, RuleKind right)
{
    return RuleKind(int(left) | int(right));
}

inline RuleKind operator&(RuleKind left, RuleKind right)
{
    return RuleKind(int(left) & int(right));
}

inline RuleKind operator~(RuleKind operand)
{
    return RuleKind(~int(operand));
}

enum class PathKind
{
    none, dir, file
};

class PathRule
{
public:
    PathRule(PathMatcher& pathMatcher, std::string name_, RuleKind ruleKind_, PathKind pathKind_);
    PathRule(PathMatcher& pathMatcher, xml::Element* element_);
    PathKind GetPathKind() const { return pathKind; }
    RuleKind GetRuleKind() const { return ruleKind; }
    xml::Element* GetElement() const { return element; }
    const std::string& Name() const { return name; }
    bool Matches(const std::string& name);
private:
    xml::Element* element;
    std::string name;
    RuleKind ruleKind;
    PathKind pathKind;
    rex::Nfa nfa;
};

class PathRuleSet
{
public:
    PathRuleSet(PathRuleSet* parentRuleSet_);
    PathRuleSet* GetParentRuleSet() const { return parentRuleSet; }
    void AddRule(PathRule* rule);
    PathRule* GetRule(const std::string& name) const;
    bool IncludeDir(const std::string& dirName) const;
    bool IncludeFile(const std::string& fileName) const;
private:
    PathRuleSet* parentRuleSet;
    std::vector<std::unique_ptr<PathRule>> rules;
    std::map<std::string, PathRule*> ruleMap;
};

class PathMatcher
{
public:
    PathMatcher(const std::string& xmlFilePath_);
    const std::string& SourceRootDir() const { return sourceRootDir; }
    void SetSourceRootDir(const std::string& sourceRootDir_);
    const std::string& XmlFilePath() const { return xmlFilePath; }
    void BeginFiles(xml::Element* element);
    void EndFiles();
    void BeginDirectory(const std::string& name, xml::Element* element);
    void EndDirectory();
    const std::string& CurrentDir() const { return currentDir; }
    rex::Context& GetContext() { return context; }
    std::vector<DirectoryInfo> Directories() const;
    std::vector<FileInfo> Files() const;
private:
    rex::Context context;
    std::string xmlFilePath;
    std::string rootDir;
    std::string sourceRootDir;
    std::stack<std::string> dirStack;
    std::string currentDir;
    std::stack<std::unique_ptr<PathRuleSet>> ruleSetStack;
    std::unique_ptr<PathRuleSet> ruleSet;
};

} // namespace wing_package
