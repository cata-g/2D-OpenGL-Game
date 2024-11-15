// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW - GLFW handles the window and the keyboard
#include "dependente\glfw\glfw3.h"
#include "dependente/freeglut/glut.h"

// Library for 3D mathematics
#include "dependente/glm/glm.hpp"
#include "main.h"
#include <vector> // Include vector header


#include <cmath>
#include "ShapeDetails.h"
#include "ObjectsTypes.h"
using namespace glm;

GLFWwindow* window;
const int width = 1024, height = 768;

std::vector<Bullet> bullets; 
std::vector<Opponent> opponents;
Player player = Player();

int score = 0;
bool isGameOver = false;
float angle;

int multiplierX = 1;
int multiplierY = 1;

float bulletFireRate = 0.5f;
float lastBulletTime = 0.0f;

struct Buffer {
    GLuint vertexBuffer;
    GLuint elementBuffer;
    const GLfloat* vertices;
    const GLushort* indices;
    size_t vertexCount;
    size_t indexCount;
};


void UpdateBullets() {
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        it->y += it->speed; 
        if (it->y > 1.0f) { 
            it = bullets.erase(it);
        }
        else {
            ++it;
        }
    }
}


float mapToNDC(float coord, float maxCoord, float minCoord) {
    return (2.0f * (coord - minCoord) / (maxCoord - minCoord)) - 1.0f;
}

Point spawnPoint() {
    float boundaryOffset = 0.4f; 

    float minX = -1.0f + boundaryOffset; 
    float maxX = 1.0f - boundaryOffset;  
    float minY = -1.0f + boundaryOffset; 
    float maxY = 1.0f - boundaryOffset; 

    float x = minX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxX - minX)));
    float y = minY + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxY - minY)));
    Point newPoint = Point(x, y, false);
    std::cout << "New Point spawned at " << newPoint.getX() << " " << newPoint.getY() << std::endl;
    newPoint.collected = false;
    return newPoint;
}

bool checkCollision(const GameObject &who, const GameObject &with) {

    return (who.getX() < with.getX() + with.getSize() &&
        who.getX() + with.getSize() > with.getX() &&
        who.getY() < with.getY() + with.getSize() &&
        who.getY() + with.getSize() > with.getY());
}

void window_callback(GLFWwindow* window, int new_width, int new_height)
{
    glViewport(0, 0, new_width, new_height);
}

void HandleMovement()
{
    float boundary = 0.9f;

    // Move up
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        if (player.getY() + player.getSize() / 2 < boundary) {
            player.setY(player.getSpeed());
            multiplierX = multiplierY = 1;
            angle = 0;
        }
    }

    // Move down
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        if (player.getY() - player.getSize() / 2 > -boundary) { 
            player.setY(-player.getSpeed());
            multiplierY = -1;
            multiplierX = -1;
            angle = 180.0f;
        }
    }

    // Move left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if (player.getX() - player.getSize() / 2 > -boundary) { 
            player.setX(-player.getSpeed());
        }
    }

    // Move right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if (player.getX() + player.getSize() / 2 < boundary) { 
            player.setX(player.getSpeed());
        }
    }
    float currentTime = glfwGetTime();
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (currentTime - lastBulletTime >= bulletFireRate) {
            Bullet newBullet = Bullet(player.getX(), player.getY() + 0.15f, multiplierY * 0.05f);
            bullets.push_back(newBullet); 
            lastBulletTime = currentTime;
        }
    }
}

void UpdateOpponents(Point p) {
    float diamondX = p.x; 
    float diamondY = p.y; 
    float distanceAround = 0.15f;

    for (auto& opponent : opponents) {
        if (opponent.active) {
            switch (opponent.direction) {
            case Direction::Up:
                opponent.y += opponent.speed; 
                if (opponent.y >= diamondY + distanceAround) {
                    opponent.direction = Direction::Left; 
                }
                break;
            case Direction::Left:
                opponent.x -= opponent.speed;
                if (opponent.x <= diamondX - distanceAround) {
                    opponent.direction = Direction::Down; 
                }
                break;
            case Direction::Down:
                opponent.y -= opponent.speed; 
                if (opponent.y <= diamondY - distanceAround) {
                    opponent.direction = Direction::Right; 
                }
                break;
            case Direction::Right:
                opponent.x += opponent.speed; 
                if (opponent.x >= diamondX + distanceAround) {
                    opponent.direction = Direction::Up; 
                }
                break;
            }
        }
    }
}

void CreateBuffers(Buffer& buffer) {
    glGenBuffers(1, &buffer.vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, buffer.vertexCount * sizeof(GLfloat), buffer.vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &buffer.elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.indexCount * sizeof(GLushort), buffer.indices, GL_STATIC_DRAW);
}

void RenderShape(Buffer buffer, float offsetX, float offsetY, GLfloat color[], GameObject reference) {
    glPushMatrix();
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(multiplierX * reference.getX() + offsetX, multiplierY * reference.getY() + offsetY, 0.0f);
    glColor3f(color[0], color[1], color[2]);

    // 1st attribute buffer: vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vertexBuffer);
    glVertexAttribPointer(
        0,                  // attribute 0
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.elementBuffer);
    glDrawElements(GL_TRIANGLES, buffer.indexCount, GL_UNSIGNED_SHORT, (void*)0); 
    glDisableVertexAttribArray(0);
    glPopMatrix();
}

