/* 
 * File:   mdx.cpp
 * Author: theotime
 *
 * Created on 13 février 2016, 16:11
 */

// specs : http://www.wc3c.net/tools/specs/NubMdxFormat.txt

#include <iostream>

#include "mdx.h"
#include "geosets.h"
#include "geoset.h"


void print_id(uint32_t id) {
    char id_str[5];
    strncpy(id_str, (char *) &id, 4);
    id_str[4] = '\0';
    std::cout << id_str << std::endl;
}

bool Mdx::load(const std::string& filename) {
    std::ifstream fs;

    fs.open(filename.c_str());

    if (!fs.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return false;
    }

    uint32_t mdx;
    fs.read((char *) &mdx, sizeof (uint32_t));
    if (mdx != MDLX) {
        std::cerr << "Not a MDX file" << std::endl;
        return false;
    }
    print_id(mdx);

    Hdr_Chunk chunk;
    while (fs.read((char *) &chunk, sizeof (Hdr_Chunk))) {
        print_id(chunk.id);
        size_t byte_read = 0;

        if (chunk.id == GEOS) {
            geosets.id = chunk.id;
            geosets.size = chunk.size_i;
            byte_read = geosets.load(fs);
        }

        fs.seekg(chunk.size_i - byte_read, std::ios_base::cur);
    }
    

    fs.close();
    
    return true;
}

void Mdx::output_obj(int selected_geoset) {
    std::cout << "# Blizzard MDX" << std::endl;
    std::cout << "o Mdx" << std::endl;

    int face_base = 1;

    int i, total_geos;
    if (selected_geoset == ALL_GEOSETS) {
        i = 0;
        total_geos = geosets.numGeos();
    } else {
        i = selected_geoset;
        total_geos = selected_geoset + 1;
    }

    for (; i < total_geos; i++) {
        for (std::vector<glm::vec3>::iterator it = geosets[i].vertices.begin(); it != geosets[i].vertices.end(); it++) {
           std::cout << "v " << it->x << " " << it->y << " " << it->z << " " << std::endl;
        }

        int c = 0;
        for (std::vector<uint16_t>::iterator it = geosets[i].faces.begin(); it != geosets[i].faces.end(); it++) {
            if ((c % 3) == 0) {
                std::cout << "f ";
            }

            std::cout << *it + face_base;
            std::cout << " ";

            c++;
            if ((c % 3) == 0) {
                std::cout << std::endl;
            }
        }
        
        if (selected_geoset == ALL_GEOSETS) {
            face_base += geosets[i].vertices.size();
        }
    }
}

void Mdx::output_gl(int selected_geoset, std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<GLushort> &elements) {
    vertices = geosets[selected_geoset].vertices;
    elements = geosets[selected_geoset].faces;
    normals = geosets[selected_geoset].normals;
}