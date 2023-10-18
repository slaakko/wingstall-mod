// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module package_editor.action;

import package_editor.node;
import package_editor.rule;
import package_editor.content;
import package_editor.main_window;

namespace package_editor {

OpenAndExpandAction::OpenAndExpandAction(wing::MenuItem* menuItem_, Node* node_) : wing::ClickAction(menuItem_), node(node_)
{
}

void OpenAndExpandAction::Execute()
{
    MainWindow* mainWindow = node->GetMainWindow();
    if (mainWindow)
    {
        try
        {
            mainWindow->HideContextMenu();
            node->OpenAndExpand();
        }
        catch (const std::exception& ex)
        {
            wing::ShowErrorMessageBox(mainWindow->Handle(), ex.what());
        }
    }
}

OpenAction::OpenAction(wing::MenuItem* menuItem_, Node* node_) : wing::ClickAction(menuItem_), node(node_)
{
}

void OpenAction::Execute()
{
    MainWindow* mainWindow = node->GetMainWindow();
    if (mainWindow)
    {
        try
        {
            mainWindow->HideContextMenu();
            node->Open();
        }
        catch (const std::exception& ex)
        {
            wing::ShowErrorMessageBox(mainWindow->Handle(), ex.what());
        }
    }
}

AddAction::AddAction(wing::MenuItem* menuItem_, Node* node_, NodeKind kind_) : wing::ClickAction(menuItem_), node(node_), kind(kind_)
{
}

void AddAction::Execute()
{
    MainWindow* mainWindow = node->GetMainWindow();
    if (mainWindow)
    {
        try
        {
            mainWindow->HideContextMenu();
            node->AddNew(kind);
        }
        catch (const std::exception& ex)
        {
            wing::ShowErrorMessageBox(mainWindow->Handle(), ex.what());
        }
    }
}

EditAction::EditAction(wing::MenuItem* menuItem_, Node* node_) : wing::ClickAction(menuItem_), node(node_)
{
}

void EditAction::Execute()
{
    MainWindow* mainWindow = node->GetMainWindow();
    if (mainWindow)
    {
        try
        {
            mainWindow->HideContextMenu();
            node->Edit();
        }
        catch (const std::exception& ex)
        {
            wing::ShowErrorMessageBox(mainWindow->Handle(), ex.what());
        }
    }
}

RemoveAction::RemoveAction(wing::MenuItem* menuItem_, Node* node_) : wing::ClickAction(menuItem_), node(node_)
{
}

void RemoveAction::Execute()
{
    MainWindow* mainWindow = node->GetMainWindow();
    if (mainWindow)
    {
        try
        {
            mainWindow->HideContextMenu();
            node->Remove();
        }
        catch (const std::exception& ex)
        {
            wing::ShowErrorMessageBox(mainWindow->Handle(), ex.what());
        }
    }
}

MoveUpAction::MoveUpAction(wing::MenuItem* menuItem_, Node* node_) : wing::ClickAction(menuItem_), node(node_)
{
}

void MoveUpAction::Execute()
{
    MainWindow* mainWindow = node->GetMainWindow();
    if (mainWindow)
    {
        try
        {
            mainWindow->HideContextMenu();
            node->MoveUp();
        }
        catch (const std::exception& ex)
        {
            wing::ShowErrorMessageBox(mainWindow->Handle(), ex.what());
        }
    }
}

MoveDownAction::MoveDownAction(wing::MenuItem* menuItem_, Node* node_) : wing::ClickAction(menuItem_), node(node_)
{
}

void MoveDownAction::Execute()
{
    MainWindow* mainWindow = node->GetMainWindow();
    if (mainWindow)
    {
        try
        {
            mainWindow->HideContextMenu();
            node->MoveDown();
        }
        catch (const std::exception& ex)
        {
            wing::ShowErrorMessageBox(mainWindow->Handle(), ex.what());
        }
    }
}

AddVSRulesAction::AddVSRulesAction(wing::MenuItem* menuItem_, Node* node_) : wing::ClickAction(menuItem_), node(node_)
{
}

void AddVSRulesAction::Execute()
{
    MainWindow* mainWindow = node->GetMainWindow();
    if (mainWindow)
    {
        try
        {
            mainWindow->HideContextMenu();
            if (node && node->Kind() == NodeKind::rules)
            {
                Rules* rules = static_cast<Rules*>(node);
                rules->AddRule(MakeExcludeDebugDirRule());
                rules->AddRule(MakeExcludeReleaseDirRule());
                rules->AddRule(MakeExcludeX64DirRule());
                rules->AddRule(MakeExcludeVSDirRule());
                rules->OpenAndExpand();
            }
        }
        catch (const std::exception& ex)
        {
            wing::ShowErrorMessageBox(mainWindow->Handle(), ex.what());
        }
    }
}

ExcludeAction::ExcludeAction(wing::MenuItem* menuItem_, Node* node_) : wing::ClickAction(menuItem_), node(node_)
{
}

void ExcludeAction::Execute()
{
    MainWindow* mainWindow = node->GetMainWindow();
    if (mainWindow)
    {
        try
        {
            mainWindow->HideContextMenu();
            if (node)
            {
                if (node->Kind() == NodeKind::contentDirectory)
                {
                    ContentDirectory* contentDirectory = static_cast<ContentDirectory*>(node);
                    Node* ruleContainerNode = contentDirectory->Parent()->GetRuleContainerNode();
                    if (ruleContainerNode)
                    {
                        ruleContainerNode->AddRule(new Rule(contentDirectory->Name(), RuleKind::exclude, PathKind::dir));
                        Package* package = ruleContainerNode->GetPackage();
                        if (package)
                        {
                            package->SetDirty();
                        }
                        contentDirectory->Parent()->OpenAndExpand();
                    }
                }
                else if (node->Kind() == NodeKind::contentFile)
                {
                    ContentFile* contentFile = static_cast<ContentFile*>(node);
                    Node* ruleContainerNode = contentFile->Parent()->GetRuleContainerNode();
                    if (ruleContainerNode)
                    {
                        ruleContainerNode->AddRule(new Rule(contentFile->Name(), RuleKind::exclude, PathKind::file));
                        Package* package = ruleContainerNode->GetPackage();
                        if (package)
                        {
                            package->SetDirty();
                        }
                        contentFile->Parent()->OpenAndExpand();
                    }
                }
            }
        }
        catch (const std::exception& ex)
        {
            wing::ShowErrorMessageBox(mainWindow->Handle(), ex.what());
        }
    }
}

IncludeAction::IncludeAction(wing::MenuItem* menuItem_, Node* node_) : wing::ClickAction(menuItem_), node(node_)
{
}

void IncludeAction::Execute()
{
    MainWindow* mainWindow = node->GetMainWindow();
    if (mainWindow)
    {
        try
        {
            mainWindow->HideContextMenu();
            if (node)
            {
                if (node->Kind() == NodeKind::contentDirectory)
                {
                    ContentDirectory* contentDirectory = static_cast<ContentDirectory*>(node);
                    Node* ruleContainerNode = contentDirectory->Parent()->GetRuleContainerNode();
                    if (ruleContainerNode)
                    {
                        ruleContainerNode->AddRule(new Rule(contentDirectory->Name(), RuleKind::include, PathKind::dir));
                        Package* package = ruleContainerNode->GetPackage();
                        if (package)
                        {
                            package->SetDirty();
                        }
                        contentDirectory->Parent()->OpenAndExpand();
                    }
                }
                else if (node->Kind() == NodeKind::contentFile)
                {
                    ContentFile* contentFile = static_cast<ContentFile*>(node);
                    Node* ruleContainerNode = contentFile->Parent()->GetRuleContainerNode();
                    if (ruleContainerNode)
                    {
                        ruleContainerNode->AddRule(new Rule(contentFile->Name(), RuleKind::include, PathKind::file));
                        Package* package = ruleContainerNode->GetPackage();
                        if (package)
                        {
                            package->SetDirty();
                        }
                        contentFile->Parent()->OpenAndExpand();
                    }
                }
            }
        }
        catch (const std::exception& ex)
        {
            wing::ShowErrorMessageBox(mainWindow->Handle(), ex.what());
        }
    }
}

SaveAction::SaveAction(wing::MenuItem* menuItem_, Node* node_) : wing::ClickAction(menuItem_), node(node_)
{
}

void SaveAction::Execute()
{
    MainWindow* mainWindow = node->GetMainWindow();
    if (mainWindow)
    {
        try
        {
            mainWindow->HideContextMenu();
            if (node)
            {
                node->Save();
            }
        }
        catch (const std::exception& ex)
        {
            wing::ShowErrorMessageBox(mainWindow->Handle(), ex.what());
        }
    }
}

BuildAction::BuildAction(wing::MenuItem* menuItem_, Node* node_) : wing::ClickAction(menuItem_), node(node_)
{
}

void BuildAction::Execute()
{
    MainWindow* mainWindow = node->GetMainWindow();
    if (mainWindow)
    {
        try
        {
            mainWindow->HideContextMenu();
            if (node)
            {
                node->Build();
            }
        }
        catch (const std::exception& ex)
        {
            wing::ShowErrorMessageBox(mainWindow->Handle(), ex.what());
        }
    }
}

CloseAction::CloseAction(wing::MenuItem* menuItem_, Node* node_) : wing::ClickAction(menuItem_), node(node_)
{
}

void CloseAction::Execute()
{
    MainWindow* mainWindow = node->GetMainWindow();
    if (mainWindow)
    {
        try
        {
            mainWindow->HideContextMenu();
            if (node)
            {
                node->Close();
            }
        }
        catch (const std::exception& ex)
        {
            wing::ShowErrorMessageBox(mainWindow->Handle(), ex.what());
        }
    }
}

ClearAction::ClearAction(wing::MenuItem* menuItem_, Node* node_) : wing::ClickAction(menuItem_), node(node_)
{
}

void ClearAction::Execute()
{
    MainWindow* mainWindow = node->GetMainWindow();
    if (mainWindow)
    {
        try
        {
            mainWindow->HideContextMenu();
            if (node)
            {
                node->Clear();
            }
        }
        catch (const std::exception& ex)
        {
            wing::ShowErrorMessageBox(mainWindow->Handle(), ex.what());
        }
    }
}

} // package_editor
