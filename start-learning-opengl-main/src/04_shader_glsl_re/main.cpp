#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec2 aPos;\n" // 位置变量的属性位置值为 0
                                 "out vec4 vertexColor;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n" // 注意我们如何把一个 vec3 作为 vec4 的构造器的参数
                                  "  vertexColor = vec4(0.5f, 1.0f, 0.1f, 1.0f);"                                                       //  "   gl_PointSize = 10.0f;"
                                 "}\0";

const char *fragmentShaderSource1 = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec4 vertexColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   vec4 color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   // 对颜色使用rgba，或是对纹理坐标使用stpq访问相同的分量
                                   "   FragColor = vertexColor;\n"
                                   "}\n\0";

const char *fragmentShaderSource2 = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "uniform vec4 ourColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = ourColor;\n"
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

  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

  float vertices[] = {
      -0.75f, -0.5f,
      -0.5f, 0.5f,
      -0.25f,  -0.5f, 

      0.25f, -0.5f, 
      0.5f, 0.5f, 
      0.75f,  -0.5f,
  };

  float vertices1[] = {
      -0.75f, -0.5f, 0.0f,
      -0.5f, 0.5f, 0.0f,
      -0.25f,  -0.5f, 0.0f,
  };

  float vertices2[] = {
      0.25f, -0.5f, 0.0f,
      0.5f, 0.5f, 0.0f,
      0.75f,  -0.5f, 0.0f,
  };

  /*
    VBO是 用来存储顶点数据的 内存缓冲，它会在GPU内存（通常被称为显存）中储存大量顶点。
    VAO是 用来存储状态配置的
  */
 // 使用两个 VAO 和 VBO绘制两个三角形
  unsigned int VBO[2], VAO[2];
  glGenBuffers(2, VBO); // 生成缓冲对象
  glGenVertexArrays(2, VAO); // 生成顶点数组对象

  glBindVertexArray(VAO[0]);             // 绑定顶点数组对象到目标上
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // 绑定缓冲对象到目标上

  // 将顶点数据复制到缓冲对象中
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 设置顶点属性指针
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAO[1]);             // 绑定顶点数组对象到目标上
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]); // 绑定缓冲对象到目标上

  // 将顶点数据复制到缓冲对象中
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 设置顶点属性指针
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(float) * 6));  // 从缓冲的第9个点开始读取数据
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
  unsigned int fragmentShader[2];
  fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER); // 创建着色器对象

  // 附加着色器源码到着色器对象上
  glShaderSource(fragmentShader[0], 1, &fragmentShaderSource1, NULL);
  // 编译着色器
  glCompileShader(fragmentShader[0]);

  // 检查编译是否成功
  glGetShaderiv(fragmentShader[0], GL_COMPILE_STATUS, &success);
  if (!success)
  {
    // 获取错误信息
    glGetShaderInfoLog(fragmentShader[0], 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER); // 创建着色器对象
  // 附加着色器源码到着色器对象上
  glShaderSource(fragmentShader[1], 1, &fragmentShaderSource2, NULL);
  // 编译着色器
  glCompileShader(fragmentShader[1]);

  // 检查编译是否成功
  glGetShaderiv(fragmentShader[1], GL_COMPILE_STATUS, &success);
  if (!success)
  {
    // 获取错误信息
    glGetShaderInfoLog(fragmentShader[1], 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // 创建着色器程序
  unsigned int shaderProgram[2];
  shaderProgram[0] = glCreateProgram(); // 创建着色器程序对象

  // 将着色器附加到程序对象上
  glAttachShader(shaderProgram[0], vertexShader);
  glAttachShader(shaderProgram[0], fragmentShader[0]);
  // 链接着色器程序
  glLinkProgram(shaderProgram[0]);

  // 检查链接是否成功
  glGetProgramiv(shaderProgram[0], GL_LINK_STATUS, &success);
  if (!success)
  {
    // 获取错误信息
    glGetProgramInfoLog(shaderProgram[0], 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n"
              << infoLog << std::endl;
  }

  // 激活着色器程序
  glUseProgram(shaderProgram[0]);

  shaderProgram[1] = glCreateProgram(); // 创建着色器程序对象

  // 将着色器附加到程序对象上
  glAttachShader(shaderProgram[1], vertexShader);
  glAttachShader(shaderProgram[1], fragmentShader[1]);
  // 链接着色器程序
  glLinkProgram(shaderProgram[1]);

  // 检查链接是否成功
  glGetProgramiv(shaderProgram[1], GL_LINK_STATUS, &success);
  if (!success)
  {
    // 获取错误信息
    glGetProgramInfoLog(shaderProgram[1], 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n"
              << infoLog << std::endl;
  }

  // 激活着色器程序
  glUseProgram(shaderProgram[1]);

  // 删除着色器对象
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader[0]);
  glDeleteShader(fragmentShader[1]);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 设置绘制模式为线框模式

  int vertexColorLocation = glGetUniformLocation(shaderProgram[1], "ourColor");

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // 渲染指令
    // ...
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram[0]); // 激活着色器程序
    glBindVertexArray(VAO[0]);      // 绑定顶点数组对象
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 绘制矩形
    glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制三角形
    // glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制三角形
    // glDrawArrays(GL_TRIANGLES, 3, 3); // 绘制三角形

    float timeValue = glfwGetTime();
    float greenValue = sin(timeValue) / 2.0f + 0.5f;
    float redValue = cos(timeValue) / 2.0f + 0.5f;
    glUseProgram(shaderProgram[1]); // 激活着色器程序
    glUniform4f(vertexColorLocation, redValue, greenValue, 0.0f, 1.0f);
    glBindVertexArray(VAO[1]);      // 绑定顶点数组对象
    glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制三角形

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
  glDeleteVertexArrays(1, &VAO[0]);
  glDeleteVertexArrays(1, &VAO[1]);
  glDeleteBuffers(1, &VBO[0]);
  glDeleteBuffers(1, &VBO[1]);
  glDeleteProgram(shaderProgram[0]);
  glDeleteProgram(shaderProgram[1]);

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}