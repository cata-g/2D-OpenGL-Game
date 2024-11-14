#pragma once
#include "Shape.h"
#include "ShapeData.h"

class Elephant {
public:
    Elephant();
    void draw();
    void handleMovement(float deltaX, float deltaY);
    void deleteBuffers();

private:
    Shape body;
    Shape trunk;
    Shape leftEar;
    Shape rightEar;
    Shape tail;
    float speed;
};