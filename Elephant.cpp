#include "Elephant.h"

Elephant::Elephant()
{
    speed = 0.01f;

    ShapeData shapesData;
    body.setData(shapesData.getSquarePoints(), shapesData.getSquareIndices(), 4, 6);
    trunk.setData(shapesData.getTrunkPoints(), shapesData.getTrunkIndices(), 4, 6);
    leftEar.setData(shapesData.getLeftEarPoints(), shapesData.getEarIndices(), 4, 6);
    rightEar.setData(shapesData.getRightEarPoints(), shapesData.getEarIndices(), 4, 6);
    tail.setData(shapesData.getTailPoints(), shapesData.getTailIndices(), 4, 6);

    body.Generate();
    trunk.Generate();
    leftEar.Generate();
    rightEar.Generate();
    tail.Generate();
}

void Elephant::draw()
{
    body.draw();
    trunk.draw();
    leftEar.draw();
    rightEar.draw();
    tail.draw();
}

void Elephant::handleMovement(float deltaX, float deltaY)
{
    body.setPosition(body.getPosition().x + deltaX, body.getPosition().y + deltaY);
    trunk.setPosition(trunk.getPosition().x + deltaX, trunk.getPosition().y + deltaY);
    leftEar.setPosition(leftEar.getPosition().x + deltaX, leftEar.getPosition().y + deltaY);
    rightEar.setPosition(rightEar.getPosition().x + deltaX, rightEar.getPosition().y + deltaY);
    tail.setPosition(tail.getPosition().x + deltaX, tail.getPosition().y + deltaY);
}

void Elephant::deleteBuffers()
{
    body.deleteBuffers();
    trunk.deleteBuffers();
    leftEar.deleteBuffers();
    rightEar.deleteBuffers();
    tail.deleteBuffers();
}
