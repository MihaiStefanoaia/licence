//
// Created by mihai-laptop on 4/28/23.
//

#ifndef UNTITLED_CONTIGUOUS_ALLOCATOR_H
#define UNTITLED_CONTIGUOUS_ALLOCATOR_H

#include <cstdlib>
#include <new>
#include <list>
#include <map>
#include <iostream>


class contiguous_allocator{
    class memory_page{
        void* data;
        explicit memory_page(int size = 1){
            data = aligned_alloc(4096, 4096 * size);
        }
        ~memory_page(){
            free(data);
        }
        friend class contiguous_allocator;
    };

    struct allocation{
        int begin;
        int end;
    };

    static contiguous_allocator* instance;
    std::map<memory_page*, std::list<allocation>> allocs = {};


    void* get_compatible_pointer(int size){
        if(size == 0)
            exit(1);
        //tbd, needs actual allocation. for now uses an entire page for each allocation
        auto* tmp = new memory_page((size/4096) + (size % 4096 != 0));
        allocs[tmp].push_back({0,size-1});
        return  (char*)tmp->data + 0;
    }

    contiguous_allocator() = default;

public:
    static contiguous_allocator& get_instance(){
        if(instance == nullptr)
            instance = new contiguous_allocator();
        return *instance;
    }
    template<typename T, class... Args>
    T* create(Args&& ... args){
        auto* tmp = new (get_compatible_pointer(sizeof(T))) T(std::forward<Args>(args)...);
        std::cout << "returning address " << (unsigned long)tmp << '\n';
        return tmp;
    }

    template<typename T>
    void destroy(T* loc){
        for(auto& kvp : allocs){
            for(auto& page_allocs : kvp.second ){
                std::cout << "trying to find address " << (unsigned long)loc << " in page " << (unsigned long)kvp.first->data<< "\n";
                if((void*)loc == (void*)((char*)kvp.first->data + page_allocs.begin)){
                    loc->~T();
                    return;
                }
            }
        }
        exit(-1);
    }

};


#endif //UNTITLED_CONTIGUOUS_ALLOCATOR_H
