#ifndef MY_STRING_H
#define MY_STRING_H

#include "reference_counter.hpp"

#include <iostream>
#include <cstring>

class my_string {
    public:
        my_string();
        my_string(const char*);
        my_string(my_string const& s);
        my_string& operator= (my_string const& s);
        ~my_string();
        char getChar(const int& i) const;
        void setChar(const int& i, const char& c);
        void print() const;

    private:
        reference_counter reference;
        unsigned int m_size;
        char* m_data;
};

#endif