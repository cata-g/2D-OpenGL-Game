#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    glGenBuffers(1, &m_RendererID); // generating buffer getting back id
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); // select the buffer (select = bind in opengl)
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); // add data
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); // select the buffer (select = bind in opengl)
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0); // select the buffer (select = bind in opengl)
}
