/* 
 * File:   geosets.cpp
 * Author: theotime
 * 
 * Created on 13 février 2016, 16:11
 */

#include "geosets.h"

size_t Geosets::load(std::ifstream& fs) {
    int index_geoset = 0;
    size_t byte_read = 0;
    while (byte_read != size) {
        Geoset geoset;
        byte_read += geoset.load(fs);
        geosets[index_geoset] = geoset;
        index_geoset++;
    }

    ngeos = index_geoset;

    return byte_read;
}

size_t Geosets::numGeos() {
    return ngeos;
}

Geoset& Geosets::operator[] (size_t index) {
    assert(index < ngeos);
    assert(index >= 0);

    return geosets[index];
}
