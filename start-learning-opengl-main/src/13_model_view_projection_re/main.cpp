#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tool/shader.h>
#include <geometry/BoxGeometry.h>
#include <geometry/PlaneGeometry.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <tool/stb_image.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
std::string Shader::dirName;

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

using namespace std;

int main(int argc, char *argv[])
{
  Shader::dirName = argv[1];
  glfwInit();
  // 设置主要和次要版本
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // 创建窗口对象
  GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
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
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
  // glDepthFunc(GL_LESS);

  // 注册窗口变化监听
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  Shader ourShader("./shader/vertex.glsl", "./shader/fragment.glsl");

  PlaneGeometry planeGeometry(1.0, 1.0);
  BoxGeometry boxGeometry(1.0, 1.0, 1.0);

  // 生成纹理
  unsigned int texture1, texture2;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  // 设置环绕和过滤方式
  float borderColor[] = {0.3f, 0.1f, 0.7f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // 图像y轴翻转
  stbi_set_flip_vertically_on_load(true);

  // 加载图片
  int width, height, nrChannels;
  unsigned char *data = stbi_load("./static/texture/container.jpg", &width, &height, &nrChannels, 0);

  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  stbi_image_free(data);

  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  // 设置环绕和过滤方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // 加载图片
  data = stbi_load("./static/texture/awesomeface.png", &width, &height, &nrChannels, 0);

  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  stbi_image_free(data);
  ourShader.use();
  ourShader.setInt("texture1", 0);
  ourShader.setInt("texture2", 1);

  float factor = 0.0;

  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),
      glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),
      glm::vec3(-1.3f, 1.0f, -1.5f)};

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // 渲染指令
    // ...
    glClearColor(25.0 / 255.0, 25.0 / 255.0, 25.0 / 255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ourShader.use();

    factor = glfwGetTime();
    ourShader.setFloat("factor", -factor * 0.3);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));                                                       // 观察矩阵 平移 z轴
    projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f); // 投影矩阵 透视投影
    ourShader.setMat4("view", view);
    ourShader.setMat4("projection", projection);

    for (unsigned int i = 0; i < 10; i++)
    {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(1.0, 1.0, 1.0));            // 模型矩阵 旋转 55度  x轴                                                       // 模型矩阵 平移 z轴
      
      ourShader.setMat4("model", model);

      glBindVertexArray(boxGeometry.VAO);

      glDrawElements(GL_TRIANGLES, boxGeometry.indices.size(), GL_UNSIGNED_INT, 0);
      // glDrawElements(GL_POINTS, planeGeometry.indices.size(), GL_UNSIGNED_INT, 0);
      // glDrawElements(GL_LINE_LOOP, planeGeometry.indices.size(), GL_UNSIGNED_INT, 0);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  boxGeometry.dispose();
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