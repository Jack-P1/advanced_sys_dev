#include <iostream>
#include "reference_counter.hpp"

reference_counter::reference_counter(){
    ref_count = 0;
    ref_count_ptr = &ref_count;
}

void reference_counter::increment()
{
    (*ref_count_ptr)++;
}

void reference_counter::decrement()
{
    (*ref_count_ptr)--;
}

unsigned int reference_counter::getCount() const
{
    return *ref_count_ptr;
}

reference_counter& reference_counter::operator= (reference_counter const& r)
{
    ref_count_ptr = r.ref_count_ptr;

    return *this;
}