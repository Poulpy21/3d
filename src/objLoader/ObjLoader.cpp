#include <iostream>
#include <cassert>

#include "tiny_obj_loader.h"
#include "ObjLoader.h"

ObjLoader::ObjLoader (std::string const& file, std::string const& basepath) : objFilename(file + ".obj"), mtlFilename(file + ".mtl") {
    std::string err = tinyobj::LoadObj(shapes, objFilename.c_str(), basepath.c_str());
    if (!err.empty()) {
        std::cout << "Error during loading of "  << basepath << objFilename << std::endl;
        std::cerr << err << std::endl;
        exit(EXIT_FAILURE);
    }

    print();
}

void ObjLoader::print () {
    std::cout << "# of shapes : " << shapes.size() << std::endl;

    for (size_t i = 0; i < shapes.size(); i++) {
        printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
        printf("shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());
        assert((shapes[i].mesh.indices.size() % 3) == 0);
        for (size_t f = 0; f < shapes[i].mesh.indices.size(); f++) {
            printf("  idx[%ld] = %d\n", f, shapes[i].mesh.indices[f]);
        }

        printf("shape[%ld].vertices: %ld\n", i, shapes[i].mesh.positions.size());
        assert((shapes[i].mesh.positions.size() % 3) == 0);
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            printf("  v[%ld] = (%f, %f, %f)\n", v,
                   shapes[i].mesh.positions[3*v+0],
                   shapes[i].mesh.positions[3*v+1],
                   shapes[i].mesh.positions[3*v+2]);
        }

        printf("shape[%ld].normals: %ld\n", i, shapes[i].mesh.normals.size());
        assert((shapes[i].mesh.normals.size() % 3) == 0);
        for (size_t v = 0; v < shapes[i].mesh.normals.size() / 3; v++) {
            printf("  v[%ld] = (%f, %f, %f)\n", v,
                   shapes[i].mesh.normals[3*v+0],
                   shapes[i].mesh.normals[3*v+1],
                   shapes[i].mesh.normals[3*v+2]);
        }

        /* printf("shape[%ld].material.name = %s\n", i, shapes[i].material.name.c_str()); */
        /* printf("  material.Ka = (%f, %f ,%f)\n", shapes[i].material.ambient[0], shapes[i].material.ambient[1], shapes[i].material.ambient[2]); */
        /* printf("  material.Kd = (%f, %f ,%f)\n", shapes[i].material.diffuse[0], shapes[i].material.diffuse[1], shapes[i].material.diffuse[2]); */
        /* printf("  material.Ks = (%f, %f ,%f)\n", shapes[i].material.specular[0], shapes[i].material.specular[1], shapes[i].material.specular[2]); */
        /* printf("  material.Tr = (%f, %f ,%f)\n", shapes[i].material.transmittance[0], shapes[i].material.transmittance[1], shapes[i].material.transmittance[2]); */
        /* printf("  material.Ke = (%f, %f ,%f)\n", shapes[i].material.emission[0], shapes[i].material.emission[1], shapes[i].material.emission[2]); */
        /* printf("  material.Ns = %f\n", shapes[i].material.shininess); */
        /* printf("  material.Ni = %f\n", shapes[i].material.ior); */
        /* printf("  material.dissolve = %f\n", shapes[i].material.dissolve); */
        /* printf("  material.illum = %d\n", shapes[i].material.illum); */
        /* printf("  material.map_Ka = %s\n", shapes[i].material.ambient_texname.c_str()); */
        /* printf("  material.map_Kd = %s\n", shapes[i].material.diffuse_texname.c_str()); */
        /* printf("  material.map_Ks = %s\n", shapes[i].material.specular_texname.c_str()); */
        /* printf("  material.map_Ns = %s\n", shapes[i].material.normal_texname.c_str()); */
        /* std::map<std::string, std::string>::iterator it(shapes[i].material.unknown_parameter.begin()); */
        /* std::map<std::string, std::string>::iterator itEnd(shapes[i].material.unknown_parameter.end()); */
        /* for (; it != itEnd; it++) { */
        /*     printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str()); */
        /* } */
        printf("\n");
    }
}

void ObjLoader::draw () {
    // TODO : use vbos and add normals
    glBegin(GL_TRIANGLES);

    for (size_t i = 0; i < shapes.size(); i++) {
        for (size_t f = 0; f < shapes[i].mesh.indices.size(); f++) {
            float ind = shapes[i].mesh.indices[f];
            float x = shapes[i].mesh.positions[3 * ind + 0];
            float y = shapes[i].mesh.positions[3 * ind + 1];
            float z = shapes[i].mesh.positions[3 * ind + 2];

            if (shapes[i].mesh.normals.size() == shapes[i].mesh.positions.size()) {
                float xn = shapes[i].mesh.normals[3 * ind + 0];
                float yn = shapes[i].mesh.normals[3 * ind + 1];
                float zn = shapes[i].mesh.normals[3 * ind + 2];

                glNormal3f(xn, yn, zn);
            }
            glVertex3f(x, y, z);
        }
    }

    glEnd();
}

