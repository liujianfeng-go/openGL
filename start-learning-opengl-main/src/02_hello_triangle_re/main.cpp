#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n" // 位置变量的属性位置值为 0
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // 注意我们如何把一个 vec3 作为 vec4 的构造器的参数
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";

int main()
{
  glfwInit();
  // 设置主要和次要版本
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // 创建窗口对象
  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  // 设置视口
  glViewport(0, 0, 800, 600);

  // 注册窗口变化监听
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // 定义顶点数组
  float vertices[] = {
      -0.5f, -0.5f, 0.0f, // left
      0.5f, -0.5f, 0.0f,  // right
      0.0f, 0.5f, 0.0f    // top
  };

  // 创建顶点缓冲对象
  unsigned int VBO;
  glGenBuffers(1, &VBO); // 生成缓冲对象

  // 绑定缓冲对象
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定缓冲对象到目标上

  // 将顶点数据复制到缓冲对象中
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 创建顶点着色器
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER); // 创建着色器对象
  
  // 附加着色器源码到着色器对象上
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  // 编译着色器
  glCompileShader(vertexShader);
  
  // 检查编译是否成功
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    // 获取错误信息
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // 创建片段着色器
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // 创建着色器对象

  // 附加着色器源码到着色器对象上
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  // 编译着色器
  glCompileShader(fragmentShader);

  // 检查编译是否成功
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    // 获取错误信息
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // 创建着色器程序
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram(); // 创建着色器程序对象

  // 将着色器附加到程序对象上
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  // 链接着色器程序
  glLinkProgram(shaderProgram);

  // 检查链接是否成功
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    // 获取错误信息
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n"
              << infoLog << std::endl;
  }

  // 激活着色器程序
  glUseProgram(shaderProgram);

  // 删除着色器对象
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);


  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // 渲染指令
    // ...
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, 800, 600);
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}