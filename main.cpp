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
using namespace glm;

// Variables
GLFWwindow* window;
const int width = 1024, height = 768;

struct Bullet {
    float x, y;  // Position
    float speed; // Speed of the bullet
};
enum Direction {
    UP,
    LEFT,
    DOWN,
    RIGHT
};
struct Point {
    float x;
    float y;
    bool collected; // To track if the point has been collected
};

struct Opponent {
    float x, y;  // Position
    float speed; // Speed of the opponent
    bool active; // To track if the opponent is active
    Direction direction;
};

// Vector to hold opponents
std::vector<Opponent> opponents;

int score = 0;
float squareX = 0.0f; // X position of the square
float squareY = 0.0f; // Y position of the square
//const float speed = 0.0001f; // Speed of movement
bool isGameOver = false;
float angle;
const float speed = 0.01f;

int multiplierX = 1;
int multiplierY = 1;

#define PI 3.14159265358979323846
void renderScore() {
    // Use a text rendering library or OpenGL text rendering to display the score
    //std::cout << "Score: " << score << std::endl; // For console output
}


std::vector<Bullet> bullets; // Vector to hold bullets
void UpdateBullets() {
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        it->y += it->speed; // Move the bullet upwards
        if (it->y > 1.0f) { // Remove bullets that go off screen
            it = bullets.erase(it);
        }
        else {
            ++it;
        }
    }
}

void RenderBullets() {
    glColor3f(0.0f, 0.0f, 1.0f); // Bullet color (black)

    for (const Bullet& bullet : bullets) {
        glPushMatrix();
        glTranslatef(bullet.x, bullet.y, 0.0f);

        // Draw a small bullet (you can adjust the size)
        glBegin(GL_QUADS);
        glVertex2f(-0.01f, -0.01f);
        glVertex2f(0.01f, -0.01f);
        glVertex2f(0.01f, 0.01f);
        glVertex2f(-0.01f, 0.01f);
        glEnd();

        glPopMatrix();
    }
}

float mapToNDC(float coord, float maxCoord, float minCoord) {
    return (2.0f * (coord - minCoord) / (maxCoord - minCoord)) - 1.0f;
}
Point spawnPoint() {
    Point newPoint;
    float boundaryOffset = 0.4f; // Offset from the edges of the screen

    // Calculate the valid range for spawning points
    float minX = -1.0f + boundaryOffset; // Minimum X in NDC
    float maxX = 1.0f - boundaryOffset;  // Maximum X in NDC
    float minY = -1.0f + boundaryOffset; // Minimum Y in NDC
    float maxY = 1.0f - boundaryOffset;  // Maximum Y in NDC

    // Generate random positions within the defined boundaries
    newPoint.x = minX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxX - minX)));
    newPoint.y = minY + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxY - minY)));

    std::cout << "New Point spawned at " << newPoint.x << " " << newPoint.y << std::endl;
    newPoint.collected = false;
    return newPoint;
}

bool checkCollision(const Point& point) {
    // Define the hitbox for the player and the point
    float playerWidth = 0.4f; // Assuming player has a width attribute
    float playerHeight = 0.4f; // Assuming player has a height attribute
    float pointSize = 0.025; // Size of the diamond point
    float pointNDCX =point.x;
    float pointNDCY = point.y;
    /*std::cout << "Player details " << squareX << " " << squareY << std::endl;
    std::cout << "Point details " << pointNDCX + pointSize << " " << pointNDCY + pointSize << std::endl;*/
    /*std::cout << (squareX < pointNDCX + pointSize &&
        squareX + playerWidth > pointNDCX &&
        squareY < pointNDCY + pointSize &&
        squareY + playerHeight > pointNDCY) << std::endl;*/
    // Check for overlap
    return (squareX < pointNDCX + pointSize &&
        squareX + playerWidth > pointNDCX &&
        squareY < pointNDCY + pointSize &&
        squareY + playerHeight > pointNDCY);
}

