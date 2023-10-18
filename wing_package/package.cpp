// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing_package.package;

import wing_package.component;
import wing_package.info;
import wing_package.preinstall_component;
import wing_package.uninstall_component;
import wing_package.installation_component;
import wing_package.path_matcher;
import wing_package.environment;
import wing_package.links;
import xpath;

namespace wing_package {

PackageObserver::~PackageObserver()
{
}

PackageStreamObserver::PackageStreamObserver(Package* package_) : package(package_)
{
}

void PackageStreamObserver::PositionChanged(util::Stream* stream)
{
    package->NotifyStreamPositionChanged();
}

AbortException::AbortException() : std::runtime_error("abort")
{
}

Package::Package() :
    Node(NodeKind::package), id(util::nil_uuid()), compression(Compression::none), component(), file(), stream(),
    streamObserver(this), size(0), interrupted(false), action(Action::continueAction), status(Status::idle), includeUninstaller(false),
    fileCount(0), fileIndex(0), includeFileContent(false), fileContentSize(0), fileContentPos(0), uncompressedSize(0), streamStartPosition(0)
{
    variables.SetParent(this);
    SetInstallationComponent(new InstallationComponent());
}

Package::Package(const std::string& name_) :
    Node(NodeKind::package, name_), id(util::nil_uuid()), compression(Compression::none), component(), file(), stream(),
    streamObserver(this), size(0), interrupted(false), action(Action::continueAction), status(Status::idle), includeUninstaller(false),
    fileCount(0), fileIndex(0), includeFileContent(false), fileContentSize(0), fileContentPos(0), uncompressedSize(0), streamStartPosition(0)
{
    variables.SetParent(this);
    SetInstallationComponent(new InstallationComponent());
}

Package::Package(PathMatcher& pathMatcher, xml::Document* doc) :
    Node(NodeKind::package), id(util::nil_uuid()), compression(Compression::none), component(), file(), stream(),
    streamObserver(this), size(0), interrupted(false), action(Action::continueAction), status(Status::idle), includeUninstaller(false),
    fileCount(0), fileIndex(0), includeFileContent(false), fileContentSize(0), fileContentPos(0), uncompressedSize(0), streamStartPosition(0)
{
    variables.SetParent(this);
    std::unique_ptr<xpath::XPathObject> packageObject = xpath::Evaluate("/package", doc);
    if (packageObject)
    {
        if (packageObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(packageObject.get());
            int n = nodeSet->Length();
            if (n == 1)
            {
                xml::Node* node = (*nodeSet)[0];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    std::string nameAttr = element->GetAttribute("name");
                    if (!nameAttr.empty())
                    {
                        SetName(nameAttr);
                    }
                    else
                    {
                        throw std::runtime_error("package element has no 'name' attribute in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
                            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
                    }
                    std::string sourceRootDirAttr = element->GetAttribute("sourceRootDir");
                    if (!sourceRootDirAttr.empty())
                    {
                        pathMatcher.SetSourceRootDir(sourceRootDirAttr);
                        SetSourceRootDir(pathMatcher.SourceRootDir());
                    }
                    else
                    {
                        throw std::runtime_error("package element has no 'sourceRootDir' attribute in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
                            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
                    }
                    std::string targetRootDirAttr = element->GetAttribute("targetRootDir");
                    if (!targetRootDirAttr.empty())
                    {
                        SetTargetRootDir(targetRootDirAttr);
                    }
                    else
                    {
                        throw std::runtime_error("package element has no 'targetRootDir' attribute in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
                            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
                    }
                    std::string appNameAttr = element->GetAttribute("appName");
                    if (!appNameAttr.empty())
                    {
                        SetAppName(appNameAttr);
                    }
                    else
                    {
                        throw std::runtime_error("package element has no 'appname' attribute in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
                            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
                    }
                    std::string publisherAttr = element->GetAttribute("publisher");
                    if (!publisherAttr.empty())
                    {
                        SetPublisher(publisherAttr);
                    }
                    std::string iconFilePathAttr = element->GetAttribute("iconFilePath");
                    if (!iconFilePathAttr.empty())
                    {
                        SetIconFilePath(iconFilePathAttr);
                    }
                    std::string compressionAttr = element->GetAttribute("compression");
                    if (!compressionAttr.empty())
                    {
                        SetCompression(ParseCompressionStr(compressionAttr));
                    }
                    std::string versionAttr = element->GetAttribute("version");
                    if (!versionAttr.empty())
                    {
                        SetVersion(versionAttr);
                    }
                    else
                    {
                        SetVersion("1.0.0");
                    }
                    std::string includeUninstallerAttr = element->GetAttribute("includeUninstaller");
                    if (!includeUninstallerAttr.empty())
                    {
                        try
                        {
                            includeUninstaller = util::ParseBool(includeUninstallerAttr);
                        }
                        catch (const std::exception& ex)
                        {
                            throw std::runtime_error("could not parse 'includeUninstaller' attribute: " + std::string(ex.what()));
                        }
                    }
                    else
                    {
                        includeUninstaller = true;
                    }
                    std::string idAttr = element->GetAttribute("id");
                    if (!idAttr.empty())
                    {
                        SetId(util::ParseUuid(idAttr));
                    }
                    else
                    {
                        SetId(util::random_uuid());
                    }
                }
                else
                {
                    throw std::runtime_error("one package element node expected in package XML document '" + pathMatcher.XmlFilePath() + "'");
                }
            }
            else
            {
                throw std::runtime_error("one package node expected in package XML document '" + pathMatcher.XmlFilePath() + "'");
            }
        }
    }
    std::unique_ptr<xpath::XPathObject> preinstallObject = xpath::Evaluate("/package/preinstall", doc);
    if (preinstallObject)
    {
        if (preinstallObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(preinstallObject.get());
            int n = nodeSet->Length();
            if (n == 1)
            {
                xml::Node* node = (*nodeSet)[0];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    SetPreinstallComponent(new PreinstallComponent(pathMatcher, element));
                }
            }
            else if (n > 1)
            {
                throw std::runtime_error("at most one 'preinstall' element allowed in package XML document '" + pathMatcher.XmlFilePath() + "'");
            }
        }
    }
    std::unique_ptr<xpath::XPathObject> componentObject = xpath::Evaluate("/package/component", doc);
    if (componentObject)
    {
        if (componentObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(componentObject.get());
            int n = nodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* node = (*nodeSet)[i];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    Component* component = new Component(pathMatcher, element);
                    AddComponent(component);
                }
            }
        }
    }
    if (includeUninstaller)
    {
        UninstallComponent* uninstallComponent = new UninstallComponent();
        AddComponent(uninstallComponent);
        uninstallComponent->Initialize();
    }
    std::unique_ptr<xpath::XPathObject> environmentObject = xpath::Evaluate("/package/environment", doc);
    if (environmentObject)
    {
        if (environmentObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(environmentObject.get());
            int n = nodeSet->Length();
            if (n == 1)
            {
                xml::Node* node = (*nodeSet)[0];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    SetEnvironment(new Environment(pathMatcher, element));
                }
            }
            else if (n > 1)
            {
                throw std::runtime_error("at most one environment node expected in package XML document '" + pathMatcher.XmlFilePath() + "'");
            }
        }
    }
    std::unique_ptr<xpath::XPathObject> linksObject = xpath::Evaluate("/package/links", doc);
    if (linksObject)
    {
        if (linksObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(linksObject.get());
            int n = nodeSet->Length();
            if (n == 1)
            {
                xml::Node* node = (*nodeSet)[0];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    SetLinks(new Links(pathMatcher, element));
                }
            }
            else if (n > 1)
            {
                throw std::runtime_error("at most one 'links' node expected in package XML document '" + pathMatcher.XmlFilePath() + "'");
            }
        }
    }
    std::unique_ptr<xpath::XPathObject> uninstallObject = xpath::Evaluate("package/uninstall/run", doc);
    if (uninstallObject)
    {
        if (uninstallObject->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(uninstallObject.get());
            int n = nodeSet->Length();
            for (int i = 0; i < n; ++i)
            {
                xml::Node* node = (*nodeSet)[i];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    std::string commandAttr = element->GetAttribute("command");
                    if (!commandAttr.empty())
                    {
                        uninstallCommands.push_back(commandAttr);
                    }
                    else
                    {
                        throw std::runtime_error("'uninstall/run' element does not have 'command' attribute in package XML document '" + pathMatcher.XmlFilePath() + "' line " +
                            std::to_string(element->GetSourcePos().line) + ", column " + std::to_string(element->GetSourcePos().col));
                    }
                }
            }
        }
    }
    SetInstallationComponent(new InstallationComponent());
}

