#pragma once

#include <cstddef>
#include <vector>
#include <assert.h>

//namespace pttoth{

template<typename T>
int indexOfInVector(const std::vector<T>& vec, T& element){
    for(size_t i=0; i<vec.size(); ++i){
        if(vec[i] == element){
            assert(i < INT_MAX);
            return i;
        }
    }
    return -1;
}

template<typename T>
inline bool containedInVector(std::vector<T>& vec, T& element){
    return ( -1 < indexOfInVector(vec, element) );
}

template<typename T>
inline void removeElementInVector(std::vector<T>& vec, size_t pos){
    auto iter = vec.begin()+pos;
    vec.erase(iter);
}

//} //end of namespace

