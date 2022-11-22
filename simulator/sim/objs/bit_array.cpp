//
// Created by Mihai - PC on 11/15/2022.
//

#include <stdexcept>
#include <bitset>
#include <iostream>
#include <sstream>
#include "bit_array.h"

namespace sim {
    namespace objs {
        bit_array::bit_array(int hi, int lo, uint64_t init) {
            this->content = init;
            if(std::abs(hi - lo) > 64){
                throw std::invalid_argument("Indexes will generate an array bigger than supported (64 bits)");
            }
            if(hi>lo){
                hi_index = hi;
                lo_index = lo;
            } else {
                hi_index = lo;
                lo_index = hi;
            }
        }

        uint64_t bit_array::value_between(int hi, int lo) const {
            if(hi < lo)
                std::swap(hi,lo);
            if(hi >= this->hi_index){
                throw std::invalid_argument("High index higher than available data");
            } else if(lo < this->lo_index){
                throw std::invalid_argument("Low index lower than available data");
            }
            return content >> lo & ~(0xFFFFFFFFFFFFFFFF << (hi-lo + 1));
        }

        /// a.modify_value(i,j,b,k,l) here is syntactically the same as a[i:j] = b[k:l] in Verilog
        bit_array& bit_array::modify_value(int to_hi, int to_lo, bit_array& value, int from_hi, int from_lo) {
            if(from_hi < from_lo)
                std::swap(from_hi,from_lo);
            if(to_hi < to_lo)
                std::swap(to_hi,to_lo);
            if((from_hi - from_lo) != (to_hi - to_lo)) {
                std::stringstream err_str("");
                err_str << "Size mismatch between "
                        << '(' << to_hi << ',' << to_lo << ')'
                        << " and "
                        << '(' << from_hi << ',' << from_lo << ')';
                throw std::invalid_argument(err_str.str());
            }
            uint64_t new_val = value.value_between(from_hi, from_lo);
            new_val <<= to_lo;
            uint64_t mask = 0xFFFFFFFFFFFFFFFF >> to_lo << (63 - (to_hi - to_lo)) >> (63 - to_hi);
            std::bitset<64> x(mask);
            this->content = (this->content & ~mask) | (new_val & mask);
            this->on_value_change(BOTH);
            return *this;
        }

        bit_array& bit_array::modify_value(int index,bit& value) {
            uint64_t a = value.get_content() << index;
            uint64_t mask = 1 << index;
            content = (content & ~mask) | a;
            return *this;
        }

        bool bit_array::value_at(int index) const {
            if(index > hi_index || index < lo_index){
                std::stringstream err_str("");
                err_str << "Invalid index \'" << index << "\'";
                throw std::invalid_argument(err_str.str());
            }
            return 1 & (content >> index);
        }


    } // objs
}

