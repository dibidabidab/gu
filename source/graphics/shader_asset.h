
#ifndef GAME_SHADER_ASSET_H
#define GAME_SHADER_ASSET_H

#include "shader_program.h"
#include "../asset_manager/asset.h"

class ShaderAsset : public ShaderProgram
{

    asset<std::string> vertCode, fragCode, geomCode;

  public:

    enum class CompileBehavior
    {
        COMPILE_ON_CONSTRUCT,
        COMPILE_ON_FIRST_USE
    };

    ShaderAsset(const std::string &name, const std::string &vertPath, CompileBehavior compile = CompileBehavior::COMPILE_ON_CONSTRUCT)
        : ShaderProgram(name, asset<std::string>(vertPath)->c_str(), nullptr, compile == CompileBehavior::COMPILE_ON_CONSTRUCT),
          vertCode(vertPath)
    {}

    ShaderAsset(const std::string &name, const std::string &vertPath, const std::string &fragPath, CompileBehavior compile = CompileBehavior::COMPILE_ON_CONSTRUCT)
        : ShaderProgram(name, asset<std::string>(vertPath)->c_str(), asset<std::string>(fragPath)->c_str(), compile == CompileBehavior::COMPILE_ON_CONSTRUCT),
          vertCode(vertPath), fragCode(fragPath)
    {}

    ShaderAsset(const std::string &name, const std::string &vertPath, const std::string &geomPath, const std::string &fragPath, CompileBehavior compile = CompileBehavior::COMPILE_ON_CONSTRUCT)
        : ShaderProgram(name, asset<std::string>(vertPath)->c_str(), asset<std::string>(geomPath)->c_str(), asset<std::string>(fragPath)->c_str(), compile == CompileBehavior::COMPILE_ON_CONSTRUCT),
        vertCode(vertPath), fragCode(fragPath), geomCode(geomPath)
    {}

    void use() override
    {
        if (vertCode.hasReloaded()
            || (fragCode.isSet() && fragCode.hasReloaded())
            || (geomCode.isSet() && geomCode.hasReloaded())
            || compileFinishTime < ShaderDefinitions::global().lastEditTime || compileFinishTime < definitions.lastEditTime
            || !compiled_)
        {
            glDeleteProgram(programId);
            compile(
                vertCode->c_str(),
                fragCode.isSet() ? fragCode->c_str() : nullptr,
                geomCode.isSet() ? geomCode->c_str() : nullptr
            );
        }
        ShaderProgram::use();
    }

};


#endif
