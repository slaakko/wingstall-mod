// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.json;

import util.unicode;
import util.text_util;

namespace util {

std::string JsonValueTypeStr(JsonValueType type)
{
    std::string str;
    if ((type & JsonValueType::object) != JsonValueType::none)
    {
        if (!str.empty())
        {
            str.append(" | ");
        }
        str.append("object");
    }
    if ((type & JsonValueType::array) != JsonValueType::none)
    {
        if (!str.empty())
        {
            str.append(" | ");
        }
        str.append("array");
    }
    if ((type & JsonValueType::string) != JsonValueType::none)
    {
        if (!str.empty())
        {
            str.append(" | ");
        }
        str.append("string");
    }
    if ((type & JsonValueType::number) != JsonValueType::none)
    {
        if (!str.empty())
        {
            str.append(" | ");
        }
        str.append("number");
    }
    if ((type & JsonValueType::boolean) != JsonValueType::none)
    {
        if (!str.empty())
        {
            str.append(" | ");
        }
        str.append("boolean");
    }
    return str;
}

JsonValue::JsonValue(JsonValueType type_) : type(type_)
{
}

JsonValue::~JsonValue()
{
}

void JsonValue::Write(CodeFormatter& formatter)
{
    formatter.Write(ToString());
}

JsonString::JsonString() : JsonValue(JsonValueType::string), value()
{
}

JsonString::JsonString(const std::u32string& value_) : JsonValue(JsonValueType::string), value(value_)
{
}

JsonValue* JsonString::Clone() const
{
    return new JsonString(value);
}

void JsonString::SetValue(const std::u32string& value_)
{
    value = value_;
}

void JsonString::Append(char32_t c)
{
    value.append(1, c);
}

std::u16string JsonString::JsonCharStr(char32_t c) const
{
    std::u16string result;
    std::u32string s(1, c);
    std::u16string t = ToUtf16(s);
    for (char16_t x : t)
    {
        switch (x)
        {
        case '"': result.append(u"\\\""); break;
        case '\\': result.append(u"\\\\"); break;
        case '\b': result.append(u"\\b"); break;
        case '\f': result.append(u"\\f"); break;
        case '\n': result.append(u"\\n"); break;
        case '\r': result.append(u"\\r"); break;
        case '\t': result.append(u"\\t"); break;
        default:
        {
            if (x >= 32 && x <= 126)
            {
                result.append(1, x);
            }
            else
            {
                result.append(u"\\u").append(ToUtf16(ToHexString(static_cast<uint16_t>(x))));
            }
        }
        }
    }
    return result;
}

std::string JsonString::ToString() const
{
    std::u16string s;
    for (char32_t c : value)
    {
        s.append(JsonCharStr(c));
    }
    return "\"" + ToUtf8(s) + "\"";
}

JsonNumber::JsonNumber() : JsonValue(JsonValueType::number), value(0.0)
{
}

JsonNumber::JsonNumber(double value_) : JsonValue(JsonValueType::number), value(value_)
{
}

JsonValue* JsonNumber::Clone() const
{
    return new JsonNumber(value);
}

std::string JsonNumber::ToString() const
{
    if (value == static_cast<int64_t>(value))
    {
        return std::to_string(static_cast<int64_t>(value));
    }
    else
    {
        return std::to_string(value);
    }
}

JsonBool::JsonBool() : JsonValue(JsonValueType::boolean), value(false)
{
}

JsonBool::JsonBool(bool value_) : JsonValue(JsonValueType::boolean), value(value_)
{
}

JsonValue* JsonBool::Clone() const
{
    return new JsonBool(value);
}

std::string JsonBool::ToString() const
{
    return value ? "true" : "false";
}

JsonObject::JsonObject() : JsonValue(JsonValueType::object), fieldValues(), fieldMap()
{
}

void JsonObject::AddField(const std::u32string& fieldName, std::unique_ptr<JsonValue>&& fieldValue)
{
    fieldMap[fieldName] = fieldValue.get();
    fieldValues.push_back(std::move(fieldValue));
}

JsonValue* JsonObject::GetField(const std::u32string& fieldName) const
{
    auto it = fieldMap.find(fieldName);
    if (it != fieldMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

bool JsonObject::HasField(const std::u32string& fieldName) const
{
    return GetField(fieldName) != nullptr;
}

JsonString* JsonObject::GetStringField(const std::u32string& fieldName) const
{
    JsonValue* value = GetField(fieldName);
    if (value)
    {
        if (value->IsString())
        {
            JsonString* str = static_cast<JsonString*>(value);
            return str;
        }
        else
        {
            throw std::runtime_error("error getting field " + ToUtf8(fieldName) + ": string field expected");
        }
    }
    else
    {
        return nullptr;
    }
}

JsonNumber* JsonObject::GetNumberField(const std::u32string& fieldName) const
{
    JsonValue* value = GetField(fieldName);
    if (value)
    {
        if (value->IsNumber())
        {
            JsonNumber* number = static_cast<JsonNumber*>(value);
            return number;
        }
        else
        {
            throw std::runtime_error("error getting field " + ToUtf8(fieldName) + ": number field expected");
        }
    }
    else
    {
        return nullptr;
    }
}

JsonBool* JsonObject::GetBooleanField(const std::u32string& fieldName) const
{
    JsonValue* value = GetField(fieldName);
    if (value)
    {
        if (value->IsBoolean())
        {
            JsonBool* boolean = static_cast<JsonBool*>(value);
            return boolean;
        }
        else
        {
            throw std::runtime_error("error getting field " + ToUtf8(fieldName) + ": boolean field expected");
        }
    }
    else
    {
        return nullptr;
    }
}

JsonObject* JsonObject::GetObjectField(const std::u32string& fieldName) const
{
    JsonValue* value = GetField(fieldName);
    if (value)
    {
        if (value->IsObject())
        {
            JsonObject* object = static_cast<JsonObject*>(value);
            return object;
        }
        else
        {
            throw std::runtime_error("error getting field " + ToUtf8(fieldName) + ": object field expected");
        }
    }
    else
    {
        return nullptr;
    }
}

JsonArray* JsonObject::GetArrayField(const std::u32string& fieldName) const
{
    JsonValue* value = GetField(fieldName);
    if (value)
    {
        if (value->IsArray())
        {
            JsonArray* array = static_cast<JsonArray*>(value);
            return array;
        }
        else
        {
            throw std::runtime_error("error getting field " + ToUtf8(fieldName) + ": array field expected");
        }
    }
    else
    {
        return nullptr;
    }
}

JsonValue* JsonObject::Clone() const
{
    JsonObject* clone = new JsonObject();
    for (const auto& p : fieldMap)
    {
        clone->AddField(p.first, std::unique_ptr<JsonValue>(p.second->Clone()));
    }
    return clone;
}

std::string JsonObject::ToString() const
{
    std::string str = "{";
    bool first = true;
    for (const auto& p : fieldMap)
    {
        JsonString s(p.first);
        JsonValue* v(p.second);
        if (first)
        {
            first = false;
        }
        else
        {
            str.append(", ");
        }
        str.append(s.ToString()).append(":").append(v->ToString());
    }
    str.append("}");
    return str;
}

void JsonObject::Write(CodeFormatter& formatter)
{
    formatter.WriteLine("{");
    formatter.IncIndent();
    JsonValueType lastItemType = JsonValueType::object;
    bool first = true;
    for (const auto& p : fieldMap)
    {
        JsonString s(p.first);
        JsonValue* v(p.second);
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.WriteLine(",");
        }
        s.Write(formatter);
        formatter.Write(": ");
        if (v->IsObject())
        {
            formatter.WriteLine();
            formatter.IncIndent();
        }
        if (s.Value() == U"content")
        {
            formatter.BeginContent();
        }
        v->Write(formatter);
        if (s.Value() == U"content")
        {
            formatter.EndContent();
        }
        if (v->IsObject())
        {
            formatter.DecIndent();
        }
        lastItemType = v->Type();
    }
    formatter.DecIndent();
    if (lastItemType != JsonValueType::array && lastItemType != JsonValueType::object)
    {
        formatter.WriteLine();
    }
    formatter.WriteLine("}");
}

JsonArray::JsonArray() : JsonValue(JsonValueType::array)
{
}

void JsonArray::AddItem(std::unique_ptr<JsonValue>&& item)
{
    items.push_back(std::move(item));
}

JsonValue* JsonArray::Clone() const
{
    JsonArray* clone = new JsonArray();
    for (const std::unique_ptr<JsonValue>& item : items)
    {
        clone->AddItem(std::unique_ptr<JsonValue>(item->Clone()));
    }
    return clone;
}

JsonValue* JsonArray::GetItem(int index) const
{
    if (index >= 0 && index < items.size())
    {
        return items[index].get();
    }
    else
    {
        throw std::runtime_error("invalid JSON array index");
    }
}

JsonValue* JsonArray::operator[](int index) const
{
    return GetItem(index);
}

std::string JsonArray::ToString() const
{
    std::string str = "[";
    bool first = true;
    for (const std::unique_ptr<JsonValue>& item : items)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            str.append(", ");
        }
        str.append(item->ToString());
    }
    str.append("]");
    return str;
}

void JsonArray::Write(CodeFormatter& formatter)
{
    formatter.WriteLine("[");
    formatter.IncIndent();
    JsonValueType lastItemType = JsonValueType::array;
    bool first = true;
    for (const std::unique_ptr<JsonValue>& item : items)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.WriteLine(",");
        }
        item->Write(formatter);
        lastItemType = item->Type();
    }
    formatter.DecIndent();
    if (lastItemType != JsonValueType::array && lastItemType != JsonValueType::object)
    {
        formatter.WriteLine();
    }
    formatter.WriteLine("]");
}

JsonNull::JsonNull() : JsonValue(JsonValueType::null)
{
}

JsonValue* JsonNull::Clone() const
{
    return new JsonNull();
}

std::string JsonNull::ToString() const
{
    return "null";
}

} // namespace util
