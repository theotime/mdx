/* 
 * File:   main.cpp
 * Author: theotime
 *
 * Created on 13 février 2016, 16:11
 */

#include <iostream>

#include "mdx.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cout << "Need filename" << std::endl;
        return false;
    }
    
    if (sizeof (float) != 4) {
        std::cerr << "Error non 32bits float" << std::endl;
        return false;
    }

    Mdx mdx;
    mdx.load(argv[1]);
    mdx.output_obj(ALL_GEOSETS);

    return true;
}

