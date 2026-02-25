#include "../include/core/Engine.hpp"
#include "../include/core/LightManager.hpp"
#include <string>
//#include <format>

Engine engine;
LightManager lightManager;
glm::vec3 pos(0.0f, 4.0f, 0.0f);
glm::vec3 lightCol(1.0f, 1.0f, 1.0f);
ShaderManager lineShader;



void printVec(const glm::vec3& vector) {
    std::cout << "X: " << vector.x << ", Y: " << vector.y << ", Z: " << vector.z << std::endl;
}
void print(std::string text){
	std::cout << text << std::endl;
}
glm::vec3 lerpVec3(const glm::vec3& start, const glm::vec3& end, float t) {
    t = glm::clamp(t, 0.0f, 1.0f); // Ensure t is in the [0, 1] range
    return (1.0f - t) * start + t * end;
}
float lerpFloat(const float start, const float end, float t) {
    t = glm::clamp(t, 0.0f, 1.0f); // Ensure t is in the [0, 1] range
    return (1.0f - t) * start + t * end;
}
void look(GLFWwindow* window, double xpos, double ypos){
	float sens = 0.1f;
	double deltaX = xpos - engine.W/2;
	double deltaY = ypos - engine.H/2;
	deltaX *= sens;
	deltaY *= sens;

	engine.camera.yaw += deltaX;
	engine.camera.pitch -= deltaY;

	//pitch constraints
	if (engine.camera.pitch < -89.0f) {
		engine.camera.pitch = -89.0f;
	} else if (engine.camera.pitch > 89.0f) {
		engine.camera.pitch = 89.0f;
	}

	glfwSetCursorPos(window, engine.W/2, engine.H/2);
}
void setFullscreen(int i){
	int monitorCount;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
	int monitorX, monitorY, monitorWidth, monitorHeight;
	glfwGetMonitorPos(monitors[i], &monitorX, &monitorY);
	const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);
	monitorWidth = mode->width;
	monitorHeight = mode->height;
	std::cout <<"monx: "<< monitorX << std::endl;
	std::cout <<"mony: "<< monitorY << std::endl;
	std::cout <<"monw: "<< monitorWidth << std::endl;
	std::cout <<"monh: "<< monitorHeight << std::endl;
	// Check if the cursor is within the monitor's bounds
	glfwSetWindowMonitor(engine.window, monitors[i], 0, 0, monitorWidth, monitorHeight, mode->refreshRate);
}
class Collider{
	public:
		std::string name;
		std::string lastCollisionObj = "NULL";
		std::pair<glm::vec3, glm::vec3> MaxMins;
		glm::vec3 mtv;
		Collider(std::string inputName, std::pair<glm::vec3, glm::vec3> inputMaxMins);
		void setMaxMinsFromMesh(std::array<glm::vec2, 3> meshMaxMins);
		void setLastCollisionObj(std::string name, glm::vec3 MTV);
};
Collider::Collider(std::string inputName, std::pair<glm::vec3, glm::vec3> inputMaxMins){
	name = inputName;
	MaxMins = inputMaxMins;
}
void Collider::setMaxMinsFromMesh(std::array<glm::vec2, 3> meshMaxMins){
	std::pair<glm::vec3, glm::vec3> outputMaxMins;
	outputMaxMins.second = glm::vec3(meshMaxMins[0].x, meshMaxMins[1].x, meshMaxMins[2].x);
	outputMaxMins.first = glm::vec3(meshMaxMins[0].y, meshMaxMins[1].y, meshMaxMins[2].y);
	MaxMins = outputMaxMins;
}
void Collider::setLastCollisionObj(std::string name, glm::vec3 MTV){
	lastCollisionObj = name;
	mtv = MTV;
}
class CollisionManager{
	public:
		std::map<std::string, Collider*> colliders;
		void addCollider(std::string name, std::pair<glm::vec3, glm::vec3> MaxMins);
		void checkCollisions();
		void removeCollider(std::string name);
		std::pair<glm::vec3, glm::vec3> convertMeshToCollider(std::array<glm::vec2, 3> meshMaxMins);
		glm::vec3 calcMTV(std::pair<glm::vec3, glm::vec3> box1,std::pair<glm::vec3, glm::vec3> box2);
};
std::pair<glm::vec3, glm::vec3> CollisionManager::convertMeshToCollider(std::array<glm::vec2, 3> meshMaxMins){
	std::pair<glm::vec3, glm::vec3> outputMaxMins;
	outputMaxMins.second = glm::vec3(meshMaxMins[0].x, meshMaxMins[1].x, meshMaxMins[2].x);
	outputMaxMins.first = glm::vec3(meshMaxMins[0].y, meshMaxMins[1].y, meshMaxMins[2].y);
	return outputMaxMins;
}
void CollisionManager::addCollider(std::string name, std::pair<glm::vec3, glm::vec3> MaxMins){
	Collider* collider = new Collider(name, MaxMins);
	colliders[name] = collider;
}
void CollisionManager::removeCollider(std::string name){
	colliders.erase(name);
}
glm::vec3 CollisionManager::calcMTV(std::pair<glm::vec3, glm::vec3> box1,std::pair<glm::vec3, glm::vec3> box2){
	// Calculate overlap along each axis
    float overlapX = std::min(box1.first.x, box2.first.x) - std::max(box1.second.x, box2.second.x);
    float overlapY = std::min(box1.first.y, box2.first.y) - std::max(box1.second.y, box2.second.y);
    float overlapZ = std::min(box1.first.z, box2.first.z) - std::max(box1.second.z, box2.second.z);

    // Check for overlap along each axis
    if (overlapX > 0 && overlapY > 0 && overlapZ > 0) {
        // Determine the axis with the smallest overlap
        if (overlapX <= overlapY && overlapX <= overlapZ) {
            float sign = -(box1.first.x + box1.second.x) > (box2.first.x + box2.second.x) ? -1.0f : 1.0f;
            return glm::vec3(overlapX * sign, 0.0f, 0.0f);
        } else if (overlapY <= overlapZ) {
            float sign = -(box1.first.y + box1.second.y) > (box2.first.y + box2.second.y) ? -1.0f : 1.0f;
            return glm::vec3(0.0f, overlapY * sign, 0.0f);
        } else {
            float sign = -(box1.first.z + box1.second.z) > (box2.first.z + box2.second.z) ? -1.0f : 1.0f;
            return glm::vec3(0.0f, 0.0f, overlapZ * sign);
        }
    } else {
        // No overlap, return zero vector
        return glm::vec3(0.0f);
    }
}
void CollisionManager::checkCollisions(){
	for(const auto& obj1: colliders){
		std::map<std::string, Collider*> tempColliders(colliders);
		tempColliders.erase(obj1.first);
		colliders[obj1.first]->setLastCollisionObj("NULL", glm::vec3(0));
		for(const auto& obj2: tempColliders){
			std::pair<glm::vec3, glm::vec3> box1 = obj1.second->MaxMins;
			std::pair<glm::vec3, glm::vec3> box2 = obj2.second->MaxMins;
			/* debug print
			std::cout << "chimen:" << std::endl;
			printVec(box1.first);
			printVec(box1.second);
			std::cout << "bingform:" << std::endl;
			printVec(box2.first);
			printVec(box2.second);
			*/
			glm::vec3 mtv = calcMTV(box1, box2);
			if(mtv != glm::vec3(0.0f)){
				colliders[obj1.first]->setLastCollisionObj(obj2.first, mtv);
				colliders[obj2.first]->setLastCollisionObj(obj1.first, mtv);
				engine.events.triggerEvent(obj1.first, 0);
				engine.events.triggerEvent(obj2.first, 0);
			}
		}
	}
}
CollisionManager collisionManager;

