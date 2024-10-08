#include "my_string.hpp"
#include "reference_counter.hpp"
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
    m_data = s.m_data;
    m_size = s.m_size;

    return *this;
}

my_string::my_string(my_string const& s)
{
    m_data = s.m_data;
    m_size = s.m_size;
}

// my_string::~my_string()
// {
//     // this is greater than 1 as it includes itself (might need to revisit)
//     if(reference.getCount() > 1)
//     {
//         reference.decrement();
//     }
//     else{
//         std::cout << "Reference count = 0, freeing memory..." << std::endl;
//         delete m_data;
//     }

// }

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

int main()
{

    reference_counter<my_string> string1(new my_string("Hello world"));
    string1->print();
    std::cout << string1.getCount() << std::endl;

    {
        reference_counter<my_string> string2(string1);
        std::cout << string1.getCount() << std::endl;
    } // string 2 out of scope
`
    // count down to 1
    std::cout << string1.getCount() << std::endl;

    // destroy string 1
    // delete [] string1;

    return 0;
}