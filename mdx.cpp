// specs : http://www.wc3c.net/tools/specs/NubMdxFormat.txt

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>

#include <stdint.h>

using namespace std;

typedef struct {
    uint32_t id;
    uint32_t size;
} Chunk;

typedef struct {
    uint32_t id;
    uint32_t size_i;
} Set;

typedef struct {
    float x;
    float y;
    float z;
} Vector3;

typedef struct {
    uint16_t a;
    uint16_t b;
    uint16_t c;
} Face;

void print_id(uint32_t id) {
    char id_str[5];
    strncpy(id_str, (char *) &id, 4);
    id_str[4] = '\0';
//    cout << id_str << endl;
}

int main(int argc, char *argv[]) {


    if (argc != 2) {
        cout << "Need filename" << endl;
        return false;
    }

    string filename = argv[1];

    ifstream fs;

    fs.open(filename.c_str());

    if (!fs.is_open()) {
        cerr << "Error opening file" << endl;
        return false;
    }

    if (sizeof (float) != 4) {
        cerr << "Error non 32bits float" << endl;
        return false;
    }

    uint32_t mdx;
    fs.read((char *) &mdx, sizeof (uint32_t));
    print_id(mdx);

    int i = 0;
    Chunk chunk;
    Set vrtx, nrms, ptyp, pcnt, pvtx, gndx, mtgc, mats;
    Vector3* vertex, * normals;
    uint32_t* face_types, * face_groups, *MatrixGroupSize, *MatrixIndex;
    Face* faces;
    uint8_t* matrix_groups;

    cout << "# Blizzard MDX" << endl;
    cout << "o Mdx" << endl;
    
    
    int face_base = 1;


    while (fs.read((char *) &chunk, sizeof (Chunk))) {
        print_id(chunk.id);
        //        cout << chunk.size << endl;
        int byte_read = 0;

        if (chunk.id == 0x534f4547) {//GEOS

            while (byte_read != chunk.size) {

                uint32_t inclusive_size;

                fs.read((char *) &inclusive_size, sizeof (inclusive_size));
                byte_read += sizeof (inclusive_size);


                ////////////////////////// VRTX


                fs.read((char *) &vrtx, sizeof (vrtx));
                byte_read += sizeof (vrtx);

                //            cout << "  ";
                print_id(vrtx.id);

                vertex = (Vector3 *) malloc(sizeof (Vector3) * vrtx.size_i);

                fs.read((char *) vertex, sizeof (Vector3) * vrtx.size_i);
                byte_read += sizeof (Vector3) * vrtx.size_i;

                for (i = 0; i < vrtx.size_i; i++) {
                    cout << "v ";

                    cout << vertex[i].x;
                    cout << " ";
                    cout << vertex[i].y;
                    cout << " ";
                    cout << vertex[i].z;

                    cout << endl;
                }


                free(vertex);


                ///////////////////// NRMS

                fs.read((char *) &nrms, sizeof (nrms));
                byte_read += sizeof (nrms);

                //            cout << "  ";
                print_id(nrms.id);

                normals = (Vector3 *) malloc(sizeof (Vector3) * nrms.size_i);

                fs.read((char *) normals, sizeof (Vector3) * nrms.size_i);
                byte_read += sizeof (Vector3) * nrms.size_i;
                free(normals);



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

                faces = (Face *) malloc(sizeof (Face) * (pvtx.size_i / 3));

                fs.read((char *) faces, sizeof (Face) *(pvtx.size_i / 3));
                byte_read += sizeof (Face) * (pvtx.size_i / 3);


                for (i = 0; i < (pvtx.size_i / 3); i++) {
                    cout << "f ";

                    cout << faces[i].a + face_base;
                    cout << " ";
                    cout << faces[i].b + face_base;
                    cout << " ";
                    cout << faces[i].c + face_base;

                    cout << endl;
                }
                
                face_base += vrtx.size_i;

                free(faces);


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

//                cout << size_extents << endl;

                fs.seekg((sizeof (uint32_t) * 7 * size_extents), ios_base::cur); //skip multiple stuff
                byte_read += (sizeof (uint32_t) * 7 * size_extents);


                Set uvas;
                fs.read((char *) &uvas, sizeof ( uvas));
                byte_read += sizeof (uvas);
                print_id(uvas.id);

                Set uvbs;
                fs.read((char *) &uvbs, sizeof ( uvbs));
                byte_read += sizeof (uvbs);
                print_id(uvbs.id);

                //            fs.seekg((sizeof(uint32_t) * 3), ios_base::cur);//skip multiple stuff
                //            
                //            uint32_t size_texture_pos;
                //            fs.read((char *) &size_texture_pos, sizeof(uint32_t));
                //            byte_read += sizeof(uint32_t);

                fs.seekg((sizeof (uint32_t) * uvbs.size_i * 2), ios_base::cur); //skip multiple stuff // FLOAT2
                byte_read += (sizeof (uint32_t) * uvbs.size_i * 2);

//                cout << chunk.size << ":" << byte_read << endl;
            }
        }


        fs.seekg(chunk.size - byte_read, ios_base::cur);
    }


    fs.close();

    //    cout << "OK" << endl;

    return true;

}
