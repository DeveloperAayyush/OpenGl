#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

static uint32_t CompileShader(int type, const std::string& shader)
{
    uint32_t id = 0;

    id = glCreateShader(type);
    const char* src = shader.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // error handling code snippet
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "\n Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex shader" : "Fragment shader")<<" with error --" ;
        std::cout << message << std::endl;
        glDeleteShader(id);
    }

    return id;
}

static int createShader(const std::string& VertexShader, const std::string& FragmentShader)
{
    uint32_t program = glCreateProgram();

    uint32_t vs = CompileShader(GL_VERTEX_SHADER, VertexShader);
    uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, FragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);

    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;

}

struct Shaders
{
    std::string VertexShader;
    std::string FragmentShader;
};

static Shaders getShader(const std::string& path)
{
    std::ifstream file(path);

    enum ShadersType
    {
        NO_TYPE = -1,
        VERTEX_TYPE = 0,
        FRAGMENT_TYPE = 1
    };

    ShadersType shType = ShadersType::NO_TYPE;

    std::stringstream shadersArray[2];
    std::string line;
    while (getline(file, line))
    {
        if (line.find("#Shader") != std::string::npos)
        {
            if (line.find("Vertex") != std::string::npos)
                shType = ShadersType::VERTEX_TYPE;
            else
                shType = ShadersType::FRAGMENT_TYPE;
        }
        else
        {
            shadersArray[(int)shType] << line <<'\n';
        }
    }

    return { shadersArray[ShadersType::VERTEX_TYPE].str() , shadersArray[ShadersType::FRAGMENT_TYPE].str()};

}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Aayyush's Game Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "\n GlewInit is not Ok";

    std::cout << glGetString(GL_VERSION);

    float positions[6] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    unsigned int bufferId;
    glCreateBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);

    //Aayyush: Vertex Attribute stuff lecture 5
    glEnableVertexAttribArray(0);       //Aayyush: It is necessary to enable the Vertex Attribute before we can use the vertex attributes.
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), 0);


    Shaders sh = getShader("Res/Shaders/Basic.shader");

    int shaderId = createShader(sh.VertexShader, sh.FragmentShader);
    glUseProgram(shaderId);

    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
    


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}