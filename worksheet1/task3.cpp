#include "my_string.hpp"
#include <iostream>
#include <cstring>
#include <stdexcept>

my_string::my_string()
{
    m_data = nullptr;
    m_size= 0;
    reference.increment();
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

    reference.increment();

}

my_string& my_string::operator= (my_string const& s)
{
    m_data = s.m_data;
    m_size = s.m_size;
    reference = s.reference;

    reference.increment();

    return *this;
}

my_string::my_string(my_string const& s)
{
    m_data = s.m_data;
    m_size = s.m_size;
    reference = s.reference;

    reference.increment();
}

my_string::~my_string()
{
    // this is greater than 1 as it includes itself (might need to revisit)
    if(reference.getCount() > 1)
    {
        reference.decrement();
    }
    else{
        std::cout << "Reference count = 0, freeing memory..." << std::endl;
        delete m_data;
    }

}

void my_string::print() const
{
    std::cout << m_data << " [" << reference.getCount() << "] " << std::endl;

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