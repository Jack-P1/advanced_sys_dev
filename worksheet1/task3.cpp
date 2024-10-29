#include "my_string.hpp"
#include <iostream>
#include <cstring>
#include <stdexcept>

my_string::my_string()
{
    m_data = nullptr;
    m_size= 0;
    ref_count = new unsigned int(0);
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

    ref_count = new unsigned int(1);
}

my_string& my_string::operator= (my_string const& s)
{
    // self assignment check
    if (this == &s){
        return *this;
    }

    // cleanup data of the current object if this is the final reference
    if(--(*ref_count) == 0){
        delete[] m_data;
        delete[] ref_count;
    }
    
    // copy data and reference count
    this->m_data = s.m_data;
    this->m_size = s.m_size;
    this->ref_count = s.ref_count;

    (*ref_count)++;


    return *this;
}

my_string::my_string(my_string const& s)
{
    this->m_data = s.m_data;
    this->m_size = s.m_size;
    this->ref_count = s.ref_count;

    (*ref_count)++;
}

my_string::~my_string()
{
    if (--(*ref_count) == 0) {
        std::cout << "Reference counter equals 0, freeing memory..." << std::endl;
        delete[] m_data;
        delete ref_count;
    }
}

void my_string::print() const
{
    std::cout << m_data << " [" << *ref_count << "] " << std::endl;
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

int main()
{
    /*
        Example of reference count reaching 0
    */
    
    // manually create object on the heap
    my_string* s = new my_string("Hello, world!");
    
    {
        my_string t = *s;
        s->print();
        t.print();
        std::cout << s->getChar(1) << std::endl;
    } // t goes out of scope
    
    // ref count down to 1
    s->print();

    //destroy s
    delete s;

    return 0;
}