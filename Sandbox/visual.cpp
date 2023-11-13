#include <iostream>
#include <stdint.h>
#include <glad/glad.h>
#include <cmath>
#include "visual.h"
#include "game.h"
#include "field.h"
#include "cell.h"
#include "material.h"
#include "shaders.h"

#pragma comment(lib, "opengl32.lib")

void InitializeTextures();
void UpdateScreenTexture();
void UpdateScreenRect();

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
const float frame_vertices[] =
{
    -0.999f,  0.999f,   0.999f, 0.999f,
    -0.999f, -0.999f,   0.999f, -0.999f,
    -0.999f, -0.999f,  -0.999f, 0.999f,
     0.999f, -0.999f,   0.999f, 0.999f
};

uint32_t screen_texture;
uint8_t* screen_texture_data;

uint32_t frame_texture;

uint32_t shader_default;
uint32_t rectVAO, rectVBO;
uint32_t frameVAO, frameVBO;

bool matrix_updated;

void InitVisual()
{
    gladLoadGL();
    
    shader_default = LoadShader("shaders/default.vert", "shaders/default.frag");
    glUniform1i(glGetUniformLocation(shader_default, "screenTexture"), 0);

    glEnable(GL_TEXTURE_2D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    screen_texture_data = new uint8_t[FIELD_WIDTH * FIELD_HEIGHT * 3];

    InitializeTextures();
}

void FreeVisual()
{
    delete[] screen_texture_data;
}

void DrawMain()
{
    if (matrix_updated)
    {
        UpdateScreenTexture();
        matrix_updated = false;
    }

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glUseProgram(shader_default);

    //Draw screen
    glBindTexture(GL_TEXTURE_2D, screen_texture);
    glBindVertexArray(rectVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //Draw frame
    glBindTexture(GL_TEXTURE_2D, frame_texture);
    glBindVertexArray(frameVAO);
    glDrawArrays(GL_LINES, 0, 8);
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


    glGenVertexArrays(1, &frameVAO);
    glGenBuffers(1, &frameVBO);
    glBindVertexArray(frameVAO);
    glBindBuffer(GL_ARRAY_BUFFER, frameVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(frame_vertices), &frame_vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
}

void InitializeTextures()
{
    UpdateScreenTexture();

    const uint8_t frame_texture_data[] { 50, 50, 50 };

    glGenTextures(1, &frame_texture);
    glBindTexture(GL_TEXTURE_2D, frame_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &frame_texture_data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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