/* 
 * File:   mdx.h
 * Author: theotime
 *
 * Created on 13 février 2016, 16:12
 */

#ifndef MDX_H
#define MDX_H

#include <GL/glew.h>

#include <string>
#include <vector>

#include <stdint.h>

#include "geosets.h"

#define ALL_GEOSETS -1

#define MDLX 0x584c444d
#define GEOS 0x534f4547

typedef struct {
    uint32_t id;
    uint32_t size_i;
} Hdr_Chunk;


class Mdx {
public:
    bool load(const std::string& filename);
    
    void output_obj(int selected_geoset);
    void output_gl(int selected_geoset, std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<GLushort> &elements);
    
private:
    std::string filename;
    Geosets geosets;
};


void print_id(uint32_t id);

#endif /* MDX_H */

