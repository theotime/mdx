/* 
 * File:   geoset.cpp
 * Author: theotime
 * 
 * Created on 13 février 2016, 16:11
 */

#include "mdx.h"
#include "geoset.h"

size_t Geoset::load(std::ifstream& fs) {
    Hdr_Chunk vrtx, nrms, ptyp, pcnt, pvtx, gndx, mtgc, mats, uvas, uvbs;
    
    int i = 0;
    size_t byte_read = 0;

    uint32_t inclusive_size;
    fs.read((char *) &inclusive_size, sizeof (inclusive_size));
    byte_read += sizeof (inclusive_size);


    ////////////////////////// VRTX


    fs.read((char *) &vrtx, sizeof (vrtx));
    byte_read += sizeof (vrtx);

    //            cout << "  ";
    print_id(vrtx.id);

    float x, y, z;
    glm::vec3 vertex;
    for (i = 0; i < vrtx.size_i; i++) {
        fs.read((char*) &x, sizeof (float));
        vertex.x = x;
        fs.read((char*) &y, sizeof (float));
        vertex.y = y;
        fs.read((char*) &z, sizeof (float));
        vertex.z = z;
        vertices.push_back(vertex);
    }
    byte_read += sizeof (float) * 3 * vrtx.size_i;


    ///////////////////// NRMS

    fs.read((char *) &nrms, sizeof (nrms));
    byte_read += sizeof (nrms);

    //            cout << "  ";
    print_id(nrms.id);

    glm::vec3 normal;
    for (i = 0; i < vrtx.size_i; i++) {
        fs.read((char*) &x, sizeof (float));
        normal.x = x;
        fs.read((char*) &y, sizeof (float));
        normal.y = y;
        fs.read((char*) &z, sizeof (float));
        normal.z = z;
        normals.push_back(normal);
    }
    byte_read += sizeof (float) * 3 * vrtx.size_i;



    ///////////////////// PTYP

    fs.read((char *) &ptyp, sizeof ( ptyp));
    byte_read += sizeof (ptyp);

    //            cout << "  ";
    print_id(ptyp.id);

    fs.seekg(sizeof (uint32_t) * ptyp.size_i, std::ios_base::cur); // Skip
    byte_read += sizeof (uint32_t) * ptyp.size_i;

    ///////////////////// PCNT

    fs.read((char *) &pcnt, sizeof ( pcnt));
    byte_read += sizeof (pcnt);

    //            cout << "  ";
    print_id(pcnt.id);

    fs.seekg(sizeof (uint32_t) * pcnt.size_i, std::ios_base::cur); // Skip
    byte_read += sizeof (uint32_t) * pcnt.size_i;


    ///////////////////// PVTX

    fs.read((char *) &pvtx, sizeof ( pvtx));
    byte_read += sizeof (pvtx);

    //            cout << "  ";
    print_id(pvtx.id);

    uint16_t face;
    for (i = 0; i < pvtx.size_i; i++) {
        fs.read((char*) &face, sizeof (uint16_t));
        faces.push_back(face);
    }
    byte_read += sizeof (uint16_t) * pvtx.size_i;


    ///////////////////// GNDX

    fs.read((char *) &gndx, sizeof ( gndx));
    byte_read += sizeof (gndx);

    //            cout << "  ";
    print_id(gndx.id);

    fs.seekg(sizeof (uint8_t) * gndx.size_i, std::ios_base::cur); // Skip
    byte_read += sizeof (uint8_t) * gndx.size_i;

    ///////////////////// MTGC

    fs.read((char *) &mtgc, sizeof ( mtgc));
    byte_read += sizeof (mtgc);

    //            cout << "  ";
    print_id(mtgc.id);

    fs.seekg(sizeof (uint32_t) * mtgc.size_i, std::ios_base::cur); // Skip
    byte_read += sizeof (uint32_t) * mtgc.size_i;

    ///////////////////// MATS

    fs.read((char *) &mats, sizeof ( mats));
    byte_read += sizeof (mats);

    //            cout << "  ";
    print_id(mats.id);

    fs.seekg(sizeof (uint32_t) * mats.size_i, std::ios_base::cur); // Skip
    byte_read += sizeof (uint32_t) * mats.size_i;

    fs.seekg((sizeof (uint32_t) * 10), std::ios_base::cur); //skip multiple stuff
    byte_read += (sizeof (uint32_t) * 10);

    uint32_t size_extents;
    fs.read((char *) &size_extents, sizeof (uint32_t));
    byte_read += sizeof (uint32_t);

    fs.seekg((sizeof (uint32_t) * 7 * size_extents), std::ios_base::cur); //skip multiple stuff
    byte_read += (sizeof (uint32_t) * 7 * size_extents);


    fs.read((char *) &uvas, sizeof ( uvas));
    byte_read += sizeof (uvas);
    print_id(uvas.id);

    fs.read((char *) &uvbs, sizeof ( uvbs));
    byte_read += sizeof (uvbs);
    print_id(uvbs.id);

    fs.seekg((sizeof (uint32_t) * uvbs.size_i * 2), std::ios_base::cur); //skip multiple stuff // FLOAT2
    byte_read += (sizeof (uint32_t) * uvbs.size_i * 2);


    return byte_read;
}
