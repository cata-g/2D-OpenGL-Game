#include "Renderer.h"
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLCheckError()
{
    while (GLenum error = glGetError())
    {
        // should transform in hexadecimal in search in glew
        std::cout << "[OpenGL Error] " << error << std::endl;
        return false;
    }

    return true;
}