// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing.metrics;

import dom;
import xpath;
import util;
import std.filesystem;

namespace wing {

std::string GetDefaultMetricsFilePath()
{
    std::string metricsFilePath;
    const char* wingstallRoot = getenv("WINGSTALL_ROOT");
    if (wingstallRoot && *wingstallRoot)
    {
        metricsFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(wingstallRoot, "config"), "metrics.xml"));
    }
    return metricsFilePath;
}

SizeElement::SizeElement(const std::string& name_) : name(name_)
{
}

void SizeElement::Read(xml::Element* parentElement)
{
    std::unique_ptr<xpath::XPathObject> result = xpath::Evaluate(name, parentElement);
    if (result)
    {
        if (result->Type() == xpath::XPathObjectType::nodeSet)
        {
            xpath::XPathNodeSet* nodeSet = static_cast<xpath::XPathNodeSet*>(result.get());
            int n = nodeSet->Length();
            if (n == 1)
            {
                xml::Node* node = (*nodeSet)[0];
                if (node->Kind() == xml::NodeKind::elementNode)
                {
                    xml::Element* element = static_cast<xml::Element*>(node);
                    size.Width = std::stod(element->GetAttribute("width"));
                    size.Height = std::stod(element->GetAttribute("height"));
                }
            }
        }
    }
}

void SizeElement::Write(xml::Element* parentElement)
{
    std::unique_ptr<xml::Element> element(xml::MakeElement(name));
    element->SetAttribute("width", std::to_string(size.Width));
    element->SetAttribute("height", std::to_string(size.Height));
    parentElement->AppendChild(element.release());
}

Metrics::Metrics() :
    dpiX(96), dpiY(96),
    defaultButtonSize(88, 25), defaultButtonSizeElement("button"),
    defaultLabelSize(71, 19), defaultLabelSizeElement("label"),
    defaultTextBoxSize(94, 14), defaultTextBoxSizeElement("textBox"),
    defaultComboBoxSize(121, 21), defaultComboBoxSizeElement("comboBox"),
    defaultListBoxSize(120, 96), defaultListBoxSizeElement("listBox"),
    defaultCheckBoxSize(80, 17), defaultCheckBoxSizeElement("checkBox"),
    defaultControlSpacing(10, 10), defaultControlSpacingElement("controlSpacing")
{
    SetElementValues();
}

void Metrics::SetElementValues()
{
    defaultButtonSizeElement.Set(SizeF(HorizontalPixelsToMM(defaultButtonSize.Width), VerticalPixelsToMM(defaultButtonSize.Height)));
    defaultLabelSizeElement.Set(SizeF(HorizontalPixelsToMM(defaultLabelSize.Width), VerticalPixelsToMM(defaultLabelSize.Height)));
    defaultTextBoxSizeElement.Set(SizeF(HorizontalPixelsToMM(defaultTextBoxSize.Width), VerticalPixelsToMM(defaultTextBoxSize.Height)));
    defaultComboBoxSizeElement.Set(SizeF(HorizontalPixelsToMM(defaultComboBoxSize.Width), VerticalPixelsToMM(defaultComboBoxSize.Height)));
    defaultListBoxSizeElement.Set(SizeF(HorizontalPixelsToMM(defaultListBoxSize.Width), VerticalPixelsToMM(defaultListBoxSize.Height)));
    defaultCheckBoxSizeElement.Set(SizeF(HorizontalPixelsToMM(defaultCheckBoxSize.Width), VerticalPixelsToMM(defaultCheckBoxSize.Height)));
    defaultControlSpacingElement.Set(SizeF(HorizontalPixelsToMM(defaultControlSpacing.Width), VerticalPixelsToMM(defaultControlSpacing.Height)));
}

void Metrics::LoadFromFile(const std::string& fileName)
{
    if (std::filesystem::exists(fileName))
    {
        std::unique_ptr<xml::Document> metricsDoc = xml::ReadDocument(fileName);
        Read(metricsDoc->DocumentElement());
    }
}

