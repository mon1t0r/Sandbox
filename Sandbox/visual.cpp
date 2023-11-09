#include <glad/glad.h>
#include <cmath>
#include "visual.h"
#include "game.h"

#pragma comment(lib, "opengl32.lib")
 
float field_ratio = FIELD_HEIGHT / (float)FIELD_WIDTH;
float frame_width;

void DrawMain()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    float col;
    float start_x = 1.0f / (float)FIELD_WIDTH - 1.0f;
    float start_y = 1.0f / (float)FIELD_HEIGHT - 1.0f;

    glPointSize(frame_width / (float)FIELD_WIDTH);
    glBegin(GL_POINTS);
        for(int i = 0; i < FIELD_WIDTH; ++i)
            for (int j = 0; j < FIELD_HEIGHT; ++j)
            {
                col = matrix[i][j] ? 1.0f : 0.5f;
                glColor3f(col, col, col);
                glVertex2f(start_x + i * 2.0f / (float)FIELD_WIDTH, start_y + j * 2.0f / (float)FIELD_HEIGHT);
            }
    glEnd();

    glColor3f(0.5f, 1.0f, 0.5f);
    glBegin(GL_LINES);
        glVertex2f(-0.999f, 0.999f); glVertex2f(0.999f, 0.999f);
        glVertex2f(-0.999f, -0.999f); glVertex2f(0.999f, -0.999f);
        glVertex2f(-0.999f, -0.999f); glVertex2f(-0.999f, 0.999f);
        glVertex2f(0.999f, -0.999f); glVertex2f(0.999f, 0.999f);
    glEnd();
}

void Rescale(int width, int height)
{
    float frame_height;
    if (width > height / field_ratio)
    {
        frame_width = (int)roundf(height / field_ratio);
        frame_height = height;
    }
    else
    {
        frame_width = width;
        frame_height = (int)roundf(width * field_ratio);
    }
    glViewport((width - frame_width) / 2, (height - frame_height) / 2, frame_width, frame_height);
}