bool checkCollisionWOpponent(const Opponent& point) {
    // Define the hitbox for the player and the point
    float playerWidth = 0.2f; // Assuming player has a width attribute
    float playerHeight = 0.2f; // Assuming player has a height attribute
    float pointSize = 0.001;//ize of the diamond point
    float pointNDCX = point.x;
    float pointNDCY = point.y;
    //std::cout << "Player details " << squareX << " " << squareY << std::endl;
    //std::cout << "Point details " << pointNDCX + pointSize << " " << pointNDCY + pointSize << std::endl;
    //std::cout << (squareX < pointNDCX + pointSize &&
    //    squareX + playerWidth > pointNDCX &&
    //    squareY < pointNDCY + pointSize &&
    //    squareY + playerHeight > pointNDCY) << std::endl;
        // Check for overlap
    return (squareX < pointNDCX + pointSize &&
        squareX + playerWidth > pointNDCX &&
        squareY < pointNDCY + pointSize &&
        squareY + playerHeight > pointNDCY);
}

void drawDiamond(float x, float y) {
    // Convert to NDC
    float ndcX = x;
    float ndcY = y;

    glBegin(GL_TRIANGLES);

    // Draw the diamond shape using two triangles
    glVertex2f(ndcX, ndcY + 0.025f);   // Top vertex
    glVertex2f(ndcX - 0.025f, ndcY);   // Left vertex
    glVertex2f(ndcX, ndcY - 0.025f);   // Bottom vertex

    glVertex2f(ndcX, ndcY - 0.025f);   // Bottom vertex
    glVertex2f(ndcX + 0.025f, ndcY);   // Right vertex
    glVertex2f(ndcX, ndcY + 0.025f);   // Top vertex

    glEnd();
}
// Body vertices
const GLfloat square_points[] = {
    -0.1f, -0.1f, 0.0f, // Bottom left
     0.1f, -0.1f, 0.0f, // Bottom right
     0.1f,  0.1f, 0.0f, // Top right
    -0.1f,  0.1f, 0.0f  // Top left
};

// Square indices
const GLushort square_indices[] = {
    0, 1, 2, // First triangle
    0, 2, 3  // Second triangle
};

// Trunk vertices 
const GLfloat trunk_points[] = {
    -0.03f, 0.1f, 0.0f, // Bottom left
     0.03f, 0.1f, 0.0f, // Bottom right
     0.03f, 0.2f, 0.0f, // Top right (changed from 0.3f to 0.2f)
    -0.03f, 0.2f, 0.0f  // Top left (changed from 0.3f to 0.2f)
};

// Trunk indices
const GLushort trunk_indices[] = {
    0, 1, 2, // First triangle
    0, 2, 3  // Second triangle
};

// Ear vertices 
const GLfloat left_ear_points[] = {
    -0.1f, 0.1f, 0.0f, // Bottom left
    -0.05f, 0.1f, 0.0f, // Bottom right
    -0.05f, 0.2f, 0.0f, // Top right
    -0.1f, 0.2f, 0.0f   // Top left
};

// Right Ear vertices
const GLfloat right_ear_points[] = {
    0.1f, 0.1f, 0.0f,  // Bottom left
    0.05f, 0.1f, 0.0f, // Bottom right
    0.05f, 0.2f, 0.0f, // Top right
    0.1f, 0.2f, 0.0f   // Top left
};

// Ear indices
const GLushort ear_indices[] = {
    0, 1, 2, // First triangle
    0, 2, 3  // Second triangle
};

// Tail vertices
const GLfloat tail_points[] = {
    -0.02f, -0.1f, 0.0f, // Bottom left
     0.02f, -0.1f, 0.0f, // Bottom right
     0.02f, -0.05f, 0.0f, // Top right
    -0.02f, -0.05f, 0.0f  // Top left
};

// Tail indices
const GLushort tail_indices[] = {
    0, 1, 2, // First triangle
    0, 2, 3  // Second triangle
};

