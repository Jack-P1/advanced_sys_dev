#include "my_string.hpp"
#include <iostream>
#include <cstring>
#include <stdexcept>

my_string::my_string()
{
    m_data = nullptr;
    m_size= 0;
}

my_string::my_string(const char* pos)
{
    const char* start_ptr = pos;

    unsigned int x = 0;
    while(*pos != '\0'){
        pos++;
        x++;
    }
    
    m_size = x;

    m_data = new char[m_size];

    for(int i = 0; i < m_size; i++){
        m_data[i] = *start_ptr;
        start_ptr++;
    }

}

my_string& my_string::operator= (my_string const& s)
{
    // self assignment check
    if(this == &s){
        return *this;
    }
    m_data = s.m_data;
    m_size = s.m_size;

    return *this;
}

my_string::my_string(my_string const& s)
{
    m_data = s.m_data;
    m_size = s.m_size;
}

void my_string::print() const
{
    std::cout << m_data << std::endl;

}

char my_string::getChar(const int& i) const
{
    if(i > m_size || i < 0){
        throw std::invalid_argument("Index out of bounds");
    }

    return m_data[i];

}

void my_string::setChar(const int& i, const char& c)
{
    if(i > m_size || i < 0){
        throw std::invalid_argument("Index out of bounds");
    }

    m_data[i] = c;

}

int main(){

    my_string s("Hello world");
    s.print();
    {
        my_string t = s;
        s.print();
        t.print();
        std::cout << s.getChar(1) << std::endl;
        s.print();
        t.print();
    }
    s.setChar(1,'E');
    s.print();

    return 0;
}