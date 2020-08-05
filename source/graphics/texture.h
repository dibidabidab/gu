#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>
#include "../gl_includes.h"
#include "shader_program.h"

class Texture;

typedef std::shared_ptr<Texture> SharedTexture;

class Texture
{

  public:

    /**
     * Load a DXT1/3/5 texture directly to OpenGL
     */
    static SharedTexture fromDDSFile(const char *path); // todo: should return Texture instead of SharedTexture. But changing that will break old projects.

    static Texture fromImageFile(const char *path);

    const GLuint id, width, height;

    Texture(GLuint id, GLuint width, GLuint height);

    void bind(GLuint unit);

    // same as bind() but also sets sampler uniform for shader.
    void bind(GLuint unit, const ShaderProgram &shader, const char* name);

    ~Texture();

};

class DDSData
{
  public:

    GLuint height, width, linearSize, mipMapCount, fourCC, format, bufsize;

    unsigned char *buffer;

    DDSData(const char *path);

    ~DDSData();
};

#endif
