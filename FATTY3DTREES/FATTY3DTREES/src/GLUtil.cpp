#include "GLUtil.h"

namespace GLUtil {

    unsigned buildVAOfromData(const std::vector<float>& vertexes, 
                                const std::vector<float>& colours, 
                                const std::vector<unsigned>& indexes) {
        //ids for buffer objects
        unsigned vbo[2], vao, ibo;

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        //generate vertex buffers and assign vertexes to first
        glGenBuffers(2, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(float), vertexes.data(), GL_STATIC_DRAW);

        //shader layout location for vertexes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
        glEnableVertexAttribArray(0);

        //assign colours to second
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, colours.size() * sizeof(float), colours.data(), GL_STATIC_DRAW);

        //shader layout location for colours
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glEnableVertexAttribArray(1);

        //generate index buffer and assign indexes to it
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(unsigned), indexes.data(), GL_STATIC_DRAW);

        return vao;
    }

}