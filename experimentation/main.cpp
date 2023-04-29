

#include <iostream>
#include "contiguous_allocator.h"

class non_trivial{
    int* array;
    public:
    explicit non_trivial(int size){
        std::cout<< "constructing this mess\n";
        array = new int[size];
    }
    virtual ~non_trivial(){
        std::cout << "destructing this thing\n";
        delete array;
    }
};

class child : public non_trivial{
    char* bruh;
public:
    explicit child(int size, int str_l) : non_trivial(size){
        bruh = new char[str_l];
        std::cout << "making a child\n";
    }
    ~child(){
        std::cout << "destroying the child\n";
    }
};

class bruh{
    unsigned long long r[64];
};

int main(){
    std::cout << sizeof(bruh) << " " << alignof(bruh) << '\n';
    std::cout << "my allocator:\n\n";
    auto& allocator = contiguous_allocator::get_instance();
    auto* pointer = allocator.create<child>(10,48);
    non_trivial* test = pointer;
    allocator.destroy(test);

    std::cout << "\nregular allocator:\n\n";
    auto* p = new child(10,48);
    non_trivial* regular = p;
    delete p;
}