float CalculateAngle(float characterX, float characterY, double mouseX, double mouseY) {
    float deltaX = mouseX - characterX;
    float deltaY = mouseY - characterY;
    return atan2(deltaY, deltaX) * (180.0f / PI); // Convert radians to degrees
}

// Callbacks
// Key callbacks
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

}

// Window callback
void window_callback(GLFWwindow* window, int new_width, int new_height)
{
    glViewport(0, 0, new_width, new_height);
}
float bulletFireRate = 0.5f; // Fire rate in seconds (e.g., 0.5 seconds between shots)
float lastBulletTime = 0.0f; // Time of the last bullet fired
// Movement method
void HandleMovement()
{
    // Get the current window size
    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);

    // Calculate the boundaries
    float half_square_width = 0.2f; // Half of the square's width
    float half_square_height = 0.2f; // Half of the square's height

    float boundary = 0.9f;

    // Move up
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        if (squareY + half_square_height < boundary) { // Check upper boundary
            squareY += speed;
            multiplierX = multiplierY = 1;
        }
    }

    // Move down
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        if (squareY - half_square_height > -boundary) { // Check lower boundary
            squareY -= speed;
            multiplierY = -1;
            multiplierX = -1;
        }
    }

    // Move left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if (squareX - half_square_width > -boundary) { // Check left boundary
            squareX -= speed;
            //multiplierX = 1;
            //multiplierY = 1;
        }
    }

    // Move right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if (squareX + half_square_width < boundary) { // Check right boundary
            squareX += speed;
            //multiplierY = 1;
            //multiplierX = 1;
        }
    }
    float currentTime = glfwGetTime();
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (currentTime - lastBulletTime >= bulletFireRate) {
            Bullet newBullet;
            newBullet.x = squareX; // Start bullet from the trunk's position
            newBullet.y = squareY + 0.15f; // Position it above the trunk
            newBullet.speed = multiplierY * 0.05f; // Set bullet speed
            /*newBullet.speed = 0.002;*/
            bullets.push_back(newBullet); // Add new bullet to the vector
            lastBulletTime = currentTime;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        angle = 0; // Up
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        angle = 180.0f; // Down
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        //angle = 90; // Left
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        //angle =270.0f; // Right
    }
}

void spawnOpponent(float diamondX, float diamondY) {
    Opponent newOpponent;
    std::cout << "Aici" << diamondX << " " << diamondY << std::endl;
    newOpponent.x = diamondX - 0.1f; // Randomly position near the diamond
    newOpponent.y = diamondY; // Randomly position near the diamond
    newOpponent.speed = 0.0005f; // Set speed
    newOpponent.active = true; // Set as active
    newOpponent.direction = UP;
    opponents.push_back(newOpponent); // Add to the vector

    std::cout << "Spawned opponent at: (" << newOpponent.x << ", " << newOpponent.y << ")\n"; // Debug output
}


void UpdateOpponents(Point p) {
    float diamondX = p.x; // Get diamond's original X position
    float diamondY = p.y; // Get diamond's Y position
    float distanceAround = 0.15f;

    for (auto& opponent : opponents) {
        if (opponent.active) {
            // Check the current direction and move accordingly
            switch (opponent.direction) {
            case UP:
                opponent.y += opponent.speed; // Move up
                if (opponent.y >= diamondY + distanceAround) {
                    opponent.direction = LEFT; // Change direction to left
                }
                break;
            case LEFT:
                opponent.x -= opponent.speed; // Move left
                if (opponent.x <= diamondX - distanceAround) {
                    opponent.direction = DOWN; // Change direction to down
                }
                break;
            case DOWN:
                opponent.y -= opponent.speed; // Move down
                if (opponent.y <= diamondY - distanceAround) {
                    opponent.direction = RIGHT; // Change direction to right
                }
                break;
            case RIGHT:
                opponent.x += opponent.speed; // Move right
                if (opponent.x >= diamondX + distanceAround) {
                    opponent.direction = UP; // Change direction to up
                }
                break;
            }
        }
    }
}

