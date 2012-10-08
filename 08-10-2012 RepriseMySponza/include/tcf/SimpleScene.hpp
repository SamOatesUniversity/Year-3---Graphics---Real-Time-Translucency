/**
 * SimpleScene.hpp
 *
 * A simple scene of multiple geometries stored via TCF.
 *
 * Copyright (c) 2011 Tyrone Davison.
 */

#pragma once
#ifndef __TCF_SIMPLESCENE__
#define __TCF_SIMPLESCENE__

#include "types.hpp"
#include <vector>

namespace tcf {

    struct SimpleScene {
        enum Topology {
            kIndexedTriangleList = 1
        };

        struct Mesh {
            Topology topology;
            std::vector<Vector3> vertexArray;
            std::vector<Vector3> normalArray;
            std::vector<Vector3> tangentArray;
            std::vector<Vector2> texcoordArray;
            std::vector<int> indexArray;
            std::vector<Matrix> instanceArray;
        };

        std::vector<Mesh> meshArray;
    };

    bool readFile(const char *path, SimpleScene *out);
    bool writeFile(const char *path, const SimpleScene *in);

}

#endif
