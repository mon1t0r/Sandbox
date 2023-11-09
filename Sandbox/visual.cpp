#include <glad/glad.h>
#include "visual.h"

#pragma comment(lib, "opengl32.lib")

void DrawMain()
{

}

void Rescale(int width, int height)
{
    int size = width > height ? height : width;
    glViewport((width - size) / 2, (height - size) / 2, size, size);
}