void Package::SetStatus(Status status_, const std::string& statusStr_, const std::string& errorMessage_)
{
    if (status != status_ || statusStr != statusStr_ || errorMessage != errorMessage_)
    {
        status = status_;
        statusStr = statusStr_;
        errorMessage = errorMessage_;
        NotifyStatusChanged();
    }
}

void Package::NotifyStatusChanged()
{
    for (PackageObserver* observer : observers)
    {
        observer->StatusChanged(this);
    }
}

void Package::SetComponent(Node* component_)
{
    if (component != component_)
    {
        component = component_;
        NotifyComponentChanged();
    }
}

void Package::NotifyComponentChanged()
{
    for (PackageObserver* observer : observers)
    {
        observer->ComponentChanged(this);
    }
}

void Package::SetFile(File* file_)
{
    if (file != file_)
    {
        file = file_;
        NotifyFileChanged();
    }
}

void Package::NotifyFileChanged()
{
    for (PackageObserver* observer : observers)
    {
        observer->FileChanged(this);
    }
}

void Package::NotifyFileIndexChanged()
{
    for (PackageObserver* observer : observers)
    {
        observer->FileIndexChanged(this);
    }
}

void Package::NotifyFileContentPositionChanged()
{
    for (PackageObserver* observer : observers)
    {
        observer->FileContentPositionChanged(this);
    }
}