void Metrics::SaveToFile(const std::string& fileName, bool setElementValues)
{
    if (setElementValues)
    {
        SetElementValues();
    }
    xml::Document metricsDoc;
    metricsDoc.AppendChild(xml::MakeElement("metrics"));
    metricsDoc.DocumentElement()->AppendChild(xml::MakeComment("metrics are in millimeters"));
    metricsDoc.DocumentElement()->AppendChild(xml::MakeText("\n"));
    Write(metricsDoc.DocumentElement());
    std::ofstream metricsFile(fileName);
    util::CodeFormatter formatter(metricsFile);
    formatter.SetIndentSize(1);
    metricsDoc.Write(formatter);
}

void Metrics::Calculate(Graphics& graphics)
{
    dpiX = graphics.GetDpiX();
    dpiY = graphics.GetDpiY();
    defaultButtonSize.Width = MMToHorizontalPixels(defaultButtonSizeElement.Get().Width);
    defaultButtonSize.Height = MMToVerticalPixels(defaultButtonSizeElement.Get().Height);
    defaultLabelSize.Width = MMToHorizontalPixels(defaultLabelSizeElement.Get().Width);
    defaultLabelSize.Height = MMToVerticalPixels(defaultLabelSizeElement.Get().Height);
    defaultTextBoxSize.Width = MMToHorizontalPixels(defaultTextBoxSizeElement.Get().Width);
    defaultTextBoxSize.Height = MMToVerticalPixels(defaultTextBoxSizeElement.Get().Height);
    defaultComboBoxSize.Width = MMToHorizontalPixels(defaultComboBoxSizeElement.Get().Width);
    defaultComboBoxSize.Height = MMToHorizontalPixels(defaultComboBoxSizeElement.Get().Height);
    defaultListBoxSize.Width = MMToHorizontalPixels(defaultListBoxSizeElement.Get().Width);
    defaultListBoxSize.Height = MMToVerticalPixels(defaultListBoxSizeElement.Get().Height);
    defaultCheckBoxSize.Width = MMToHorizontalPixels(defaultCheckBoxSizeElement.Get().Width);
    defaultCheckBoxSize.Height = MMToVerticalPixels(defaultCheckBoxSizeElement.Get().Height);
    defaultControlSpacing.Width = MMToHorizontalPixels(defaultControlSpacingElement.Get().Width);
    defaultControlSpacing.Height = MMToVerticalPixels(defaultControlSpacingElement.Get().Height);
}

void Metrics::Read(xml::Element* parentElement)
{
    defaultButtonSizeElement.Read(parentElement);
    defaultLabelSizeElement.Read(parentElement);
    defaultTextBoxSizeElement.Read(parentElement);
    defaultComboBoxSizeElement.Read(parentElement);
    defaultListBoxSizeElement.Read(parentElement);
    defaultCheckBoxSizeElement.Read(parentElement);
    defaultControlSpacingElement.Read(parentElement);
}

void Metrics::Write(xml::Element* parentElement)
{
    defaultButtonSizeElement.Write(parentElement);
    defaultLabelSizeElement.Write(parentElement);
    defaultTextBoxSizeElement.Write(parentElement);
    defaultComboBoxSizeElement.Write(parentElement);
    defaultListBoxSizeElement.Write(parentElement);
    defaultCheckBoxSizeElement.Write(parentElement);
    defaultControlSpacingElement.Write(parentElement);
}

Metrics ScreenMetrics::metrics;

void ScreenMetrics::Load()
{
    std::string metricsFilePath = GetDefaultMetricsFilePath();
    if (!metricsFilePath.empty())
    {
        metrics.LoadFromFile(metricsFilePath);
    }
}

void ScreenMetrics::Save(bool setElementValues)
{
    metrics.SaveToFile(GetDefaultMetricsFilePath(), setElementValues);
}

void ScreenMetrics::Calculate(Graphics& graphics)
{
    Load();
    metrics.Calculate(graphics);
}

void ScreenMetrics::Set(Metrics& metrics_)
{
    metrics = metrics_;
}

void LoadMetrics()
{
    ScreenMetrics::Load();
}

bool ScreenMetrics::calculated = false;

} // wing
