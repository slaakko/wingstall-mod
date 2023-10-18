// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module dom.axis;

namespace xml {

std::string AxisName(Axis axis)
{
    switch (axis)
    {
    case Axis::child: return "child";
    case Axis::descendant: return "descendant";
    case Axis::parent: return "parent";
    case Axis::ancestor: return "ancestor";
    case Axis::followingSibling: return "following-sibling";
    case Axis::precedingSibling: return "preceding-sibling";
    case Axis::following: return "following";
    case Axis::preceding: return "preceding";
    case Axis::attribute: return "attribute";
    case Axis::ns: return "namespace";
    case Axis::self: return "self";
    case Axis::descendantOrSelf: return "descendant-or-self";
    case Axis::ancestorOrSelf: return "ancestor-or-self";
    }
    return "<unknown axis>";
}

} // namespace xml