void Package::NotifyStreamPositionChanged()
{
    int64_t pos = stream->Position() - streamStartPosition;
    if (pos == uncompressedSize)
    {
        int x = 0;
    }
    else if (pos > uncompressedSize)
    {
        int x = 0;
    }
    for (PackageObserver* observer : observers)
    {
        observer->StreamPositionChanged(this);
    }
}

int64_t Package::GetStreamPosition() const
{
    int64_t pos = stream->Position() - streamStartPosition;
    return pos;
}

void Package::AddObserver(PackageObserver* observer)
{
    if (std::find(observers.begin(), observers.end(), observer) == observers.end())
    {
        observers.push_back(observer);
    }
}

void Package::RemoveObserver(PackageObserver* observer)
{
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

const std::string& Package::GetSourceRootDir() const
{
    if (!SourceRootDir().empty())
    {
        return SourceRootDir();
    }
    else
    {
        throw std::runtime_error("source root directory not set");
    }
}

const std::string& Package::GetTargetRootDir() const
{
    if (!TargetRootDir().empty())
    {
        return TargetRootDir();
    }
    else
    {
        throw std::runtime_error("target root directory not set");
    }
}

void Package::SetVersion(const std::string& version_)
{
    version = version_;
}

int Package::MajorVersion() const
{
    if (!Version().empty())
    {
        std::vector<std::string> v = util::Split(Version(), '.');
        if (v.size() > 0)
        {
            return std::stoi(v[0]);
        }
    }
    return -1;
}

int Package::MinorVersion() const
{
    if (!Version().empty())
    {
        std::vector<std::string> v = util::Split(Version(), '.');
        if (v.size() > 1)
        {
            return std::stoi(v[1]);
        }
    }
    return -1;
}

int Package::Build() const
{
    if (!Version().empty())
    {
        std::vector<std::string> v = util::Split(Version(), '.');
        if (v.size() > 2)
        {
            return std::stoi(v[2]);
        }
    }
    return -1;
}

int Package::BinaryVersion() const
{
    if (!Version().empty())
    {
        std::vector<std::string> v = util::Split(Version(), '.');
        int major = MajorVersion();
        if (major != -1)
        {
            int minor = MinorVersion();
            if (minor == -1)
            {
                minor = 0;
            }
            int build = Build();
            if (build == -1)
            {
                build = 0;
            }
            return
                static_cast<int>(static_cast<uint8_t>(major)) << static_cast<int>(3 * 8) |
                static_cast<int>(static_cast<uint8_t>(minor)) << static_cast<int>(2 * 8) |
                static_cast<int>(static_cast<uint16_t>(build));
        }
    }
    return -1;
}

void Package::SetId(const util::uuid& id_)
{
    id = id_;
}

std::string Package::UninstallString() const
{
    if (includeUninstaller)
    {
        return util::MakeNativePath(util::Path::Combine(targetRootDir, "uninstall.exe"));
    }
    return std::string();
}

void Package::SetPreinstallComponent(Component* preinstallComponent_)
{
    preinstallComponent.reset(preinstallComponent_);
    preinstallComponent->SetParent(this);
}

Component* Package::GetPreinstallComponent() const
{
    return preinstallComponent.get();
}

void Package::SetInstallationComponent(Component* installationComponent_)
{
    installationComponent.reset(installationComponent_);
    installationComponent->SetParent(this);
}

Component* Package::GetInstallationComponent() const
{
    return installationComponent.get();
}

void Package::AddComponent(Component* component)
{
    component->SetParent(this);
    components.push_back(std::unique_ptr<Component>(component));
}

void Package::SetEnvironment(Environment* environment_)
{
    environment.reset(environment_);
    environment->SetParent(this);
}

void Package::SetLinks(Links* links_)
{
    links.reset(links_);
    links->SetParent(this);
}

void Package::WriteIndex(const std::string& filePath)
{
    util::Streams streams;
    streams.Add(new util::FileStream(filePath, util::OpenMode::write | util::OpenMode::binary));
    streams.Add(new util::BufferedStream(streams.Back()));
    util::BinaryStreamWriter writer(streams.Back());
    WriteIndex(writer);
}

void Package::ReadIndex(const std::string& filePath)
{
    util::Streams streams;
    streams.Add(new util::FileStream(filePath, util::OpenMode::read | util::OpenMode::binary));
    streams.Add(new util::BufferedStream(streams.Back()));
    util::BinaryStreamReader reader(streams.Back());
    ReadIndex(reader);
}

void Package::WriteIndex(util::BinaryStreamWriter& writer)
{
    CheckInterrupted();
    Node::WriteIndex(writer);
    writer.Write(sourceRootDir);
    writer.Write(targetRootDir);
    writer.Write(uint8_t(compression));
    writer.Write(version);
    writer.Write(appName);
    writer.Write(publisher);
    writer.Write(iconFilePath);
    writer.Write(id);
    writer.Write(includeUninstaller);
    int32_t numComponents = components.size();
    writer.Write(numComponents);
    for (int32_t i = 0; i < numComponents; ++i)
    {
        Component* component = components[i].get();
        wing_package::WriteIndex(component, writer);
    }
    bool hasEnvironment = environment != nullptr;
    writer.Write(hasEnvironment);
    if (hasEnvironment)
    {
        environment->WriteIndex(writer);
    }
    bool hasLinks = links != nullptr;
    writer.Write(hasLinks);
    if (hasLinks)
    {
        links->WriteIndex(writer);
    }
    int32_t numUninstallCommands = uninstallCommands.size();
    writer.Write(numUninstallCommands);
    for (int32_t i = 0; i < numUninstallCommands; ++i)
    {
        writer.Write(uninstallCommands[i]);
    }
}

void Package::ReadIndex(util::BinaryStreamReader& reader)
{
    Node::ReadIndex(reader);
    SetComponent(this);
    CheckInterrupted();
    sourceRootDir = reader.ReadUtf8String();
    std::string packageTargetRootDir = reader.ReadUtf8String();
    if (targetRootDir.empty())
    {
        targetRootDir = packageTargetRootDir;
    }
    compression = Compression(reader.ReadByte());
    version = reader.ReadUtf8String();
    appName = reader.ReadUtf8String();
    publisher = reader.ReadUtf8String();
    iconFilePath = reader.ReadUtf8String();
    reader.ReadUuid(id);
    includeUninstaller = reader.ReadBool();
    int32_t numComponents = reader.ReadInt();
    for (int32_t i = 0; i < numComponents; ++i)
    {
        Component* component = BeginReadComponent(reader);
        AddComponent(component);
        component->ReadIndex(reader);
    }
    bool hasEnvironment = reader.ReadBool();
    if (hasEnvironment)
    {
        SetEnvironment(new Environment());
        environment->ReadIndex(reader);
    }
    bool hasLinks = reader.ReadBool();
    if (hasLinks)
    {
        SetLinks(new Links());
        links->ReadIndex(reader);
    }
    int32_t numUninstallCommands = reader.ReadInt();
    for (int32_t i = 0; i < numUninstallCommands; ++i)
    {
        std::string uninstallCommand = reader.ReadUtf8String();
        uninstallCommands.push_back(uninstallCommand);
    }
}

void Package::WriteData(util::BinaryStreamWriter& writer)
{
    Package* package = GetPackage();
    if (package)
    {
        package->CheckInterrupted();
    }
    for (const auto& component : components)
    {
        component->WriteData(writer);
    }
}

void Package::ReadData(util::BinaryStreamReader& reader)
{
    SetComponent(this);
    CheckInterrupted();
    for (const auto& component : components)
    {
        component->ReadData(reader);
    }
}

xml::Element* Package::ToXml() const
{
    xml::Element* element = xml::MakeElement("packageIndex");
    element->SetAttribute("name", Name());
    element->SetAttribute("appName", appName);
    element->SetAttribute("publisher", publisher);
    element->SetAttribute("iconFilePath", iconFilePath);
    element->SetAttribute("sourceRootDir", sourceRootDir);
    element->SetAttribute("targetRootDir", targetRootDir);
    element->SetAttribute("compression", CompressionStr(compression));
    element->SetAttribute("version", version);
    element->SetAttribute("id", util::ToString(id));
    element->SetAttribute("includeUninstaller", util::ToString(includeUninstaller));
    for (const auto& component : components)
    {
        xml::Element* child = component->ToXml();
        element->AppendChild(child);
    }
    if (environment != nullptr)
    {
        xml::Element* child = environment->ToXml();
        element->AppendChild(child);
    }
    if (links != nullptr)
    {
        xml::Element* child = links->ToXml();
        element->AppendChild(child);
    }
    xml::Element* variablesElement = variables.ToXml();
    element->AppendChild(variablesElement);
    return element;
}

std::unique_ptr<xml::Document> Package::XmlDoc() const
{
    std::unique_ptr<xml::Document> doc(new xml::Document());
    doc->AppendChild(ToXml());
    return doc;
}

void Package::WriteIndexToXmlFile(const std::string& xmlFilePath)
{
    std::ofstream file(xmlFilePath);
    util::CodeFormatter formatter(file);
    formatter.SetIndentSize(1);
    std::unique_ptr<xml::Document> doc = XmlDoc();
    doc->Write(formatter);
}

std::unique_ptr<xml::Document> Package::InfoDoc() const
{
    xml::Element* rootElement = xml::MakeElement("packageInfo");
    rootElement->SetAttribute("appName", appName);
    rootElement->SetAttribute("appVersion", version);
    rootElement->SetAttribute("publisher", publisher);
    rootElement->SetAttribute("iconFilePath", iconFilePath);
    rootElement->SetAttribute("compression", CompressionStr(compression));
    std::string installDirName = util::Path::GetFileName(targetRootDir);
    rootElement->SetAttribute("installDirName", installDirName);
    std::string defaultContainingDirPath = util::Path::GetDirectoryName(util::GetFullPath(targetRootDir));
    rootElement->SetAttribute("defaultContainingDirPath", defaultContainingDirPath);
    rootElement->SetAttribute("uncompressedPackageSize", std::to_string(size));
    rootElement->SetAttribute("includeUninstaller", util::ToString(includeUninstaller));
    rootElement->SetAttribute("id", util::ToString(id));
    std::unique_ptr<xml::Document> doc(new xml::Document());
    doc->AppendChild(rootElement);
    return doc;
}

void Package::WriteInfoXmlFile(const std::string& xmlFilePath)
{
    std::ofstream file(xmlFilePath);
    util::CodeFormatter formatter(file);
    formatter.SetIndentSize(1);
    std::unique_ptr<xml::Document> doc = InfoDoc();
    doc->Write(formatter);
}

void Package::Create(const std::string& filePath, Content content)
{
    CheckInterrupted();
    if (content != Content::none)
    {
        util::Streams streams = GetWriteStreams(filePath);
        if (streams.Count() > 0)
        {
            includeFileContent = false;
            util::Stream* uncompressedStream = streams.Get(0);
            fileContentSize = 0;
            fileContentPos = 0;
            util::BinaryStreamWriter uncompressedStreamWriter(*uncompressedStream);
            uncompressedStreamWriter.Write(std::uint8_t(compression));
            uncompressedStreamWriter.Write(targetRootDir);
            if ((content & Content::preinstall) != Content::none)
            {
                bool hasPreinstallComponent = preinstallComponent != nullptr;
                uncompressedStreamWriter.Write(hasPreinstallComponent);
                if (hasPreinstallComponent)
                {
                    preinstallComponent->Write(streams);
                }
            }
            includeFileContent = true;
            util::BinaryStreamWriter writer(streams.Back());
            streamStartPosition = writer.Position();
            if ((content & Content::index) != Content::none)
            {
                WriteIndex(writer);
            }
            if ((content & Content::data) != Content::none)
            {
                WriteData(writer);
            }
            size = writer.Position() - streamStartPosition;
            SetComponent(nullptr);
            SetFile(nullptr);
            SetStatus(Status::succeeded, "writing succeeded", std::string());
        }
    }
}

std::string Package::ExpandPath(const std::string& path) const
{
    return variables.ExpandPath(path);
}

void Package::Install(DataSource dataSource, const std::string& filePath, uint8_t* data, int64_t size, Content content)
{
    InfoItem* uncompressedSizeItem = GetInfoItem(InfoItemKind::uncompressedPackageSize);
    if (uncompressedSizeItem && uncompressedSizeItem->Type() == InfoItemType::integer)
    {
        uncompressedSize = static_cast<IntegerItem*>(uncompressedSizeItem)->Value();
    }
    ResetAction();
    try
    {
        if (content != Content::none)
        {
            util::Streams streams = GetReadBaseStream(dataSource, filePath, data, size);
            if (streams.Count() > 0)
            {
                includeFileContent = false;
                util::Stream* uncompressedStream = streams.Get(0);
                util::BinaryStreamReader uncompressedStreamReader(*uncompressedStream);
                Compression packageCompression = static_cast<Compression>(uncompressedStreamReader.ReadByte());
                std::string packageTargetRootDir = uncompressedStreamReader.ReadUtf8String();
                if (targetRootDir.empty())
                {
                    SetTargetRootDir(packageTargetRootDir);
                }
                AddReadCompressionStreams(streams, packageCompression);
                if ((content & Content::preinstall) != Content::none)
                {
                    bool hasPreinstallComponent = uncompressedStreamReader.ReadBool();
                    if (hasPreinstallComponent)
                    {
                        SetPreinstallDir(util::GetFullPath(util::Path::Combine(GetTargetRootDir(), util::ToString(util::random_uuid()))));
                        SetStatus(Status::running, "checking prerequisites...", std::string());
                        SetPreinstallComponent(new PreinstallComponent());
                        preinstallComponent->Read(streams);
                        preinstallComponent->RunCommands();
                    }
                }
                fileContentSize = 0;
                fileContentPos = 0;
                includeFileContent = true;
                stream = &streams.Back();
                stream->AddObserver(&streamObserver);
                util::BinaryStreamReader reader(*stream);
                streamStartPosition = reader.Position();
                if ((content & Content::index) != Content::none)
                {
                    SetStatus(Status::running, "reading package index...", std::string());
                    ReadIndex(reader);
                }
                if ((content & Content::data) != Content::none)
                {
                    SetStatus(Status::running, "copying files...", std::string());
                    ReadData(reader);
                }
                if (environment)
                {
                    SetStatus(Status::running, "creating environment variables...", std::string());
                    environment->Install();
                }
                if (links)
                {
                    SetStatus(Status::running, "creating links...", std::string());
                    links->Install();
                }
                if (includeUninstaller)
                {
                    SetStatus(Status::running, "writing uninstall information file...", std::string());
                    std::string uninstallBinFilePath = util::GetFullPath(util::Path::Combine(GetTargetRootDir(), "uninstall.bin"));
                    WriteIndex(uninstallBinFilePath);
                }
                if (installationComponent != nullptr)
                {
                    SetStatus(Status::running, "creating installation registry information...", std::string());
                    installationComponent->CreateInstallationInfo();
                }
                SetComponent(nullptr);
                SetFile(nullptr);
                SetStatus(Status::succeeded, "installation succeeded", std::string());
                stream->RemoveObserver(&streamObserver);
            }
        }
    }
    catch (const AbortException&)
    {
        SetStatus(Status::aborted, "installation aborted", std::string());
    }
    catch (const std::exception& ex)
    {
        SetStatus(Status::failed, "installation failed", ex.what());
    }
}

void Package::Uninstall()
{
    ResetAction();
    try
    {
        Node::Uninstall();
        SetStatus(Status::running, "running uninstall commands...", std::string());
        RunUninstallCommands();
        if (links)
        {
            SetStatus(Status::running, "removing links...", std::string());
            links->Uninstall();
        }
        if (environment)
        {
            SetStatus(Status::running, "removing environment variables...", std::string());
            environment->Uninstall();
        }
        for (const auto& component : components)
        {
            SetStatus(Status::running, "removing files...", std::string());
            component->Uninstall();
        }
        if (installationComponent)
        {
            SetStatus(Status::running, "removing installation information from registry...", std::string());
            installationComponent->RemoveInstallationInfo();
        }
        SetComponent(nullptr);
        SetFile(nullptr);
        SetStatus(Status::succeeded, "uninstallation succceeded", std::string());
    }
    catch (const AbortException&)
    {
        SetStatus(Status::aborted, "uninstallation aborted", std::string());
    }
    catch (const std::exception& ex)
    {
        SetStatus(Status::failed, "uninstallation failed", ex.what());
    }
}

void Package::RunUninstallCommands()
{
    int n = uninstallCommands.size();
    for (int i = 0; i < n; ++i)
    {
        RunUninstallCommand(uninstallCommands[i]);
    }
}

void Package::RunUninstallCommand(const std::string& uninstallCommand)
{
    int exitCode = 0;
    try
    {
        util::Process process(uninstallCommand, util::Process::Redirections::none);
        process.WaitForExit();
        exitCode = process.ExitCode();
        if (exitCode != 0)
        {
            throw std::runtime_error("uninstall action '" + uninstallCommand + "' returned exit code " + std::to_string(exitCode));
        }
    }
    catch (const std::exception& ex)
    {
        LogError(ex.what());
    }
    if (exitCode == 99)
    {
        throw AbortException();
    }
}

void Package::LogError(const std::string& error)
{
    for (PackageObserver* observer : observers)
    {
        observer->LogError(this, error);
    }
}

void Package::IncrementFileCount()
{
    ++fileCount;
}

void Package::IncrementFileIndex()
{
    ++fileIndex;
    NotifyFileIndexChanged();
}

void Package::IncrementFileContentSize(int64_t size)
{
    if (!includeFileContent) return;
    fileContentSize += size;
}

void Package::IncrementFileContentPosition(int64_t amount)
{
    if (!includeFileContent) return;
    fileContentPos += amount;
    NotifyFileContentPositionChanged();
}

util::Streams Package::GetReadBaseStream(DataSource dataSource, const std::string& filePath, uint8_t* data, int64_t size)
{
    util::Streams streams;
    if (dataSource == DataSource::memory)
    {
        streams.Add(new util::MemoryStream(data, size));
    }
    else if (dataSource == DataSource::file)
    {
        streams.Add(new util::FileStream(filePath, util::OpenMode::read | util::OpenMode::binary));
    }
    return streams;
}

void Package::AddReadCompressionStreams(util::Streams& streams, Compression comp)
{
    switch (comp)
    {
    case Compression::none:
    {
        break;
    }
    case Compression::deflate:
    {
        streams.Add(new util::DeflateStream(util::CompressionMode::decompress, streams.Back()));
        streams.Add(new util::BufferedStream(streams.Back()));
        break;
    }
/*
    case Compression::bzip2:
    {
        streams.Add(new BZip2Stream(CompressionMode::decompress, streams.Back()));
        streams.Add(new BufferedStream(streams.Back()));
        break;
    }
*/
    }
}

util::Streams Package::GetWriteStreams(const std::string& filePath)
{
    util::Streams streams;
    streams.Add(new util::FileStream(filePath, util::OpenMode::write | util::OpenMode::binary));
    switch (compression)
    {
    case Compression::none:
    {
        streams.Add(new util::BufferedStream(streams.Back()));
        break;
    }
    case Compression::deflate:
    {
        streams.Add(new util::BufferedStream(streams.Back()));
        streams.Add(new util::DeflateStream(util::CompressionMode::compress, streams.Back()));
        streams.Add(new util::BufferedStream(streams.Back()));
        break;
    }
/*
    case Compression::bzip2:
    {
        streams.Add(new BufferedStream(streams.Back()));
        streams.Add(new BZip2Stream(CompressionMode::compress, streams.Back()));
        streams.Add(new BufferedStream(streams.Back()));
        break;
    }
*/
    }
    return streams;
}

void Package::ResetAction()
{
    actionEvent.Reset();
    action = Action::continueAction;
    interrupted = false;
}

void Package::Interrupt()
{
    actionEvent.Reset();
    interrupted = true;
}

void Package::CheckInterrupted()
{
    if (interrupted)
    {
        Action action = GetAction();
        switch (action)
        {
        case Action::continueAction:
        {
            interrupted = false;
            return;
        }
        case Action::abortAction:
        {
            interrupted = false;
            throw AbortException();
        }
        }
        interrupted = false;
    }
}

Action Package::GetAction()
{
    actionEvent.WaitFor();
    return action;
}

void Package::SetAction(Action action_)
{
    action = action_;
    actionEvent.Set();
}

} // namespace wing_package
