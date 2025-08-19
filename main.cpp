#include <cstdio>
#include <iostream>
#include <string>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

void error_callback(int error, const char* description) {
    std::printf("Error with code'%d': %s\n", error, description);
}

void close_callback(GLFWwindow* window) {
    std::printf("user closing window");
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    const unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int lenght;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
        char message[lenght];
        glGetShaderInfoLog(id, lenght, &lenght, message);
        std::cout << "Failed to compile shader! \n" <<  message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vertexShaderId = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fragmentShaderId = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vertexShaderId);
    glAttachShader(program, fragmentShaderId);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    return program;
}

struct Vector2 {
    float x, y;
};

struct Color {
    float r, g, b, a;
};

struct Vertex {
    Vector2 position;
    Color color;
};

void drawTriangle(unsigned int buffer) {
    // // buffer id
    // unsigned int buffer;
    // // generate 1 buffer and assign the id into uint buffer ^
    // glGenBuffers(1, &buffer);
    // // I am going to work with this buffer. select it
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // // define all the data to use. Use STATIC for objects that are defined once and reused, use DYNAMIC for objects that are redefined multiple times and reused
    // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    /* define the position attribute, index is the index for the attrib, size is number of components per generic vertex attribute (1, 2, 3, 4)
     * type is the data type, normalized turned ranges from 0 to 255 into 0-1, stride is byte offset between consecutive generic vertex attributes
     * pointer is a const void* to the first component of the first generic vertex attribute */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, position));
    // enable the position vertexAttribute
    glEnableVertexAttribArray(0);
    // define the color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, color));
    // enable the color vertexAttribute
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

unsigned int defineTriangle(const std::array<Vertex, 3>& vertices) {
    // buffer id
    unsigned int buffer;
    // generate 1 buffer and assign the id into uint buffer ^
    glGenBuffers(1, &buffer);
    // I am going to work with this buffer. select it
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // define all the data to use. Use STATIC for objects that are defined once and reused, use DYNAMIC for objects that are redefined multiple times and reused
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    return buffer;
}

int main() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        std::printf("glfw initialization failed");
        exit(0);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window)
    {
        std::printf("window creation failed");
        exit(0);
    }

    glfwSetWindowCloseCallback(window, close_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);

    if (gladLoadGL(glfwGetProcAddress) == 0) {
        printf("Failed to initialize OpenGL context\n");
        exit(0);
    }

    glViewport(0, 0, 640, 480);

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // float positions[6] = {
    //     -0.5f, -0.5f,
    //     0.0f, 0.5f,
    //     0.5f, -0.5f
    // };

    // // buffer id
    // unsigned int buffer;
    // // generate 1 buffer and assign the id into uint buffer ^
    // glGenBuffers(1, &buffer);
    // // I am going to work with this buffer. select it
    // glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // // define all the data to use. Use STATIC for objects that are defined once and reused, use DYNAMIC for objects that are redefined multiple times and reused
    // glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    // /* define the position attribute, index is the index for the attrib, size is number of components per generic vertex attribute (1, 2, 3, 4)
    //  * type is the data type, normalized turned ranges from 0 to 255 into 0-1, stride is byte offset between consecutive generic vertex attributes
    //  * pointer is a const void* to the first component of the first generic vertex attribute */
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    // // enable the vertextAttribute
    // glEnableVertexAttribArray(0);

    // vertex shader ran on every vertex
    std::string vertexShader =
        "#version 330 core\n"
        "layout(location = 0) in vec2 position;\n"
        "layout(location = 1) in vec4 color;\n"
        "out vec4 v_Color;\n"
        "void main() {\n"
        "    gl_Position = vec4(position, 0.0, 1.0);\n"
        "    v_Color = color;\n"
        "}\n";

    // fragment shader ran on every pixel
    std::string fragmentShader =
        "#version 330 core\n"
        "in vec4 v_Color;\n"
        "out vec4 color;\n"
        "void main() {\n"
        "    color = v_Color;\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    std::array<Vertex, 3> vertices = {};

    vertices.at(0) = {{-0.5f, -0.5f}, {1, 0, 0, 1}};
    vertices.at(1) = {{0, 0.5f}, {0, 1, 0, 1}};
    vertices.at(2) = {{0.5f, -0.5f}, {0, 0, 1, 1}};

    const unsigned int triangleBuffer = defineTriangle(vertices);

    // main update loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        // draw the currently selected array
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        drawTriangle(triangleBuffer);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean uo things
    glDeleteProgram(shader);
    glfwDestroyWindow(window);
    glfwTerminate();
}