// Render opponents with NDC conversion
void RenderOpponents() {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    glColor3f(1.0f, 0.0f, 0.0f); // Opponent color (red)

    for (const Opponent& opponent : opponents) {
        if (opponent.active) { // Only render active opponents
            float ndcX = opponent.x;
            float ndcY = opponent.y;

            glPushMatrix();
            glTranslatef(ndcX, ndcY, 0.0f);

            // Draw a rectangle for the opponent
            glBegin(GL_QUADS);
            glVertex2f(-0.04f, -0.06f); // Bottom left
            glVertex2f(0.04f, -0.06f);  // Bottom right
            glVertex2f(0.04f, 0.02f);   // Top right
            glVertex2f(-0.04f, 0.02f);  // Top left
            glEnd();

            glPopMatrix();
        }
    }
}

// Check collision between bullets and opponents
bool checkBulletCollision(const Bullet& bullet, Opponent& opponent) {
    float opponentSize = 0.06f; // Size of the opponent (adjust as necessary)
    float bulletSize = 0.02f; // Size of the bullet

    return (bullet.x < opponent.x + opponentSize &&
        bullet.x + bulletSize > opponent.x &&
        bullet.y < opponent.y + opponentSize &&
        bullet.y + bulletSize > opponent.y);
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

    // Creating the body buffer
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square_points), square_points, GL_STATIC_DRAW);

    GLuint elementBuffer;
    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square_indices), square_indices, GL_STATIC_DRAW);

    // Create the trunk buffer
    GLuint trunk_buffer;
    glGenBuffers(1, &trunk_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, trunk_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trunk_points), trunk_points, GL_STATIC_DRAW);

    GLuint trunkElementBuffer;
    glGenBuffers(1, &trunkElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trunkElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(trunk_indices), trunk_indices, GL_STATIC_DRAW);

    // Create the L Ear buffer
    GLuint l_ear_buffer;
    glGenBuffers(1, &l_ear_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, l_ear_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(left_ear_points), left_ear_points, GL_STATIC_DRAW);

    GLuint LEarElementBuffer;;
    glGenBuffers(1, &LEarElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LEarElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ear_indices), ear_indices, GL_STATIC_DRAW);

    // Create the R Ear buffer
    GLuint r_ear_buffer;
    glGenBuffers(1, &r_ear_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, r_ear_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(right_ear_points), right_ear_points, GL_STATIC_DRAW);

    GLuint REarElementBuffer;;
    glGenBuffers(1, &REarElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, REarElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ear_indices), ear_indices, GL_STATIC_DRAW);

    // Create the tail buffer
    GLuint tail_buffer;
    glGenBuffers(1, &tail_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, tail_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tail_points), tail_points, GL_STATIC_DRAW);

    GLuint tailElementBuffer;
    glGenBuffers(1, &tailElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tailElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tail_indices), tail_indices, GL_STATIC_DRAW);

    std::vector<Point> points;
    Point p = spawnPoint();
    points.push_back(p); // Initial point

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    do {
        // Clear the screen to not flicker
        glClear(GL_COLOR_BUFFER_BIT);

        // Set callbacks functions
        /*glfwSetKeyCallback(window, key_callback); */
        glfwSetFramebufferSizeCallback(window, window_callback); // Handle resize

        if (!isGameOver) {
            // Handle Moevemet
            HandleMovement();
            // Update bullets
            UpdateBullets();

            // Update opponents
            UpdateOpponents(points.back());

            // Render bullets
            RenderBullets();

            for (auto& point : points) {
                if (!point.collected && checkCollision(point)) {
                    point.collected = true;
                    score++;
                    Point p = spawnPoint();
                    points.push_back(p); // Spawn a new point
                    spawnOpponent(p.x, p.y);
                }
            }

            // Check for bullet collisions with opponents
            for (auto& bullet : bullets) {
                for (auto& opponent : opponents) {
                    if (opponent.active && checkBulletCollision(bullet, opponent)) {
                        opponent.active = false; // Mark opponent as inactive
                    }

                }
            }

            for (auto& opponent : opponents) {

                if (opponent.active && checkCollisionWOpponent(opponent)) {
                    std::cout << "Game Over " << std::endl;
                    isGameOver = true;
                }
            }

            // Render points
            for (const auto& point : points) {
                if (!point.collected) {
                    drawDiamond(point.x, point.y); // Function to draw the diamond
                }
            }

            // Render opponents
            RenderOpponents();

            renderScore(); // Render the score
            // Other rendering and update logic
            // 
        }
            // Draw the body
            glPushMatrix();
            glTranslatef(squareX, squareY, 0.0f);
            glRotatef(angle, 0.0f, 0.0f, 1.0f);
            glColor3f(1.0f, 0.75f, 0.8f);

            // 1st attribute buffer: vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glVertexAttribPointer(
                0,                  // attribute 0
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );

            // Draw the body using indices
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0); // 6 indices
            glDisableVertexAttribArray(0);
            glPopMatrix();

            // Draw the trunk
            glPushMatrix();
            glTranslatef(squareX, squareY, 0.0f); // Position the trunk above the square
            glRotatef(angle, 0.0f, 0.0f, 1.0f);
            glColor3f(1.0f, 0.75f, 0.8f);

            // 1st attribute buffer: vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, trunk_buffer);
            glVertexAttribPointer(
                0,                  // attribute 0
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );

            // Draw the trunk using indices
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trunkElementBuffer);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0); // 6 indices
            glDisableVertexAttribArray(0);
            glPopMatrix();

            // Draw the L Ear
            glPushMatrix();
            glRotatef(angle, 0.0f, 0.0f, 1.0f);
            glTranslatef(multiplierX * squareX - 0.05f, multiplierY * squareY - 0.15f, 0.0f);
            glColor3f(1.0f, 0.75f, 0.8f);

            // 1st attribute buffer: vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, l_ear_buffer);
            glVertexAttribPointer(
                0,                  // attribute 0
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );

            // Draw the L Ear using indices
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LEarElementBuffer);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0); // 6 indices
            glDisableVertexAttribArray(0);
            glPopMatrix();

            // Draw the R_EAR
            glPushMatrix();
            glRotatef(angle, 0.0f, 0.0f, 1.0f);
            glTranslatef(multiplierX * squareX + 0.05f, multiplierY * squareY - 0.15f, 0.0f);
            glColor3f(1.0f, 0.75f, 0.8f);

            // 1st attribute buffer: vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, r_ear_buffer);
            glVertexAttribPointer(
                0,                  // attribute 0
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );

            // Draw the R Ear using indices
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, REarElementBuffer);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0); // 6 indices
            glDisableVertexAttribArray(0);
            glPopMatrix();

            // Draw the tail
            glPushMatrix();
            glRotatef(angle, 0.0f, 0.0f, 1.0f);
            glTranslatef(multiplierX * squareX, multiplierY * squareY - 0.05, 0.0f); // Position the tail behind the body
            glColor3f(1.0f, 0.75f, 0.8f); // Same color as the body

            // 1st attribute buffer: vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, tail_buffer);
            glVertexAttribPointer(
                0,                  // attribute 0
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );

            // Draw the tail using indices
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tailElementBuffer);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0); // 6 indices
            glDisableVertexAttribArray(0);
            glPopMatrix();
        

        // Swap Buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    // Cleanup
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementBuffer);    
    glDeleteBuffers(1, &trunk_buffer);
    glDeleteBuffers(1, &trunkElementBuffer);
    glDeleteBuffers(1, &l_ear_buffer);
    glDeleteBuffers(1, &r_ear_buffer);
    glDeleteBuffers(1, &LEarElementBuffer);
    glDeleteBuffers(1, &REarElementBuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glfwTerminate();
    return 0;
}
