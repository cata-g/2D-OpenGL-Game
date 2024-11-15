#pragma once
#include "dependente/glew/glew.h"

static class ShapeDetails {
private:
    const static int verticesLength = 12;
    const static int indicesLength = 6;
public:
    const static GLfloat body_points[verticesLength];
    const static GLushort body_indices[indicesLength];
    const static GLfloat trunk_points[verticesLength];
    const static GLushort trunk_indices[indicesLength];
    const static GLfloat left_ear_points[verticesLength];
    const static GLfloat right_ear_points[verticesLength];
    const static GLushort ear_indices[indicesLength];
    const static GLfloat tail_points[verticesLength];
    const static GLushort tail_indices[indicesLength];
    const static GLfloat ShapeDetails::diamond_points[verticesLength];
    const static GLushort ShapeDetails::diamond_indices[indicesLength];
    const static GLfloat ShapeDetails::opponent_points[verticesLength];
    const static GLushort ShapeDetails::opponent_indices[indicesLength];    
    const static GLfloat ShapeDetails::bullet_points[verticesLength];
    const static GLushort ShapeDetails::bullet_indices[indicesLength];
};
// Body vertices
const GLfloat ShapeDetails::body_points[verticesLength] = {
    -0.1f, -0.1f, 0.0f, // Bottom left
    0.1f, -0.1f, 0.0f, // Bottom right
    0.1f,  0.1f, 0.0f, // Top right
    -0.1f,  0.1f, 0.0f  // Top left
};

// Square indices
const GLushort ShapeDetails::body_indices[indicesLength] = {
    0, 1, 2, // First triangle
    0, 2, 3  // Second triangle
};

// Trunk vertices 
const GLfloat ShapeDetails::trunk_points[verticesLength] = {
    -0.03f, 0.1f, 0.0f, // Bottom left
     0.03f, 0.1f, 0.0f, // Bottom right
     0.03f, 0.2f, 0.0f, // Top right (changed from 0.3f to 0.2f)
    -0.03f, 0.2f, 0.0f  // Top left (changed from 0.3f to 0.2f)
};

// Trunk indices
const GLushort ShapeDetails::trunk_indices[indicesLength] = {
    0, 1, 2, // First triangle
    0, 2, 3  // Second triangle
};

// Ear vertices 
const GLfloat ShapeDetails::left_ear_points[verticesLength] = {
    -0.1f, 0.1f, 0.0f, // Bottom left
    -0.05f, 0.1f, 0.0f, // Bottom right
    -0.05f, 0.2f, 0.0f, // Top right
    -0.1f, 0.2f, 0.0f   // Top left
};

// Right Ear vertices
const GLfloat ShapeDetails::right_ear_points[verticesLength] = {
    0.1f, 0.1f, 0.0f,  // Bottom left
    0.05f, 0.1f, 0.0f, // Bottom right
    0.05f, 0.2f, 0.0f, // Top right
    0.1f, 0.2f, 0.0f   // Top left
};

// Ear indices
const GLushort ShapeDetails::ear_indices[indicesLength] = {
    0, 1, 2, // First triangle
    0, 2, 3  // Second triangle
};

// Tail vertices
const GLfloat ShapeDetails::tail_points[verticesLength] = {
    -0.02f, -0.1f, 0.0f, // Bottom left
     0.02f, -0.1f, 0.0f, // Bottom right
     0.02f, -0.05f, 0.0f, // Top right
    -0.02f, -0.05f, 0.0f  // Top left
};

// Tail indices
const GLushort ShapeDetails::tail_indices[indicesLength] = {
    0, 1, 2, // First triangle
    0, 2, 3  // Second triangle
};

// Diamond
const GLfloat ShapeDetails::diamond_points[verticesLength] = {
    0.0f,  0.025f, 0.0f,  // Top vertex
   -0.025f, 0.0f, 0.0f,   // Left vertex
    0.0f, -0.025f, 0.0f,  // Bottom vertex
    0.025f, 0.0f, 0.0f    // Right vertex
};

// Diamond indices
const GLushort ShapeDetails::diamond_indices[indicesLength] = {
    0, 1, 2,  // First triangle
    0, 2, 3   // Second triangle
};

// Opponent
const GLfloat ShapeDetails::opponent_points[verticesLength] = {
    -0.06f, -0.06f, 0.0f, // Bottom left
    0.06f, -0.06f, 0.0f, // Bottom right
    0.06f,  0.06f, 0.0f, // Top right
    -0.06f,  0.06f, 0.0f  // Top left
};

const GLushort ShapeDetails::opponent_indices[indicesLength] = {
    0, 1, 2, // First triangle
    0, 2, 3  // Second triangle
};

const GLfloat ShapeDetails::bullet_points[verticesLength] = {
        -0.01f, -0.01f, 0.0f, // Bottom left
         0.01f, -0.01f, 0.0f, // Bottom right
         0.01f,  0.01f, 0.0f, // Top right
        -0.01f,  0.01f, 0.0f  // Top left
};

const GLushort ShapeDetails::bullet_indices[indicesLength] = {
    0, 1, 2, // First triangle
    0, 2, 3  // Second triangle
};