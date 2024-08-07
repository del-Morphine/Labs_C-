#include "String.h"

String::String(const char *str)
{
    len = 0;
    for(len; str[len]; len++);
    
    this->str = new char[len + 1];
    for(int i = 0; str[i]; i++)
        this->str[i] = str[i];
    this->str[len] = '\0';
}

String::String(int len)
{
    this->len = len;
    str = new char[len + 1];
    str[0] = '\0';
}

String::String(const String& other)
{
    len = other.len;
    str = new char[len + 1];
    for(int i = 0; i < len; i++)
        str[i] = other.str[i];
    str[len] = '\0';
}

String::~String()
{
    if(str == NULL)
        return;
    delete[] str;
    str = NULL;
}

String& String::operator=(const String& other)
{
    if(this == &other)
        return *this;
    if(this->str != NULL)
        delete[] str;
    len = other.len;
    str = new char[len + 1];
    for(int i = 0; i < len; i++)
        str[i] = other.str[i];
    str[len] = '\0';
    return *this;
}

char& String::operator[](int index)
{
    if(index < 0 || index >= len)
        exit(EXIT_FAILURE);
    return str[index];
}

String String::operator+(const String& other)
{
    String result(len + other.len);
    int i = 0;
    for(i; i < len; i++)
        result.str[i] = str[i];
    for(int j = 0 ; j < len + other.len; j++)
        result.str[i+j] = other.str[j];
    return result; 
}

String String::operator+(const char* other)
{
    int other_len = 0;
    for(other_len; other[other_len]; ++other_len);
    
    String result(len + other_len);    
    int i = 0;
    for(i; i < len; i++)
        result.str[i] = str[i];
    for(int j = 0 ; j < len + other_len; j++)
        result.str[i+j] = other[j];
    return result;     
}

String& String::operator+=(const String& other)
{   
    char* new_str = new char[len + other.len + 1];
    int i = 0;
    for(i; i < len; i++)
        new_str[i] = str[i];
    for(int j = 0; j < other.len; j++)
        new_str[i+j] = other.str[j];
    if(str != NULL)
        delete[] str; 
    return *this;  
}

bool String::operator==(const String& other)
{
    for(int i = 0; str[i]; i++)
    {
        if(str[i] != other.str[i])
            return false;
    }
    return true;
}

bool String::operator==(const char *other)
{
    for(int i = 0; str[i]; i++)
    {
        if(str[i] != other[i])
            return false;
    }
    return true;
}

bool String::operator!=(const String& other)
{
    for(int i = 0; str[i]; i++)
    {
        if(str[i] != other.str[i])
            return true;
    }
    return false;    
}

bool String::operator!=(const char *other)
{
    for(int i = 0; str[i]; i++)
    {
        if(str[i] != other[i])
            return true;
    }
    return false;
}

istream& operator>>(istream& in, String& other)
{
    char buffer[257];
    in.getline(buffer, 256);
    
    other.len = 0;
    for(other.len; buffer[other.len]; ++other.len);
    other.str[other.len] = '\0';
    
    if(other.str != NULL)
        delete[] other.str;
    other.str = new char[other.len + 1];
    
    for(int i = 0; i < other.len; i++)
        other.str[i] = buffer[i];
    return in;
}
ostream& operator<<(ostream& out, String& other)
{
    out << other.str;
    return out; 
}
// ostream& operator<<(ostream& out, String other)
// {
//     out << other.str;
//     return out;
// }

int String::BMH_search(const char* pattern)
{
    if(pattern == NULL)
        return -1;
    int pattern_len = 0;
    for(pattern_len; pattern[pattern_len]; ++pattern_len);

    int table[256];
    for(int i = 0; i < 256; ++i)
        table[i] = pattern_len;
    for(int i = 0; i < pattern_len - 1; ++i)    
        table[pattern[i]] = pattern_len - 1 - i;

    int i = pattern_len - 1;
    int j = pattern_len - 1;
    int k;
    while(i < len && j >= 0)
    {   
        j = pattern_len - 1;
        k = i;
        while(j >= 0)
        {
            if(str[k] == pattern[j])
            {
                --j;
                --k;
            }
            else
            {
                i += table[str[i]];
                j = pattern_len - 1;
                break;
            }
        }
    }
    if(j >= 0)
        return -1;
    return i+1-pattern_len;
}

int String::BMH_search(const String& pattern)
{
    int table[256];
    for(int i = 0; i < 256; ++i)
        table[i] = pattern.len;
    for(int i = 0; i < pattern.len - 1; ++i)    
        table[pattern.str[i]] = pattern.len - 1 - i;

    int i = pattern.len - 1;
    int j = pattern.len - 1;
    int k;
    while(i < len && j >= 0)
    {   
        j = pattern.len - 1;
        k = i;
        while(j >= 0)
        {
            if(str[k] == pattern.str[j])
            {
                --j;
                --k;
            }
            else
            {
                i += table[str[i]];
                j = pattern.len - 1;
                break;
            }
        }
    }
    if(j >= 0)
        return -1;
    return i+1-pattern.len;    
}