int main() {
    // Basic Setup
    glewExperimental = true;
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    window = glfwCreateWindow(width, height, "Project 2D", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW Window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    Buffer bodyBuffer = { 0, 0, ShapeDetails::body_points, ShapeDetails::body_indices, sizeof(ShapeDetails::body_points) / sizeof(GLfloat), sizeof(ShapeDetails::body_indices) / sizeof(GLushort) };
    Buffer trunkBuffer = { 0, 0, ShapeDetails::trunk_points, ShapeDetails::trunk_indices, sizeof(ShapeDetails::trunk_points) / sizeof(GLfloat), sizeof(ShapeDetails::trunk_indices) / sizeof(GLushort) };
    Buffer leftEarBuffer = { 0, 0, ShapeDetails::left_ear_points, ShapeDetails::ear_indices, sizeof(ShapeDetails::left_ear_points) / sizeof(GLfloat), sizeof(ShapeDetails::ear_indices) / sizeof(GLushort) };
    Buffer rightEarBuffer = { 0, 0, ShapeDetails::right_ear_points, ShapeDetails::ear_indices, sizeof(ShapeDetails::right_ear_points) / sizeof(GLfloat), sizeof(ShapeDetails::ear_indices) / sizeof(GLushort) };
    Buffer tailBuffer = { 0, 0, ShapeDetails::tail_points, ShapeDetails::tail_indices, sizeof(ShapeDetails::tail_points) / sizeof(GLfloat), sizeof(ShapeDetails::tail_indices) / sizeof(GLushort) };
    Buffer diamondBuffer = { 0, 0, ShapeDetails::diamond_points, ShapeDetails::diamond_indices, sizeof(ShapeDetails::diamond_points) / sizeof(GLfloat), sizeof(ShapeDetails::diamond_indices) / sizeof(GLushort) };
    Buffer opponentBuffer = { 0, 0, ShapeDetails::opponent_points, ShapeDetails::opponent_indices, sizeof(ShapeDetails::opponent_points) / sizeof(GLfloat), sizeof(ShapeDetails::opponent_indices) / sizeof(GLushort) };
    Buffer bulletBuffer = { 0, 0, ShapeDetails::bullet_points, ShapeDetails::bullet_indices, sizeof(ShapeDetails::bullet_points) / sizeof(GLfloat), sizeof(ShapeDetails::bullet_indices) / sizeof(GLushort) };
    
    // Create all buffers
    CreateBuffers(bodyBuffer);
    CreateBuffers(trunkBuffer);
    CreateBuffers(leftEarBuffer);
    CreateBuffers(rightEarBuffer);
    CreateBuffers(tailBuffer);
    CreateBuffers(diamondBuffer);
    CreateBuffers(opponentBuffer);
    CreateBuffers(bulletBuffer);

    GLfloat pink[] = { 1.0f, 0.75f, 0.8f };
    GLfloat blue[] = { 0.0f, 0.0f, 0.8f };
    GLfloat red[] = { 0.8f, 0.0f, 0.0f };

    std::vector<Point> points;
    Point p = spawnPoint();
    points.push_back(p); // Initial point

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    do {
        // Clear the screen to not flicker
        glClear(GL_COLOR_BUFFER_BIT);

        // Set callbacks functions
        // Handle resize
        glfwSetFramebufferSizeCallback(window, window_callback); 

        if (!isGameOver) {
            // Handle Moevemet
            HandleMovement();

            // Update bullets
            UpdateBullets();

            // Update opponents
            UpdateOpponents(points.back());

            // Render bullets
            for (const Bullet& bullet : bullets) {
                RenderShape(bulletBuffer, 0, 0, blue, bullet);
            }

            // Check collisions
            for (auto& point : points) {
                if (!point.collected && checkCollision(player, point)) {
                    point.collected = true;
                    score++;
                    Point p = spawnPoint();
                    points.push_back(p); 
                    opponents.push_back(Opponent(p.x - 0.1f, p.y, 0.0005f)); 
                }
            }
            for (auto& bullet : bullets) {
                for (auto& opponent : opponents) {
                    if (opponent.active && checkCollision(bullet, opponent)) {
                        opponent.active = false; 
                    }

                }
            }

            for (auto& opponent : opponents) {
                if (opponent.active && checkCollision(player, opponent)) {
                    std::cout << "Game Over " << std::endl;
                    isGameOver = true;
                }
            }

            // Render points
            for (const auto& point : points) {
                if (!point.collected) {
                    RenderShape(diamondBuffer, 0, 0, blue, point);
                }
            }

            // Render opponents
            for (const Opponent& opponent : opponents)
                if (opponent.active)
                    RenderShape(opponentBuffer, 0, 0, red, opponent);

        }
        RenderShape(bodyBuffer, 0, 0, pink, player);
        RenderShape(trunkBuffer,0 ,0, pink, player);
        RenderShape(tailBuffer, 0, -0.05, pink, player);
        RenderShape(rightEarBuffer, 0.05, -0.15, pink, player);
        RenderShape(leftEarBuffer, -0.05, -0.15, pink, player);

        // Swap Buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    // Cleanup
    glDeleteBuffers(1, &bodyBuffer.vertexBuffer);
    glDeleteBuffers(1, &bodyBuffer.elementBuffer);    
    glDeleteBuffers(1, &trunkBuffer.vertexBuffer);
    glDeleteBuffers(1, &trunkBuffer.elementBuffer);
    glDeleteBuffers(1, &leftEarBuffer.vertexBuffer);
    glDeleteBuffers(1, &leftEarBuffer.elementBuffer);
    glDeleteBuffers(1, &rightEarBuffer.vertexBuffer);
    glDeleteBuffers(1, &rightEarBuffer.elementBuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glfwTerminate();
    return 0;
}
