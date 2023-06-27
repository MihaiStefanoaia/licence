

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

#include <bitset>

int main(){

    auto val = 0b01101010;
    for(int amount = 0; amount < 8; amount ++){
        auto hi = val << amount;
        auto lo = val >> (8 - amount);
        auto out = hi | lo;
        std::cout << std::bitset<8>(out) << "\n";
    }

}
