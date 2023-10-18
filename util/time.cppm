// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.time;

import std.core;
import std.filesystem;

export namespace util {

enum class Month : int8_t
{
    january = 1, february, march, april, may, june, july, august, september, october, november, december
};

int GetMonthDays(Month month, int year);

class Date
{
public:
    Date() : year(0), month(Month::january), day(1)
    {
    }
    Date(short year_, Month month_, int8_t day_) : year(year_), month(month_), day(day_)
    {
    }
    int16_t Year() const
    {
        return year;
    }
    Month GetMonth() const
    {
        return month;
    }
    int8_t Day() const
    {
        return day;
    }
    Date AddDays(int n);
    Date AddMonths(int n);
    Date AddYears(int n);
    std::string ToString() const;
    std::string ToString(bool omitDashes) const;
private:
    int16_t year;
    Month month;
    int8_t day;
};

Date GetCurrentDate();

bool operator==(const Date& left, const Date& right);

inline bool operator!=(const Date& left, const Date& right)
{
    return !(left == right);
}

bool operator<(const Date& left, const Date& right);

inline bool operator>(const Date& left, const Date& right)
{
    return right < left;
}

inline bool operator<=(const Date& left, const Date& right)
{
    return !(right > left);
}

inline bool operator>=(const Date& left, const Date& right)
{
    return !(left < right);
}

Date ParseDate(const std::string& dateStr);

class DateTime
{
public:
    DateTime() : date(), secs(0)
    {
    }
    DateTime(Date date_) : date(date_), secs(0)
    {
    }
    DateTime(Date date_, int32_t secs_) : date(date_), secs(secs_)
    {
    }
    Date GetDate() const
    {
        return date;
    }
    int32_t Hours() const
    {
        return secs / 3600;
    }
    int32_t Minutes() const
    {
        return secs / 60;
    }
    int32_t Seconds() const
    {
        return secs;
    }
    std::string ToString() const;
    std::string ToString(bool omitDashes, bool omitColons, bool omitMins, bool omitSecs) const;
private:
    Date date;
    int32_t secs;
};

DateTime GetCurrentDateTime();

bool operator==(const DateTime& left, const DateTime& right);

inline bool operator!=(const DateTime& left, const DateTime& right)
{
    return !(left == right);
}

bool operator<(const DateTime& left, const DateTime& right);

inline bool operator>(const DateTime& left, const DateTime& right)
{
    return right < left;
}

inline bool operator<=(const DateTime& left, const DateTime& right)
{
    return !(right > left);
}

inline bool operator>=(const DateTime& left, const DateTime& right)
{
    return !(left < right);
}

DateTime ParseDateTime(const std::string& dateTimeStr);

class Timestamp
{
public:
    Timestamp() : dateTime(), nanosecs(0) {}
    Timestamp(Date date_) : dateTime(date_), nanosecs(0) {}
    Timestamp(Date date_, int32_t secs_) : dateTime(date_, secs_), nanosecs(0) {}
    Timestamp(const DateTime& dateTime_) : dateTime(dateTime_), nanosecs(0) {}
    Timestamp(const DateTime& dateTime_, int32_t nanosecs_) : dateTime(dateTime_), nanosecs(nanosecs_) {}
    const DateTime& GetDateTime() const { return dateTime; }
    int32_t Nanoseconds() const { return nanosecs; }
    std::string ToString() const;
private:
    DateTime dateTime;
    int32_t nanosecs;
};

const int secsInDay = 24 * 3600;

bool operator==(const Timestamp& left, const Timestamp& right);

inline bool operator!=(const Timestamp& left, const Timestamp& right)
{
    return !(left == right);
}

bool operator<(const Timestamp& left, const Timestamp& right);

inline bool operator>(const Timestamp& left, const Timestamp& right)
{
    return right < left;
}

inline bool operator<=(const Timestamp& left, const Timestamp& right)
{
    return !(right > left);
}

inline bool operator>=(const Timestamp& left, const Timestamp& right)
{
    return !(left < right);
}

Timestamp GetCurrentTimestamp();

Timestamp ParseTimestamp(const std::string& timestampStr);

std::string FormatTimeMs(int32_t milliseconds);

std::int64_t CurrentMs();

int64_t GetCurrentTime();

std::string DurationStr(const std::chrono::nanoseconds& duration);

std::time_t Time();

std::time_t MkTime(const DateTime& dt);

DateTime ToDateTime(time_t time);

DateTime ToDateTime(const std::filesystem::file_time_type& time);

std::string TimeToString(std::time_t time);

std::string FileTimeToString(const std::chrono::file_clock::time_point& fileTime);

std::chrono::file_clock::time_point ParseFileTime(int64_t nanoseconds);

void TimeInit();
void TimeDone();

} // namespace soulng::util
