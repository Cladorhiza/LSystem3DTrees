#include "GLUtil.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "gtx/rotate_vector.hpp"

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

    unsigned buildVAOfromData(GraphicsTurtle::renderData& data) {

        std::vector<float> vertexes;
        std::vector<float> colours;

        for (const GraphicsTurtle::vertex& v : data.vertexes) {

            vertexes.push_back(v.position[0]);
            vertexes.push_back(v.position[1]);
            vertexes.push_back(v.position[2]);
            colours.push_back(v.colour[0]);
            colours.push_back(v.colour[1]);
            colours.push_back(v.colour[2]);
            colours.push_back(v.colour[3]);
        }
        return buildVAOfromData(vertexes, colours, data.indexes);
    }

    unsigned buildCircleVAO(const float position[3],const float normal[3], float radius, int resolution, const float colour[4]) {

        std::vector<float> vertexes;
        std::vector<float> colours;
        std::vector<unsigned> indexes;

        glm::vec3 originNormal(0.f, 1.f, 0.f);
        glm::vec3 originSideNormal(1.f, 0.f, 0.f);

        glm::vec3 circleNormal(glm::normalize(glm::vec3(normal[0], normal[1], normal[2])));
        glm::vec3 circleSideNormal(glm::cross(originNormal, circleNormal));

        if (glm::length(circleSideNormal) < 0.001f) 
            circleSideNormal = originSideNormal;
        circleSideNormal = glm::normalize(circleSideNormal);

        glm::vec3 radialPoint(circleSideNormal * radius);
        float step = (2.f * M_PI) / resolution;
        for (int i = 0; i < resolution; i++) {

            vertexes.push_back(radialPoint.x + position[0]);
            vertexes.push_back(radialPoint.y + position[1]);
            vertexes.push_back(radialPoint.z + position[2]);
            colours.push_back(colour[0]);
            colours.push_back(colour[1]);
            colours.push_back(colour[2]);
            colours.push_back(colour[3]);


            radialPoint = glm::rotate(radialPoint, step, circleNormal);
        }

        for (int i = 0; i < resolution-1; i++) {
            indexes.push_back(i);
            indexes.push_back(i + 1);
        }
        indexes.push_back(resolution - 1);
        indexes.push_back(0);

        return buildVAOfromData(vertexes, colours, indexes);
    }

    unsigned buildCylinderVAO(const float startPos[3], const float endPos[3], const float startNorm[3], const float endNorm[3], float radius1, float radius2, int resolution, const float colour[4])
    {



        std::vector<float> vertexes;
        std::vector<float> colours;
        std::vector<unsigned> indexes;

        glm::vec3 originNormal(0.f, 1.f, 0.f);
        glm::vec3 originSideNormal(1.f, 0.f, 0.f);

        glm::vec3 circleNormal(glm::normalize(glm::vec3(startNorm[0], startNorm[1], startNorm[2])));
        glm::vec3 circleSideNormal(glm::cross(originNormal, circleNormal));

        if (glm::length(circleSideNormal) < 0.001f)
            circleSideNormal = originSideNormal;
        circleSideNormal = glm::normalize(circleSideNormal);

        if (glm::dot(originNormal, circleNormal) < 0) {
            circleSideNormal = -circleSideNormal;
        }

        glm::vec3 radialPoint(circleSideNormal * radius1);
        float step = (2.f * M_PI) / resolution;
        for (int i = 0; i < resolution; i++) {

            vertexes.push_back(radialPoint.x + startPos[0]);
            vertexes.push_back(radialPoint.y + startPos[1]);
            vertexes.push_back(radialPoint.z + startPos[2]);
            colours.push_back(colour[0]);
            colours.push_back(colour[1]);
            colours.push_back(colour[2]);
            colours.push_back(colour[3]);


            radialPoint = glm::rotate(radialPoint, step, circleNormal);
        }
        circleNormal = glm::normalize(glm::vec3(endNorm[0], endNorm[1], endNorm[2]));
        circleSideNormal = glm::cross(originNormal, circleNormal);

        if (glm::length(circleSideNormal) < 0.001f)
            circleSideNormal = originSideNormal;
        circleSideNormal = glm::normalize(circleSideNormal);

        if (glm::dot(originNormal, circleNormal) < 0) {
            circleSideNormal = -circleSideNormal;
        }

        radialPoint = circleSideNormal * radius2;
        for (int i = 0; i < resolution; i++) {

            vertexes.push_back(radialPoint.x + endPos[0]);
            vertexes.push_back(radialPoint.y + endPos[1]);
            vertexes.push_back(radialPoint.z + endPos[2]);
            colours.push_back(colour[0]);
            colours.push_back(colour[1]);
            colours.push_back(colour[2]);
            colours.push_back(colour[3]);


            radialPoint = glm::rotate(radialPoint, step, circleNormal);
        }

        

        for (int i = 0; i < resolution - 1; i++) {
            //push triangles to build quad between two circle edges
            indexes.push_back(i);
            indexes.push_back(i + 1);
            indexes.push_back(resolution + i);
            indexes.push_back(resolution + i);
            indexes.push_back(i + 1);
            indexes.push_back(resolution + i + 1);
        }
        indexes.push_back(resolution - 1);
        indexes.push_back(0);
        indexes.push_back((resolution * 2) -1);
        indexes.push_back((resolution * 2) - 1);
        indexes.push_back(0);
        indexes.push_back(resolution);

        return buildVAOfromData(vertexes, colours, indexes);
    }

}