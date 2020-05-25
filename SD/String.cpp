#include "String.h"

#include <string.h>

String operator+(const String& lhs, const String& rhs)
{
    String tmp = lhs;
    tmp += rhs;
    return tmp;
}

String operator+(const String& lhs, const char* rhs)
{
    String tmp = lhs;
    tmp += rhs;
    return tmp;
}

bool operator==(const String& lhs, const String& rhs)
{
    return strcmp(lhs.characters(), rhs.characters()) == 0;
}

bool operator!=(const String& lhs, const String& rhs)
{
    return strcmp(lhs.characters(), rhs.characters()) != 0;
}
