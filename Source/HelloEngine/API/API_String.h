#pragma once
#include "Globals.h"
#include <iostream>
#include <cstring>

namespace API
{
    class TO_API API_String
    {
    private:
        char* str = nullptr;
        int len = 0;

    public:
        API_String();

        API_String(const char* s);

        API_String(const API_String& rhs);

        ~API_String();

        int length() const;

        const char* c_str() const;

        char* ptr();

        API_String operator+ (const API_String& rhs) const;

        bool operator== (const API_String& rhs) const;
        bool operator== (const std::string& rhs) const;

        bool operator!= (const API_String& rhs) const;
        bool operator!= (const std::string& rhs) const;

        API_String& operator= (const API_String& rhs);
        API_String& operator= (const std::string& rhs);

    };
}

