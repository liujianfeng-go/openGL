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
                                                                                         //  "   gl_PointSize = 10.0f;"
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
  // glEnable(GL_PROGRAM_POINT_SIZE); // 开启点的大小设置

  // 注册窗口变化监听
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  float vertices[] = {
      -0.75f, -0.5f, 0.0f,
      -0.5f, 0.5f, 0.0f,
      -0.25f,  -0.5f, 0.0f,

      0.25f, -0.5f, 0.0f,
      0.5f, 0.5f, 0.0f,
      0.75f,  -0.5f, 0.0f,
  };

  /*
    VBO是 用来存储顶点数据的 内存缓冲，它会在GPU内存（通常被称为显存）中储存大量顶点。
    VAO是 用来存储状态配置的
  */

  unsigned int VBO, VAO;
  glGenBuffers(1, &VBO); // 生成缓冲对象
  glGenVertexArrays(1, &VAO); // 生成顶点数组对象

  glBindVertexArray(VAO);             // 绑定顶点数组对象到目标上

  glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定缓冲对象到目标上

  // 将顶点数据复制到缓冲对象中
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 设置顶点属性指针
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0); // 解绑顶点数组对象

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

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 设置绘制模式为线框模式

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // 渲染指令
    // ...
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram); // 激活着色器程序
    glBindVertexArray(VAO);      // 绑定顶点数组对象

    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 绘制矩形

    glDrawArrays(GL_TRIANGLES, 0, 6); // 绘制三角形
    // glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制三角形
    // glDrawArrays(GL_TRIANGLES, 3, 3); // 绘制三角形
    
    /*
      GL_POINTS      // 绘制一系列点
      GL_LINE_STRIP  // 绘制一个线条
      GL_LINE_LOOP   // 绘制一个线条，首尾相连
      GL_LINES       // 绘制一系列单独线段。每两个点作为端点，线段之间不连接
      GL_LINE_STRIP_ADJACENCY // 绘制一系列连接的线段。前两个点作为第一个线段的端点，之后每两个点作为一个线段的端点，线段之间不连接
      GL_LINES_ADJACENCY      // 绘制一系列单独线段。每四个点作为两个线段的端点，线段之间不连接
      GL_TRIANGLE_STRIP       // 绘制一个三角带
      GL_TRIANGLE_FAN         // 绘制一个三角扇
      GL_TRIANGLES            // 绘制一系列单独三角形
      GL_TRIANGLE_STRIP_ADJACENCY // 绘制一个连接的三角带
    */

    glBindVertexArray(0); // 解绑顶点数组对象

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // 释放资源
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

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