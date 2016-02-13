// specs : http://www.wc3c.net/tools/specs/NubMdxFormat.txt

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <map>

#include <stdint.h>
#include <assert.h>

using namespace std;
using namespace glm;

#define ALL_GEOSETS -1

#define MDLX 0x584c444d
#define GEOS 0x534f4547

typedef struct {
    uint32_t id;
    uint32_t size_i;
} Header;

void print_id(uint32_t id) {
    char id_str[5];
    strncpy(id_str, (char *) &id, 4);
    id_str[4] = '\0';
//    cout << id_str << endl;
}

class Geoset {
public:
    size_t load(ifstream& fs);

    vector<vec3> vertices;
    vector<vec3> normals;
    vector<uint16_t> faces;
private:
};

size_t Geoset::load(ifstream& fs) {
    Header vrtx, nrms, ptyp, pcnt, pvtx, gndx, mtgc, mats, uvas, uvbs;
    uint32_t* face_types, * face_groups, *MatrixGroupSize, *MatrixIndex;
    uint8_t* matrix_groups;

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
    vec3 vertex;
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

    vec3 normal;
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

    face_types = (uint32_t *) malloc(sizeof (uint32_t) * ptyp.size_i);

    fs.read((char *) face_types, sizeof (uint32_t) * ptyp.size_i);
    byte_read += sizeof (uint32_t) * ptyp.size_i;

    free(face_types);


    ///////////////////// PCNT

    fs.read((char *) &pcnt, sizeof ( pcnt));
    byte_read += sizeof (pcnt);

    //            cout << "  ";
    print_id(pcnt.id);

    face_groups = (uint32_t *) malloc(sizeof (uint32_t) * pcnt.size_i);

    fs.read((char *) face_groups, sizeof (uint32_t) * pcnt.size_i);
    byte_read += sizeof (uint32_t) * pcnt.size_i;

    free(face_groups);


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

    matrix_groups = (uint8_t *) malloc(sizeof (uint8_t) * gndx.size_i);

    fs.read((char *) matrix_groups, sizeof (uint8_t) * gndx.size_i);
    byte_read += sizeof (uint8_t) * gndx.size_i;

    free(matrix_groups);

    ///////////////////// MTGC

    fs.read((char *) &mtgc, sizeof ( mtgc));
    byte_read += sizeof (mtgc);

    //            cout << "  ";
    print_id(mtgc.id);

    MatrixGroupSize = (uint32_t *) malloc(sizeof (uint32_t) * mtgc.size_i);

    fs.read((char *) MatrixGroupSize, sizeof (uint32_t) * mtgc.size_i);
    byte_read += sizeof (uint32_t) * mtgc.size_i;
    free(MatrixGroupSize);

    ///////////////////// MATS

    fs.read((char *) &mats, sizeof ( mats));
    byte_read += sizeof (mats);

    //            cout << "  ";
    print_id(mats.id);

    MatrixIndex = (uint32_t *) malloc(sizeof (uint32_t) * mats.size_i);

    fs.read((char *) MatrixIndex, sizeof (uint32_t) * mats.size_i);
    byte_read += sizeof (uint32_t) * mats.size_i;

    free(MatrixIndex);

    fs.seekg((sizeof (uint32_t) * 10), ios_base::cur); //skip multiple stuff
    byte_read += (sizeof (uint32_t) * 10);

    uint32_t size_extents;
    fs.read((char *) &size_extents, sizeof (uint32_t));
    byte_read += sizeof (uint32_t);

    fs.seekg((sizeof (uint32_t) * 7 * size_extents), ios_base::cur); //skip multiple stuff
    byte_read += (sizeof (uint32_t) * 7 * size_extents);


    fs.read((char *) &uvas, sizeof ( uvas));
    byte_read += sizeof (uvas);
    print_id(uvas.id);

    fs.read((char *) &uvbs, sizeof ( uvbs));
    byte_read += sizeof (uvbs);
    print_id(uvbs.id);

    fs.seekg((sizeof (uint32_t) * uvbs.size_i * 2), ios_base::cur); //skip multiple stuff // FLOAT2
    byte_read += (sizeof (uint32_t) * uvbs.size_i * 2);


    return byte_read;
}

class Geosets {
public:
    size_t load(ifstream& fs);
    
    size_t numGeos();
    
    Geoset& operator[] (size_t index);
    
    uint32_t id;
    size_t size;
    
private:
    size_t ngeos;
    
    map<size_t, Geoset> geosets;
    
};

size_t Geosets::load(ifstream& fs) {
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

class Mdx {
public:
    bool load(const string& filename);
    
    void output_obj(int selected_geoset);
    
private:
    string filename;
    Geosets geosets;
};

bool Mdx::load(const string& filename) {
    ifstream fs;

    fs.open(filename.c_str());

    if (!fs.is_open()) {
        cerr << "Error opening file" << endl;
        return false;
    }

    uint32_t mdx;
    fs.read((char *) &mdx, sizeof (uint32_t));
    if (mdx != MDLX) {
        cerr << "Not a MDX file" << endl;
        return false;
    }
    print_id(mdx);

    Header chunk;
    while (fs.read((char *) &chunk, sizeof (Header))) {
        print_id(chunk.id);
        size_t byte_read = 0;

        if (chunk.id == GEOS) {
            geosets.id = chunk.id;
            geosets.size = chunk.size_i;
            byte_read = geosets.load(fs);
        }

        fs.seekg(chunk.size_i - byte_read, ios_base::cur);
    }
    

    fs.close();
}

void Mdx::output_obj(int selected_geoset) {
    cout << "# Blizzard MDX" << endl;
    cout << "o Mdx" << endl;

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
        for (vector<vec3>::iterator it = geosets[i].vertices.begin(); it != geosets[i].vertices.end(); it++) {
            cout << "v " << it->x << " " << it->y << " " << it->z << " " << endl;
        }

        int c = 0;
        for (vector<uint16_t>::iterator it = geosets[i].faces.begin(); it != geosets[i].faces.end(); it++) {
            if ((c % 3) == 0) {
                cout << "f ";
            }

            cout << *it + face_base;
            cout << " ";

            c++;
            if ((c % 3) == 0) {
                cout << endl;
            }

        }
        
        if (selected_geoset == ALL_GEOSETS) {
            face_base += geosets[i].vertices.size();
        }
    }
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        cout << "Need filename" << endl;
        return false;
    }
    
    if (sizeof (float) != 4) {
        cerr << "Error non 32bits float" << endl;
        return false;
    }

    Mdx mdx;
    mdx.load(argv[1]);
    mdx.output_obj(2);

    return true;
}