class Line{
	private:
		GLuint vao, vbo;
	public:
		Line(std::vector<float>& startEndPoints, int numOfVerts);
		int count;
		void draw();
		void setLineWidth(float width);
};
Line::Line(std::vector<float>& startEndPoints, int numOfVerts){
	count = numOfVerts;
	glUseProgram(lineShader.getShaderID());
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	print("size:");
	print(to_string(count));
	print(to_string(startEndPoints.size()));
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1,&vbo);
	print(to_string(vao));
	print(to_string(vbo));
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, startEndPoints.size() * sizeof(float), startEndPoints.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float),(void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void Line::draw(){
	glUseProgram(lineShader.getShaderID());
	engine.camera.matrixCalc(&lineShader, engine.W, engine.H, true);

	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, count);
	print(to_string(vao));
	glBindVertexArray(0);
}
void Line::setLineWidth(float width){
	glLineWidth(width);
}
class Chicken{
	private:
		float speed = 500;
		float groundSpeed = 500;
		float defSpeed = 300;
		float yFric = 0.98f;
		float mass = 0.5f;
		float fric = 0.9f;
		float floorFric = 0.9f;
		float defFric = 0.96f;
		glm::vec3 vel, accel, forces;
		glm::vec3 slideVec = glm::vec3(0);
		bool grounded = false;
		bool canSlide = false;
		float camDist = 2.0f;
		float defCamDist = 2.0f;
		float aimDist = 0.6f;
		float grav = 9.0f;
		float aimSpeed = 200;
		float currSpeedMod = 0;
		
