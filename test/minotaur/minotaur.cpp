#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION
#define GLAD_GL_IMPLEMENTATION

#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>

// Shader source code
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;

void main()
{    
    color = spriteColor * texture(image, TexCoords);
}
)";

// Sprite Renderer Class
class SpriteRenderer {
public:
    unsigned int quadVAO;
    unsigned int shaderProgram;
    
    SpriteRenderer() {
        initRenderData();
        compileShaders();
    }
    
    ~SpriteRenderer() {
        glDeleteVertexArrays(1, &quadVAO);
    }
    
    void drawSprite(unsigned int textureID, glm::vec2 position, glm::vec2 size, float rotate = 0.0f, glm::vec4 color = glm::vec4(1.0f)) {
        // Prepare transformations
        glUseProgram(shaderProgram);
        glm::mat4 model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(position, 0.0f));
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
        model = glm::scale(model, glm::vec3(size, 1.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform4fv(glGetUniformLocation(shaderProgram, "spriteColor"), 1, glm::value_ptr(color));
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    
private:
    void compileShaders() {
        // Vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        checkCompileErrors(vertexShader, "VERTEX");
        
        // Fragment Shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        checkCompileErrors(fragmentShader, "FRAGMENT");
        
        // Shader Program
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        checkCompileErrors(shaderProgram, "PROGRAM");
        
        // Delete shaders as they're linked into our program and no longer necessary
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        // Set up projection matrix
        glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1i(glGetUniformLocation(shaderProgram, "image"), 0);
    }
    
    void initRenderData() {
        unsigned int VBO;
        float vertices[] = { 
            // pos      // tex
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 
            
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };
        
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &VBO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glBindVertexArray(quadVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    
    void checkCompileErrors(unsigned int object, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(object, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(object, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        } else {
            glGetProgramiv(object, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(object, 1024, NULL, infoLog);
                std::cout << "ERROR::Shader: Link-time error: Type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

// Sprite Sheet Animation class
class SpriteAnimation {
public:
    // Single frame data
    struct Frame {
        float x;      // x position in sprite sheet
        float y;      // y position in sprite sheet
        float width;  // width of the frame
        float height; // height of the frame
    };
    
    std::vector<Frame> frames;
    unsigned int textureID;
    float frameTime; // Time per frame in seconds
    float timer;     // Current timer
    int currentFrame;
    bool loop;
    glm::vec2 spriteSize; // Size to render the sprite
    
    SpriteAnimation(unsigned int texture, std::vector<Frame> framesList, float frameDuration, glm::vec2 size, bool looping = true)
        : textureID(texture), frames(framesList), frameTime(frameDuration), 
          timer(0.0f), currentFrame(0), loop(looping), spriteSize(size) { }
    
    void update(float dt) {
        timer += dt;
        if (timer >= frameTime) {
            timer = 0.0f;
            currentFrame++;
            
            if (currentFrame >= frames.size()) {
                if (loop) {
                    currentFrame = 0;
                } else {
                    currentFrame = frames.size() - 1;
                }
            }
        }
    }
    
    void draw(SpriteRenderer& renderer, glm::vec2 position) {
        Frame frame = frames[currentFrame];
        
        // Set texture coordinates based on current frame
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        // Draw the current frame
        renderer.drawSprite(textureID, position, spriteSize);
    }
};

// Texture loading function
unsigned int loadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
    } else {
        std::cout << "Failed to load texture: " << path << std::endl;
        stbi_image_free(data);
    }
    
    return textureID;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Minotaur Animation", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    // Initialize GLAD
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // OpenGL configuration
    glViewport(0, 0, 800, 600);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Load sprite sheet
    unsigned int spriteSheet = loadTexture("C:\\tsg2\\test\\minotaur\\Minotaur_1\\Attack.png");
    
    // Create sprite renderer
    SpriteRenderer renderer;
    
    // Define animation frames for the minotaur
    // Assuming your sprite sheet has 5 frames side by side
    // You'll need to adjust these values based on your actual sprite sheet
    std::vector<SpriteAnimation::Frame> minotaurFrames;
    
    // Let's assume each sprite is 64x64 pixels in a 320x64 sprite sheet (5 frames)
    float frameWidth = 64.0f;
    float frameHeight = 64.0f;
    float sheetWidth = 320.0f;  // total width of sprite sheet
    
    // Calculate texture coordinates for each frame
    for (int i = 0; i < 5; i++) {
        SpriteAnimation::Frame frame;
        frame.x = (i * frameWidth) / sheetWidth;
        frame.y = 0.0f;
        frame.width = frameWidth / sheetWidth;
        frame.height = 1.0f;  // Using the full height
        minotaurFrames.push_back(frame);
    }
    
    // Create the animation with 0.2 seconds per frame
    SpriteAnimation minotaurAnimation(spriteSheet, minotaurFrames, 0.2f, glm::vec2(128.0f, 128.0f), true);
    
    // Frame timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    // Main game loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Process input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        
        // Update animation
        minotaurAnimation.update(deltaTime);
        
        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw animation
        minotaurAnimation.draw(renderer, glm::vec2(336, 236));  // Center the sprite
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Clean up
    glfwTerminate();
    return 0;
}