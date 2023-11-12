#include <iostream>
#include <stdint.h>
#include <chrono>
#include <glad/glad.h>
#include <cmath>
#include "visual.h"
#include "game.h"
#include "field.h"
#include "cell.h"
#include "material.h"
#include "shaders.h"

#pragma comment(lib, "opengl32.lib")

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

void DrawScreenTexture();
void UpdateScreenTexture();
void UpdateScreenRect();

const uint8_t data_filled[] = { 255, 255, 255 };
const uint8_t data_empty[] = { 127, 127, 127 };
const float rectangle_vertices[] =
{
    // Coords     // texCoords
     1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    -1.0f,  1.0f,  0.0f, 1.0f,

     1.0f,  1.0f,  1.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f,  0.0f, 1.0f
};

uint32_t screen_texture;
uint8_t* screen_texture_data;

uint32_t shader_default;
uint32_t rectVAO, rectVBO;

bool matrix_updated;

void InitVisual()
{
    gladLoadGL();
    
    shader_default = LoadShader("shaders/default.vert", "shaders/default.frag");
    glUniform1i(glGetUniformLocation(shader_default, "screenTexture"), 0);

    glEnable(GL_TEXTURE_2D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    screen_texture_data = new uint8_t[FIELD_WIDTH * FIELD_HEIGHT * 3];

    UpdateScreenTexture();
}

void FreeVisual()
{
    delete[] screen_texture_data;
}

void DrawMain()
{
    auto t1 = high_resolution_clock::now();

    if (matrix_updated)
    {
        UpdateScreenTexture();
        matrix_updated = false;
    }

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    DrawScreenTexture();

    glColor3f(0.5f, 1.0f, 0.5f);
    glBegin(GL_LINES);
        glVertex2f(-0.999f, 0.999f); glVertex2f(0.999f, 0.999f);
        glVertex2f(-0.999f, -0.999f); glVertex2f(0.999f, -0.999f);
        glVertex2f(-0.999f, -0.999f); glVertex2f(-0.999f, 0.999f);
        glVertex2f(0.999f, -0.999f); glVertex2f(0.999f, 0.999f);
    glEnd();

    auto t2 = high_resolution_clock::now();

    duration<double, std::milli> ms_double = t2 - t1;

    //std::cout << "Rendering took " << ms_double.count() << "ms\n";
}

void DrawScreenTexture()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glUseProgram(shader_default);
    glBindTexture(GL_TEXTURE_2D, screen_texture);
    glBindVertexArray(rectVAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

void UpdateScreenTexture()
{
    if (screen_texture != NULL)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &screen_texture);
    }

    int color;
    for (int i = 0; i < FIELD_WIDTH; ++i)
    {
        for (int j = 0; j < FIELD_HEIGHT; ++j)
        {
            color = game_field->GetCell(i, j)->GetColor();
            screen_texture_data[3 * (j * FIELD_WIDTH + i) + 0] = (color & 0xFF0000) >> 16;
            screen_texture_data[3 * (j * FIELD_WIDTH + i) + 1] = (color & 0x00FF00) >> 8;
            screen_texture_data[3 * (j * FIELD_WIDTH + i) + 2] = (color & 0x0000FF);
        }
    }

    glGenTextures(1, &screen_texture);
    glBindTexture(GL_TEXTURE_2D, screen_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FIELD_WIDTH, FIELD_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, screen_texture_data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void UpdateScreenRect()
{
    glGenVertexArrays(1, &rectVAO);
    glGenBuffers(1, &rectVBO);
    glBindVertexArray(rectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), &rectangle_vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void SetMatrixUpdated()
{
    matrix_updated = true;
}

void Rescale(int width, int height)
{
    const float field_ratio = FIELD_HEIGHT / (float)FIELD_WIDTH;
    float frame_width, frame_height;
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

    UpdateScreenRect();
}