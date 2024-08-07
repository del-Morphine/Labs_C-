#ifndef STRING_H
#define STRING_H

#include <iostream>

using namespace std;
class String
{
    private:
        char *str;
        int len;
    
    public:
        String(const char *str);
        String(int len = 0);
        String(const String& other);
        ~String();

        int Len() {return len;};
        String& operator=(const String& other);
        char& operator[](int index);
        String operator+(const String& other);
        String operator+(const char* other);
        String& operator+=(const String& other);
        bool operator==(const String& other);
        bool operator==(const char* other);
        bool operator!=(const String& other);
        bool operator!=(const char* other);
        friend istream& operator>>(istream& in, String& other);
        friend ostream& operator<<(ostream& out, String& other);
//        friend ostream& operator<<(ostream& out, String other);

        int BMH_search(const char* pattern);
        int BMH_search(const String& pattern);
};

#endif