	public:
		glm::vec3 pos;
		Chicken(){
			vel = glm::vec3(0,0,0);
			accel = glm::vec3(0,0,0);
			forces = glm::vec3(0,0,0);
			pos = glm::vec3(0,10,0);
			std::string path = "../assets/resources/chickenV2";
			engine.meshes["chimen"] = Mesh::loadOBJ(path);
			engine.meshes["chimen"].scale(glm::vec3(0.05f,0.05f,0.05f));
			engine.meshes["chimen"].translate(pos);
			std::array<glm::vec2, 3> maxmin = engine.meshes["chimen"].getWorldSpaceMaxMins();
			collisionManager.addCollider("chimen", collisionManager.convertMeshToCollider(maxmin));	
			engine.events.registerEventCallback(
				"chimen",
				[](float dt){
				}
			);
		}
		void input(float deltaTime){
			float jump = 6000;
			float slide = 20000;
			float slideThreshold = 1.5f;
			glm::vec3 forward = engine.camera.dir;
			forward.y = 0;
			forward = normalize(forward);
			glm::vec3 right = engine.camera.right;
			right.y = 0;
			right = normalize(right);
			glm::vec3 left = -right;
			glm::vec3 back = -forward;
			glm::vec3 up = glm::vec3(0,1,0);
			glm::vec3 down = glm::vec3(0,-1,0);
			currSpeedMod = 0;
			if(engine.mouse.getButtonStates().right == "Down"){
				std::cout << "aim" << std::endl;
				if(camDist >= aimDist){
					camDist = lerpFloat(camDist, aimDist-1, 0.1f);
				}
			}else{
				camDist = lerpFloat(camDist, defCamDist, 0.1f);
			}
			if(grounded){
				speed = groundSpeed;
			}else{
				speed = defSpeed;
			}
			speed -= currSpeedMod;
			float currVel = glm::length(vel);
			//std::cout << currVel << std::endl;
			if(engine.keyboard.isKeyPressed(GLFW_KEY_LEFT_CONTROL)){
				engine.meshes["chimen"].scale(glm::vec3(0.05f,0.025f,0.05f));
				if(grounded && canSlide){ if(currVel > slideThreshold){
						std::cout << "slid" << std::endl;
						addForce(slideVec*slide*deltaTime);	
						canSlide = false;
					}
				}
				if(grounded){
					speed = 0;
				}
				floorFric = 0.99f;
				defFric = 0.99f;

			}else{
				engine.meshes["chimen"].scale(glm::vec3(0.05f,0.05f,0.05f));
				floorFric = 0.9f;
				defFric = 0.98f;
				canSlide = true;
			}
			slideVec = glm::vec3(0);

			if(engine.keyboard.isKeyPressed(GLFW_KEY_W)){
				addForce(forward*speed*deltaTime);
				slideVec += forward;
			}
			if(engine.keyboard.isKeyPressed(GLFW_KEY_A)){
				addForce(left*speed*deltaTime);
				slideVec += left;
			}
			if(engine.keyboard.isKeyPressed(GLFW_KEY_S)){
				addForce(back*speed*deltaTime);
				slideVec += back;
			}
			if(engine.keyboard.isKeyPressed(GLFW_KEY_D)){
				addForce(right*speed*deltaTime);
				slideVec += right;
			}
			if(grounded && engine.keyboard.isKeyPressed(GLFW_KEY_SPACE)){
				addForce(up*jump*deltaTime);
			}
			
		}
		void addForce(glm::vec3 forceVec){
			forces += forceVec;
		}
		void physics(float deltaTime){
			addForce(glm::vec3(0,-grav,0));
			Collider* collisObj = collisionManager.colliders["chimen"];
			grounded = false;
			if(collisObj->lastCollisionObj != "NULL"){
				//std::cout << "eep" << std::endl;
				grounded = true;
				glm::vec3 MTV = collisObj->mtv;
				//printVec(MTV);
				pos += MTV;
				bool xTest = (abs(MTV.x) > 0);
				bool yTest = (abs(MTV.y) > 0);
				bool zTest = (abs(MTV.z) > 0);
				if(xTest){
					vel.x = 0;
				}
				if(yTest){
					vel.y = 0;
				}
				if(zTest){
					vel.z = 0;
				}
				fric = floorFric;
			}
			accel = forces/mass;
			vel += accel*deltaTime;
			pos += vel*deltaTime;

			vel = glm::vec3(vel.x*fric, vel.y*yFric, vel.z*fric);
			fric = defFric;
			forces = glm::vec3(0,0,0);

		}
		void update(float deltaTime){
			input(deltaTime);
			physics(deltaTime);
			engine.meshes["chimen"].translate(pos);
			glm::vec3 abv(0,0.4f,0);
			float offSet = 0.3f;
			engine.meshes["chimen"].rotate(90-engine.camera.yaw, glm::vec3(0,1.0f,0));
			glm::vec3 target = abv+engine.meshes["chimen"].getMiddleVector()-engine.camera.dir*camDist + engine.camera.right*offSet;
			engine.camera.position = lerpVec3(engine.camera.position, target, 0.5f);
			collisionManager.colliders["chimen"]->setMaxMinsFromMesh(engine.meshes["chimen"].getWorldSpaceMaxMins());

			if(pos.y < -60){
				std::cout << "ded" << std::endl;
				pos = glm::vec3(0, 10, 0);
			}
			//printVec(pos);
		}
};
Chicken* chimen;
std::vector<float>  points{1,1,1,5,5,5};
Line* test;
void init(float deltaTime){
	lineShader = ShaderManager(
		"../assets/shaders/vertex/lineshader.vert",
		"../assets/shaders/fragment/lineshader.frag"
	);
	test = new Line(points,2);
	test->setLineWidth(2);
	chimen = new Chicken();
	std::cout << "C++ Standard Version: " << __cplusplus << std::endl;
	lightManager.addPointLight(&engine.meshes, pos, lightCol, lightCol, 4.8f, 2.4f);

	//glfwSetCursorPos(engine.window, engine.W/2, engine.H/2);
	glfwSetInputMode(engine.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(engine.window, look);
	
	std::string path = "../assets/resources/bingform";
	engine.meshes["bingform"] = Mesh::loadOBJ(path);
	engine.meshes["bingform"].scale(glm::vec3(105,1,105));
	collisionManager.addCollider("bingform", collisionManager.convertMeshToCollider(engine.meshes["bingform"].getWorldSpaceMaxMins()));

	engine.meshes["bingform2"] = engine.meshes["bingform"];
	engine.meshes["bingform2"].scale(glm::vec3(0.5,1,0.5));
	engine.meshes["bingform2"].translate(glm::vec3(0,2,0));
	collisionManager.addCollider("bingform2", collisionManager.convertMeshToCollider(engine.meshes["bingform2"].getWorldSpaceMaxMins()));

	path = "../assets/resources/fox";
	engine.meshes["foxy"] = Mesh::loadOBJ(path);
	engine.meshes["foxy"].translate(glm::vec3(0,0,0));
	engine.meshes["foxy2"] = engine.meshes["foxy"];
	engine.meshes["foxy2"].translate(glm::vec3(5,5,5));
	int num = 100;
	for(int i=0; i < num; i++){
		std::string id = "fox"+std::to_string(i);
		engine.meshes[id] = engine.meshes["foxy"];
		int x = rand()%100 * (rand()%2==0 ? -1 : 1);
		int y = rand()%100 * (rand()%2==0 ? -1 : 1);
		int z = rand()%6;
		engine.meshes[id].translate(glm::vec3(x,z,y));

	}

	//setFullscreen(1);
	glfwSwapInterval(1);
}

void draw(float deltaTime){
	glClearColor(0.3f, 0.0f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	lightManager.update(&engine.meshes, engine.shader);

	//std::cout << engine.meshes.size() << std::endl;
	for(auto& mesh: engine.meshes){
		engine.camera.matrixCalc(&engine.shader, engine.W, engine.H, false);
		mesh.second.draw(&engine.shader);
	}
	test->draw();
	glUseProgram(engine.shader.getShaderID());
	//std::cout << engine.camera.position.x << ", " << engine.camera.position.y << ", " << engine.camera.position.z << std::endl;

}
void update(float deltaTime){
	float lightSpeed = 0.1f;
	chimen->update(deltaTime);
	collisionManager.checkCollisions();
	lightManager.pointLights[0].pos = chimen->pos + glm::vec3(0, 4, 0);

}

int main(int argc, char** argv){
	char title[] = "uwu";
	engine.events.registerEventCallback("init", init);
	engine.events.registerEventCallback("draw", draw);
	engine.events.registerEventCallback("update", update);
	engine.init(1280, 720, title);



	return engine.main();
}
