#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define LOG(LEVEL) LogWriter().stream << "[" << #LEVEL << "] "

struct LogWriter
{
    LogWriter() = default;
    ~LogWriter() {
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        //auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::cout << "[" << time << "] "<< stream.str() << std::endl;
    }

    std::stringstream stream;
};

enum GL_LEANING_ERROR_CODE{
    GL_LEANING_ERROR_CODE_OK,
    GL_LEANING_ERROR_CODE_GLFW_INIT_FAILED,
    GL_LEANING_ERROR_CODE_CREATE_WINDOW_FAIL,
    GL_LEANING_ERROR_CODE_LOAD_ADDRESS_FAIL,
    GL_LEANING_ERROR_CODE_COMPILE_SHADER_FAIL,
    GL_LEANING_ERROR_CODE_PROGRAM_LINK_FAIL
};

#define ERROR_RETURN(CODE) -CODE

const GLchar * const vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const GLchar * const fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";

static GLuint createProgram(const GLchar* vs, const GLchar* fs) {

    GLuint vShader  = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vs, nullptr);
    glCompileShader(vShader);
    GLint compileResult = 0;
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &compileResult);
    GLchar errorMsgBuffer[1024] = {0};
    GLsizei errorMsgBufferSize = 0;
    if (compileResult != GL_TRUE) {
        glGetShaderInfoLog(vShader, 1024, &errorMsgBufferSize, errorMsgBuffer);
        LOG(ERROR) << errorMsgBuffer;
        glfwTerminate();
        return ERROR_RETURN(GL_LEANING_ERROR_CODE_COMPILE_SHADER_FAIL);
    }
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fs, nullptr);
    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &compileResult);
    if (compileResult != GL_TRUE) {
        glGetShaderInfoLog(vShader, 1024, &errorMsgBufferSize, errorMsgBuffer);
        LOG(ERROR) << errorMsgBuffer;
        glfwTerminate();
        return ERROR_RETURN(GL_LEANING_ERROR_CODE_COMPILE_SHADER_FAIL);
    }

    auto program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
    GLint linkResult = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
    if (linkResult != GL_TRUE) {
        glGetProgramInfoLog(linkResult, 1024, &errorMsgBufferSize, errorMsgBuffer);
        LOG(ERROR) << errorMsgBuffer;
        return ERROR_RETURN(GL_LEANING_ERROR_CODE_PROGRAM_LINK_FAIL);
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    return program;
}

int main() {

    if (!glfwInit()) {
        LOG(ERROR) << "glfw init failed.";
        return ERROR_RETURN(GL_LEANING_ERROR_CODE_GLFW_INIT_FAILED);
    }

    auto windows = glfwCreateWindow(800,600, "my opengl windows", nullptr, nullptr);
    if (!windows) {
        LOG(ERROR) << "create windows failed!";
        glfwTerminate();
        return ERROR_RETURN(GL_LEANING_ERROR_CODE_CREATE_WINDOW_FAIL);
    }

    glfwMakeContextCurrent(windows);
    // 必须在glfwMakeContextCurrent执行， 否则会崩溃
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG(ERROR) << "load open gl function address failed!";
        glfwTerminate();
        return ERROR_RETURN(GL_LEANING_ERROR_CODE_LOAD_ADDRESS_FAIL);
    }

    glfwSetFramebufferSizeCallback(windows, [](GLFWwindow* window, int width, int height) {
        glViewport(0,0,width,height);
    });

    //auto program = createProgram(vertexShaderSource, fragmentShaderSource);

    while (!glfwWindowShouldClose(windows)) {

        [windows](){
            if(glfwGetKey(windows, GLFW_KEY_ENTER) == GLFW_PRESS) {
                glfwSetWindowShouldClose(windows, true);
            }
        }();

        glClearColor(1.0, 1.0, 1.0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        //glUseProgram(program);

        glLineWidth(10);
        glColor3f(1, 0, 0);
        glBegin(GL_LINES);
        glVertex2f(0,0);
        glVertex2f(0.5,0.5);
        glEnd();

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor3f(0, 1, 0);
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5, 0);
        glVertex2f(0, 0.5);
        glVertex2f(0.5, 0);
        glEnd();

        glColor3f(0, 0, 1);
        glBegin(GL_LINE_STRIP);
        glVertex2f(-0.5, -0.5);
        glVertex2f(0,0);
        glVertex2f(0.5, -0.5);
        glEnd();

        glFlush();
        // 不交换背景色不会变化， 默认黑色
        glfwSwapBuffers(windows);
        glfwPollEvents();
    }

    glfwTerminate();
    return GL_LEANING_ERROR_CODE_OK;
}
