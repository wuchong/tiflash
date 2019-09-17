#pragma once

#include <Core/Field.h>

namespace DB
{

struct MyTimeBase
{

    enum MyTimeType : UInt8
    {
        TypeDate = 0,
        TypeDateTime,
        TypeTimeStamp,
        TypeDuration
    };

    UInt16 year; // year <= 9999
    UInt8 month; // month <= 12
    UInt8 day;   // day <= 31
    // When it's type is Time, HH:MM:SS may be 839:59:59 to -839:59:59, so use int16 to avoid overflow
    Int16 hour;
    UInt8 minute;
    UInt8 second;
    UInt32 micro_second; // ms second <= 999999

    UInt8 fsp;

    MyTimeBase() = default;
    MyTimeBase(UInt64 packed, UInt8 fsp_ = 0);
    MyTimeBase(UInt16 year_, UInt8 month_, UInt8 day_, UInt16 hour_, UInt8 minute_, UInt8 second_, UInt32 micro_second_, UInt8 fsp_);

    UInt64 toPackedUInt() const;

    // DateFormat returns a textual representation of the time value formatted
    // according to layout
    // See http://dev.mysql.com/doc/refman/5.7/en/date-and-time-functions.html#function_date-format
    String dateFormat(const String & layout) const;

protected:
    void convertDateFormat(char c, String & result) const;
};

struct MyDateTime : public MyTimeBase
{
    MyDateTime(UInt64 packed, UInt8 fsp) : MyTimeBase(packed, fsp) {}

    MyDateTime(UInt16 year_, UInt8 month_, UInt8 day_, UInt16 hour_, UInt8 minute_, UInt8 second_, UInt32 micro_second_, UInt8 fsp_)
        : MyTimeBase(year_, month_, day_, hour_, minute_, second_, micro_second_, fsp_)
    {}

    String toString() const;
};

struct MyDate : public MyTimeBase
{
    MyDate(UInt64 packed) : MyTimeBase(packed) {}

    MyDate(UInt16 year_, UInt8 month_, UInt8 day_) : MyTimeBase(year_, month_, day_, 0, 0, 0, 0, 0) {}

    String toString() const { return dateFormat("%Y-%m-%d"); }
};

Field parseMyDateTime(const String & str);

} // namespace DB