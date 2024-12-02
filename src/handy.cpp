#include "handy.hpp"

#include <iostream>

// Returns the intersection of two (possibly unsorted) vectors
//
//
std::vector<std::string> vector_intersection(std::vector<std::string> v1, std::vector<std::string> v2) {
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::vector<std::string> v_intersection;
    std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v_intersection));
    
    return v_intersection;
}

std::vector<const char*> vector_intersection(std::vector<const char*> v1, std::vector<const char*> v2) {
    auto char_ordering = [](const char* a, const char* b) { return std::strcmp(a, b) < 0; };
    std::sort(v1.begin(), v1.end(), char_ordering);
    std::sort(v2.begin(), v2.end(), char_ordering);

    std::vector<const char*> v_intersection;
    auto char_equality = [](const char* a, const char* b) { return std::strcmp(a, b) == 0; };
    std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v_intersection), char_equality);
    
    return v_intersection;
}


