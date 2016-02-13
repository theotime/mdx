/* 
 * File:   geoset.h
 * Author: theotime
 *
 * Created on 13 février 2016, 16:11
 */

#ifndef GEOSET_H
#define GEOSET_H

#include <fstream>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdint.h>

class Geoset {
public:
    size_t load(std::ifstream& fs);

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<uint16_t> faces;
private:
};

#endif /* GEOSET_H */

