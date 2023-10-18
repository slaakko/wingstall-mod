// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.json;

import std.core;
import util.code_formatter;

export namespace util {

export namespace json {}

enum class JsonValueType : int
{
    none = 0,
    object = 1 << 0,
    array = 1 << 1,
    string = 1 << 2,
    number = 1 << 3,
    boolean = 1 << 4,
    null = 1 << 5,
    any = object | array | string | number | boolean | null
};

constexpr JsonValueType operator|(JsonValueType left, JsonValueType right)
{
    return JsonValueType(int(left) | int(right));
}

constexpr JsonValueType operator&(JsonValueType left, JsonValueType right)
{
    return JsonValueType(int(left) & int(right));
}

constexpr JsonValueType operator~(JsonValueType type)
{
    return JsonValueType(~int(type));
}

std::string JsonValueTypeStr(JsonValueType type);

class JsonValue
{
public:
    JsonValue(JsonValueType type_);
    JsonValue(const JsonValue&) = delete;
    JsonValue& operator=(const JsonValue&) = delete;
    virtual ~JsonValue();
    virtual JsonValue* Clone() const = 0;
    JsonValueType Type() const { return type; }
    bool IsObject() const { return type == JsonValueType::object; }
    bool IsArray() const { return type == JsonValueType::array; }
    bool IsStructuredValue() const { return IsObject() || IsArray(); }
    bool IsString() const { return type == JsonValueType::string; }
    bool IsNumber() const { return type == JsonValueType::number; }
    bool IsBoolean() const { return type == JsonValueType::boolean; }
    bool IsNull() const { return type == JsonValueType::null; }
    virtual std::string ToString() const = 0;
    virtual void Write(CodeFormatter& formatter);
private:
    JsonValueType type;
};

class JsonString : public JsonValue
{
public:
    JsonString();
    JsonString(const std::u32string& value_);
    void Append(char32_t c);
    JsonValue* Clone() const override;
    const std::u32string& Value() const { return value; }
    void SetValue(const std::u32string& value_);
    std::u16string JsonCharStr(char32_t c) const;
    std::string ToString() const override;
private:
    std::u32string value;
};

class JsonNumber : public JsonValue
{
public:
    JsonNumber();
    JsonNumber(double value_);
    JsonValue* Clone() const override;
    double Value() const { return value; }
    std::string ToString() const override;
private:
    double value;
};

class JsonBool : public JsonValue
{
public:
    JsonBool();
    JsonBool(bool value_);
    JsonValue* Clone() const override;
    bool Value() const { return value; }
    std::string ToString() const override;
private:
    bool value;
};

class JsonArray;

class JsonObject : public JsonValue
{
public:
    JsonObject();
    void AddField(const std::u32string& fieldName, std::unique_ptr<JsonValue>&& fieldValue);
    int FieldCount() const { return fieldValues.size(); }
    JsonValue* GetField(const std::u32string& fieldName) const;
    bool HasField(const std::u32string& fieldName) const;
    JsonString* GetStringField(const std::u32string& fieldName) const;
    JsonNumber* GetNumberField(const std::u32string& fieldName) const;
    JsonBool* GetBooleanField(const std::u32string& fieldName) const;
    JsonObject* GetObjectField(const std::u32string& fieldName) const;
    JsonArray* GetArrayField(const std::u32string& fieldName) const;
    JsonValue* Clone() const override;
    std::string ToString() const override;
    void Write(CodeFormatter& formatter) override;
private:
    std::vector<std::unique_ptr<JsonValue>> fieldValues;
    std::map<std::u32string, JsonValue*> fieldMap;
};

class JsonArray : public JsonValue
{
public:
    JsonArray();
    void AddItem(std::unique_ptr<JsonValue>&& item);
    int Count() const { return items.size(); }
    JsonValue* GetItem(int index) const;
    JsonValue* operator[](int index) const;
    JsonValue* Clone() const override;
    std::string ToString() const override;
    void Write(CodeFormatter& formatter) override;
private:
    std::vector<std::unique_ptr<JsonValue>> items;
};

class JsonNull : public JsonValue
{
public:
    JsonNull();
    JsonValue* Clone() const override;
    std::string ToString() const override;
};

} // namespace util
