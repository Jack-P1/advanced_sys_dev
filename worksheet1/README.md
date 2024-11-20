# Worksheet 1

## Task 1

For task 1, I implemented a basic string class using the interface provided in the worksheet, which I declared in 'my_string.hpp':

```c++
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
        unsigned int m_size;
        unsigned int* ref_count = nullptr;
        char* m_data;
};
```

In addition to the provided function signatures, I declared some class variables as part of my implementation:
- `m_size` is used to hold the size of the string
- `m_data` is a char pointer to point to and access the data

These functions are then defined in `task1.cpp`.

```c++
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
```
The above constructor dynamically allocated memory by iterating over the provided `char` pointer until the null terminator. Then it calls the `new` keywork to allocate memory on the heap of the requested size (`m_size`).


## Task 2

For this, I extended my string class shown in task 1 to support automatic reference counting. This is a memory management technique which tracks how many references exist to an object and frees it when it is no longer needed. The flaw with the implementation of the previous task being that `m_data` is never freed and there is no way of tracking when multiple references exist to this data.

I implemented reference counting by:
- Creating a pointer to an unsigned integer, which is initialized to zero or one in the constructor:

```c++
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
```

- In the function overloading the assignment operator and copy constructor:

```c++
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
```
I first decrement and check whether the current object has reached a reference count of zero. This means that the data from the current object is cleaned up before pointing to `s`.
Then the reference count is copied to point to the same memory of the object `s` and incremented. 
- In the destructor, the reference count is decremented and memory only freed if the count reaches zero.

I tested this with the same code from task one but extended the `print` function to print the count. This achieved the desired results as shown on the worksheet.

![alt text](./images/image-3.png)

## Task 3

To demonstrate the reference count reaching zero, I wrote the below code:

```c++
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
```

This creates a new `my_string` object on the heap. Then inside a new block scope, define `t` to point to `s` - this increases the reference count. At the end of the block scope `t` goes out of scope and the count is decremented. Then `s` is manually deleted, which triggers a print statement in the destructor:

```c++
my_string::~my_string()
{
    if (--(*ref_count) == 0) {
        std::cout << "Reference counter equals 0, freeing memory..." << std::endl;
        delete[] m_data;
        delete ref_count;
    }
}
```

The output of this program is:

![alt text](./images/image-6.png)

## Task 4

To implement the reference counter as it's own class, I created `reference_counter.hpp` and defined a template class:

```c++
template<class T>
class reference_counter {
    private:
        T* ref_ptr = nullptr;
        unsigned int* ref_count = nullptr;
```

- `T* ref_ptr` is used to point to the provided data of type `T`. This allows it to work for any type.
- `ref_count` functions the same as the implementation above.

This class implements the following constructor functions:

```c++
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
```

The copy contructor allows multiple `reference_counter` objects to point to the same resource. This points the above class variables to that of the `reference_counter` object being passed in and increments the count. 

I also overloaded the arrow operator (`->`) to return the underlying data the reference counter is pointing to:

```c++
T* operator->() const
        {
                return this->ref_ptr;
        }
```
Which allows you to call member functions if the pointer points to an object. 

Similarly, I overload the dereference pointer to return the dereferenced `ref_ptr` in order to access the underlying data:

```c++
        T& operator*() const
        {
                return (*this->ref_ptr);
        }
```

The destructor of this class functions the same as the string class in task 2. 

My test code for this can be found in `task4.cpp`. This goes through creating multiple reference count objects to point to the same resource and demonstrates the functionality as discussed above. The output is:

![alt text](./images/image-11.png)

Please note that the two print statements are from the reference counter objects going out of scope at the end of `main()`.

