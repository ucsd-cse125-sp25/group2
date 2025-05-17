#include "ui.hpp"

GLuint BaseUI::loadTexture(const char *path) {
  int width, height, nrChannels;
  unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
  if (!data) {
    std::cerr << "Failed to load texture" << std::endl;
    return 0;
  }

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);

  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
  return texture;
}

BaseUI::BaseUI(float x, float y, float width, float height, int zIndex,
               bool clickable, bool hoverable)
    : x(x), y(y), width(width), height(height), zIndex(zIndex),
      clickable(clickable), hoverable(hoverable) {
  isAnim = false;
  hovered = false;
  setupQuad();
}

BaseUI::BaseUI(float x, float y, float width, float height, int zIndex,
               AnimationInfo animInfo, bool clickable, bool hoverable)
    : x(x), y(y), width(width), height(height), zIndex(zIndex),
      clickable(clickable), hoverable(hoverable), animInfo(animInfo) {
  hovered = false;
  isAnim = true;
  setupQuad();
}

BaseUI::~BaseUI() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void BaseUI::setOnClick(std::function<void()> callback) {
  onClickCallback = callback;
}

void BaseUI::setTexture(GLuint texture) {
  if (texture == 0) {
    std::cerr << "failed to load texture into ui" << std::endl;
  }
  textureID = texture;
}

void BaseUI::setHoverTexture(GLuint texID) { hoverTextureID = texID; }

void BaseUI::setShader(unique_ptr<Shader> shader) {
  this->shader = std::move(shader);
}

void BaseUI::draw() {
  if (!shader) {
    std::cerr << "Warning: Shader not set, skipping render\n";
    return;
  }
  shader->use();

  GLuint tex = (hoverable && hovered && hoverTextureID && !animInfo.startAnim)
                   ? hoverTextureID
                   : textureID;

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);

  if (isAnim) {
    if (!hovered || animInfo.startAnim) {
      int frameX = animInfo.currentFrame % animInfo.cols;
      int frameY = animInfo.currentFrame / animInfo.cols;
      int flippedY = (animInfo.rows - 1) - frameY;

      glm::vec2 frameSize(animInfo.frameWidth, animInfo.frameHeight);
      glm::vec2 frameOffset(frameX * animInfo.frameWidth,
                            flippedY * animInfo.frameHeight);

      // std::cout << "width: " << frameX * animInfo.frameWidth << "height: " <<
      // flippedY * animInfo.frameHeight << std::endl;
      shader->setVec2("frameSize", frameSize);
      shader->setVec2("frameOffset", frameOffset);
    } else {
      shader->setVec2("frameSize", glm::vec2(1.0f, 1.0f));
      shader->setVec2("frameOffset", glm::vec2(0.0f, 0.0f));
    }
  } else {
    shader->setInt("texture1", 0);
  }

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void BaseUI::update(float mouseX, float mouseY, int winWidth, int winHeight,
                    float deltaTime) {
  float x_ndc = (2.0f * mouseX) / winWidth - 1.0f;
  float y_ndc = 1.0f - (2.0f * mouseY) / winHeight;

  bool isHovering = isHovered(x_ndc, y_ndc);
  if (hoverable) {
    // printf("x: %f y: %f \n", x_ndc, y_ndc);
    //  Hover state change detection
    if (isHovering && !hovered) {
      std::cout << "Hovering: True " << std::endl;
      hovered = true;
    } else if (!isHovering && hovered) {
      std::cout << "Hovering: false " << std::endl;
      hovered = false;
    }
  }

  // Click detection
  if (clickable && isHovering) {
    if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) ==
        GLFW_PRESS) {
      if (!isAnim)
        onClickCallback();
      if (isAnim) {
        play();
      }
    }
  }

  if (isAnim && animInfo.startAnim) {
    animInfo.animationTimer += deltaTime;

    if (animInfo.animationTimer >= animInfo.frameDuration) {
      animInfo.animationTimer -= animInfo.frameDuration;
      if (animInfo.currentFrame < animInfo.rows * animInfo.cols) {
        animInfo.currentFrame++;
      } else {
        onClickCallback();
      }
    }
  }
}

bool BaseUI::isHovered(float x_ndc, float y_ndc) {
  float halfW = width / 2.0f;
  float halfH = height / 2.0f;

  // NDC coordinates for quad edges
  float left = x - halfW;
  float right = x + halfW;
  float bottom = y - halfH;
  float top = y + halfH;
  return (x_ndc >= left && x_ndc <= right && y_ndc >= bottom && y_ndc <= top);
}

void BaseUI::play() { animInfo.startAnim = true; }

void BaseUI::setupQuad() {
  float halfW = width / 2.0f;
  float halfH = height / 2.0f;

  float vertices[] = {
      // posX   posY      texX texY
      x - halfW, y - halfH, 0.0f, 0.0f, // Bottom Left
      x + halfW, y - halfH, 1.0f, 0.0f, // Bottom Right
      x + halfW, y + halfH, 1.0f, 1.0f, // Top Right
      x - halfW, y + halfH, 0.0f, 1.0f  // Top Left
  };

  unsigned int indices[] = {0, 1, 2, 2, 3, 0};

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);
}
