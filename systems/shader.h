#pragma once
#include <fstream>
#include <string>

class Shader {
public:
    Shader(const std::string& vertex, const std::string& fragment);

    [[nodiscard]] int getUniformLocation(const std::string& name) const;
    [[nodiscard]] int getProgram() const;

    /**
     * @brief Meant to be used with gl uniform functions
     * @tparam FUNC The type of function pointer
     * @tparam ARGS What types are the arguments
     * @param func The opengl uniform function to call
     * @param args The arguments passed into the funtion
     */
    template<typename FUNC, typename... ARGS>
    void setUniformValue(FUNC func, ARGS... args) const {
        bind();
        func(std::forward<ARGS>(args)...);
    }

    void bind() const;

    ~Shader();

private:
    int programId_;

    // shading
    static std::string getShaderString(const std::string& filePath);

    static unsigned int compileShader(unsigned int type, const std::string& source);

    static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
};
