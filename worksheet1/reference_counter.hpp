#ifndef REFERENCE_COUNTER_H
#define REFERENCE_COUNTER_H

template<class T>
class reference_counter {
    private:
        T* ref_ptr = nullptr;
        unsigned int* ref_count = nullptr;
    
    public:
        reference_counter()
        {
            ref_count = new unsigned int(0);
        }

        reference_counter(T* ptr)
        {
            ref_ptr = ptr;
            ref_count = new unsigned int(1); 
        }

        reference_counter(const reference_counter& object){
            this->ref_ptr = object.ref_ptr;
            this->ref_count = object.ref_count;

            (*this->ref_count)++;
        }
        
        T* operator->() const
        {
                return this->ref_ptr;
        }

        unsigned int getCount()
        {
            return *ref_count;
        }

        ~reference_counter(){
            (*ref_count)--;

            if((*ref_count) == 0){
                std::cout << "Ref count equals 0, freeing memory..." << std::endl;
                delete ref_ptr;
                delete ref_count;
            }
        }

};

#endif