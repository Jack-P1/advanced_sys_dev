#ifndef REFERENCE_COUNTER_H
#define REFERENCE_COUNTER_H

class reference_counter {
    public:
        reference_counter();
        void increment();
        void decrement();
        unsigned int getCount() const;
        reference_counter& operator= (reference_counter const& r);

    private:
        unsigned int ref_count;
        unsigned int* ref_count_ptr;
};

#endif