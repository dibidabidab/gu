
#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include "../gl_includes.h"

class ShaderProgram
{
  public:
    static ShaderProgram fromFiles(std::string name, const char *vertPath, const char *fragPath);

    ShaderProgram(std::string name, const char *vertSource, const char *fragSource);
    ~ShaderProgram();

    bool compiled() const;
    GLuint id() const;
    GLuint location(const char *uniformName) const;

    void use();

  private:
    GLuint programId;
    std::string name;
    bool compiled_;

    void compileAndAttach(const char *source, GLuint shaderId, const char *shaderType);
};

#endif
