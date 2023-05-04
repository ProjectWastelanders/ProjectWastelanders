#include "Headers.h"
#include "API_String.h"


API::API_String::API_String()
{

}

API::API_String::API_String(const char* s)
{
    len = strlen(s);
    str = new char[len + 1];
    strcpy_s(str,len + 1, s);
}

API::API_String::API_String(const API::API_String& rhs)
{
    len = rhs.len;
    str = new char[len + 1];
    strcpy_s(str, len+1, rhs.str);
}

API::API_String::~API_String()
{
    delete[] str;
    len = 0;
    str = nullptr;
}

int API::API_String::length() const
{
    return len;
}

const char* API::API_String::c_str() const
{
    return str;
}

char* API::API_String::ptr()
{
    return str;
}

API::API_String API::API_String::operator+(const API::API_String& rhs) const
{
    API::API_String temp;
    temp.len = len + rhs.len;
    temp.str = new char[temp.len + 1];
    strcpy_s(temp.str, temp.len+1, str);
    strcat_s(temp.str, len+1, rhs.str);
    return temp;
}

bool API::API_String::operator==(const API::API_String& rhs) const
{
    return strcmp(str, rhs.str) == 0;
}

bool API::API_String::operator==(const std::string& rhs) const
{
    return strcmp(str, rhs.c_str()) == 0;
}

bool API::API_String::operator!=(const API_String& rhs) const
{
    return strcmp(str, rhs.str) != 0;
}

bool API::API_String::operator!=(const std::string& rhs) const
{
    return strcmp(str, rhs.c_str()) != 0;
}

API::API_String& API::API_String::operator=(const API::API_String& rhs)
{
    if (this != &rhs) {
        delete[] str;
        len = rhs.len;
        str = new char[len + 1];
        strcpy_s(str, len+1, rhs.str);
    }
    return *this;
}

API::API_String& API::API_String::operator=(const std::string& rhs)
{
    delete[] str;
    len = rhs.length();
    str = new char[len + 1];
    strcpy_s(str, len+1, rhs.c_str());
    return *this;
}
