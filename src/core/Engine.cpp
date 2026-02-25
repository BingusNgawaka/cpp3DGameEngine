#include "../../include/core/Engine.hpp"

EventManager Engine::events;
ShaderManager Engine::shader;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Engine::init(int width, int height, char* title){
	W = width;
	H = height;
	keyboard.setEventManager(&events);
	// initialize GLFW
	glfwInit();
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	glfwMakeContextCurrent(window);

	// Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
    }


	// glfw func callbacks
	glfwSetKeyCallback(window, keyboard.key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	events.registerEventCallback(
		std::to_string(GLFW_KEY_ESCAPE), 
		[&](float deltaTime){glfwSetWindowShouldClose(window, GLFW_TRUE);}
	);
	try {
        // shader init
        shader = ShaderManager(
            "../assets/shaders/vertex/shader.vert",
            "../assets/shaders/fragment/shader.frag"
        );

		GLuint programID = shader.getShaderID();

		glUseProgram(programID);
		shader.sendFloat("MATERIALSHININESS", 0.5f);
		std::cout << "Shader Program ID: " << programID << std::endl;
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cerr << "OpenGL Error AFTER glUseProgram: " << error << std::endl;
		}

        // depth stuff
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
		glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

		camera.position = glm::vec3(0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		events.triggerEvent("init", 0);



    } catch (const std::exception& e) {
        std::cerr << "Error initializing OpenGL resources: " << e.what() << std::endl;
        // Handle the failure, throw an exception, or return an error code
        return;
    }
}
int Engine::main(){
	float deltaTime = 0.0f;
	float lastTime = 0.0f;

	camera.matrixCalc(&shader, W, H, false); 
	glfwSwapBuffers(window);
	while (!glfwWindowShouldClose(window)){
		glUseProgram(shader.getShaderID());
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cerr << error << std::endl;
		}
	

		camera.matrixCalc(&shader, W, H, false); 
		mouse.update(window);
		this->update(deltaTime);
		this->draw(deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
void Engine::draw(float deltaTime){
	events.triggerEvent("draw", deltaTime);
}
void Engine::update(float deltaTime){
	events.triggerEvent("update", deltaTime);
}
