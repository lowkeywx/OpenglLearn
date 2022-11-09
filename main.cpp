#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <random>
#include <cmath>

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

    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(-1.0,1.0);

    std::default_random_engine generator1(22222);
    std::uniform_real_distribution<float> distribution1(-1.0,1.0);

    const double pi = std::acos(-1);
    const double du = 2* pi / 360;
    while (!glfwWindowShouldClose(windows)) {

        [windows](){
            if(glfwGetKey(windows, GLFW_KEY_ENTER) == GLFW_PRESS) {
                glfwSetWindowShouldClose(windows, true);
            }
        }();

        glClearColor(1.0, 1.0, 1.0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        //glUseProgram(program);

        glColor3f(1, 0, 0);
//        glLineWidth(10);
//        glBegin(GL_LINES);
//        glVertex2f(0,0);
//        glVertex2f(0.5,0.5);
//        glEnd();
        //设置GL_FILL和GL_POINT看似效果相同，都是填充
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // 每个点的颜色设置不一样，效果渐变，三角形
//        glBegin(GL_TRIANGLES);
//        glColor3f(1, 0, 0);
//        glVertex2f(-0.5, 0);
//        glColor3f(0, 1, 0);
//        glVertex2f(0, 0.5);
//        glColor3f(0, 0, 1);
//        glVertex2f(0.5, 0);
//        glEnd();
            //直线
//        glColor3f(0, 0, 1);
//        glBegin(GL_LINE_STRIP);
//        glVertex2f(-0.5, -0.5);
//        glVertex2f(0,0);
//        glVertex2f(0.5, -0.5);
//        glEnd();
//          //直线
//        glColor3f(0, 1, 1);
//        glBegin(GL_LINE_LOOP);
//        glVertex2f(-0.8, -0.8);
//        glVertex2f(0.3,0.3);
//        glVertex2f(0, 0);
//        glEnd();
            //五角星
//        GLfloat xPos = distribution(generator);
//        GLfloat yPos = distribution1(generator1);
//        glBegin(GL_LINE_LOOP);
//        glVertex2f(xPos, yPos + 0.5);
//        glVertex2f(xPos-0.3, yPos-0.5);
//        glVertex2f(xPos + 0.4, yPos);
//        glVertex2f(xPos-0.4, yPos);
//        glVertex2f(xPos + 0.3, yPos-0.5);
//        glEnd();

        //直径1的，中心(0，0)的圆,带花边, 因为求出来的线段很多
        glLineWidth(10);
        // GL_LINE_LOOP 首尾相连，GL_LINE_STRIP最后一个点和第一个点会出现空隙
        glBegin(GL_LINE_LOOP);
        LOG(INFO) << "begin draw yuan";

        for (int i = 0; i < 360; ++i) {
            if (i%5 == 0) {
                auto x = std::sin(du * i);
                auto y = std::cos(du * i);
                //LOG(INFO) << "X=" << x << " Y=" << y;
                glVertex2f(x, y);
            }
        }
        LOG(INFO) << "end draw yuan";

        glEnd();

        glFlush();
        // 不交换背景色不会变化， 默认黑色
        glfwSwapBuffers(windows);
        glfwPollEvents();
    }

    glfwTerminate();
    return GL_LEANING_ERROR_CODE_OK;
}
