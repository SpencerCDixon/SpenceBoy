#include "String.h"

String operator+(const String& a, const String& b)
{
    String c = a;
    c += b;
    return c;
}

String operator+(const String& a, const char* b)
{
    String c = a;
    c += b;
    return c;
}