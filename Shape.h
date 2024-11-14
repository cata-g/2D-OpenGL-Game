#pragma once
#include "dependente\glew\glew.h"
#include "dependente/glm/glm.hpp"

class Shape {
public:
    Shape() {};
    ~Shape();
    void draw() const;
    void setPosition(float x, float y);
    const void deleteBuffers() const ;
    const void setData(const GLfloat* vertices, const GLfloat* indices, size_t vertexCount, size_t indexCount);
    const void Generate();

    // Getter functions
    const GLfloat* getVertices() const { return vertices; }
    const GLfloat* getIndices() const { return indices; }
    glm::vec2 getPosition() const { return glm::vec2(posX, posY); }


private:
    GLuint vertexBuffer;
    GLuint elementBuffer;
    const GLfloat* vertices;
    const GLfloat* indices;
    size_t vertexCount;
    size_t indexCount;
    float posX, posY;
};
