#pragma once
// Include GLEW
#include "dependente\glew\glew.h"

class ShapeData {

private:
    // Body vertices
    const GLfloat body_points[12] = {
        -0.1f, -0.1f, 0.0f, // Bottom left
         0.1f, -0.1f, 0.0f, // Bottom right
         0.1f,  0.1f, 0.0f, // Top right
        -0.1f,  0.1f, 0.0f  // Top left
    };

    // Square indices
    const GLfloat body_indices[6] = {
        0, 1, 2, // First triangle
        0, 2, 3  // Second triangle
    };

    // Trunk vertices 
    const GLfloat trunk_points[12] = {
        -0.03f, 0.1f, 0.0f, // Bottom left
         0.03f, 0.1f, 0.0f, // Bottom right
         0.03f, 0.2f, 0.0f, // Top right (changed from 0.3f to 0.2f)
        -0.03f, 0.2f, 0.0f  // Top left (changed from 0.3f to 0.2f)
    };

    // Trunk indices
    const GLfloat trunk_indices[6] = {
        0, 1, 2, // First triangle
        0, 2, 3  // Second triangle
    };

    // Ear vertices 
    const GLfloat left_ear_points[12] = {
        -0.1f, 0.1f, 0.0f, // Bottom left
        -0.05f, 0.1f, 0.0f, // Bottom right
        -0.05f, 0.2f, 0.0f, // Top right
        -0.1f, 0.2f, 0.0f   // Top left
    };

    // Right Ear vertices
    const GLfloat right_ear_points[12] = {
        0.1f, 0.1f, 0.0f,  // Bottom left
        0.05f, 0.1f, 0.0f, // Bottom right
        0.05f, 0.2f, 0.0f, // Top right
        0.1f, 0.2f, 0.0f   // Top left
    };

    // Ear indices
    const GLfloat ear_indices[6] = {
        0, 1, 2, // First triangle
        0, 2, 3  // Second triangle
    };

    // Tail vertices
    const GLfloat tail_points[12] = {
        -0.02f, -0.1f, 0.0f, // Bottom left
         0.02f, -0.1f, 0.0f, // Bottom right
         0.02f, -0.05f, 0.0f, // Top right
        -0.02f, -0.05f, 0.0f  // Top left
    };

    // Tail indices
    const GLfloat tail_indices[6] = {
        0, 1, 2, // First triangle
        0, 2, 3  // Second triangle
    };
public:
    const ShapeData() {};
    const GLfloat* getSquarePoints() { return body_points; }
    const GLfloat* getSquareIndices() { return body_indices; }
    const GLfloat* getTrunkPoints() { return trunk_points; }
    const GLfloat* getTrunkIndices() { return trunk_indices; }
    const GLfloat* getLeftEarPoints() { return left_ear_points; }
    const GLfloat* getEarIndices() { return ear_indices; }
    const GLfloat* getRightEarPoints() { return right_ear_points; }
    const GLfloat* getTailPoints() { return tail_points; }
    const GLfloat* getTailIndices() { return tail_indices; }
};