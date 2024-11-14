#include "Shape.h"
Shape::~Shape()
{
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &elementBuffer);
}

void Shape::draw() const
{
    glPushMatrix();
    glTranslatef(posX, posY, 0.0f);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, (void*)0);
    glDisableVertexAttribArray(0);
    glPopMatrix();
}

void Shape::setPosition(float x, float y) {
    posX = x;
    posY = y;
}

const void Shape::deleteBuffers() const
{
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &elementBuffer);
}

const void Shape::setData(const GLfloat*_vertices, const GLfloat* _indices, size_t _vertexCount, size_t _indexCount)
{
    vertices = _vertices;
    indices = _indices;
    vertexCount = _vertexCount;
    indexCount = indexCount;
}

const void Shape::Generate()
{
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat) * 3, vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLfloat), indices, GL_STATIC_DRAW);
}
