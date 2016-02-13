/* 
 * File:   geosets.h
 * Author: theotime
 *
 * Created on 13 février 2016, 16:11
 */

#ifndef GEOSETS_H
#define GEOSETS_H

#include <fstream>

#include <map>

#include "geoset.h"

class Geosets {
public:
    size_t load(std::ifstream& fs);
    
    size_t numGeos();
    
    Geoset& operator[] (size_t index);
    
    uint32_t id;
    size_t size;
    
private:
    size_t ngeos;
    
    std::map<size_t, Geoset> geosets;
    
};

#endif /* GEOSETS_H */

