#include <cstdlib>
#include <string>
#include "RuinConfig.h"
#include "GL/glew.h"
#include "IL/il.h"
#include "IL/ilut.h"
#include "SDL.h"
#include "SDL_main.h"
#include "SDL_opengl.h"
#include "window.h"
#include "texture.h"
#include "shader.h"

const std::string titleString = "Project Ruin " + std::to_string(Ruin_VERSION_MAJOR) + "." + std::to_string(Ruin_VERSION_MINOR);

void initDevIL()
{
	ilInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

int main(int argc, char* args[])
{
	initDevIL();
	Window window;
	if (!window.initialize(titleString.c_str(), 640, 480, false))
	{
		return EXIT_FAILURE;
	}
	glewExperimental = GL_TRUE;
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		const GLubyte *message = glewGetErrorString(error);
		// TODO Print error message.
		return EXIT_FAILURE;
	}
	do
	{
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			const GLubyte *message = gluErrorString(error);
		}
	} while (error != GL_NO_ERROR);
	if (!GLEW_VERSION_3_3)
	{
		return EXIT_FAILURE;
	}
	Texture tex;
	if (!tex.initialize("assets/images/test.png"))
	{
		return EXIT_FAILURE;
	}
	Shader shader;
	if (!shader.loadProgram("assets/shaders/standardVertex.glsl", "assets/shaders/standardFragment.glsl", "assets/shaders/standardGeometry.glsl"))
	{
		return EXIT_FAILURE;
	}
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
	static const GLfloat vertexBufferData[] = {
		-1.0f, -1.0f,
		1.0f, 1.0f
	};
	static const GLfloat textureBufferData[] = {
		0.0f, 0.0f,
		1.0f, 1.0f
	};
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4, vertexBufferData, GL_STATIC_DRAW);
	GLuint textureBuffer;
	glGenBuffers(1, &textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4, textureBufferData, GL_STATIC_DRAW);
	bool running = true;
	while (running)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT);

		shader.bindProgram();
		tex.bindTexture();
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glDrawArrays(GL_LINES, 0, 2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		window.swapBuffer();
	}
	shader.destroy();
	tex.destroy();
	window.destroy();
	SDL_Quit();
	return EXIT_SUCCESS;
}
