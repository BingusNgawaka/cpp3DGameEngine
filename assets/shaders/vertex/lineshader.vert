#version 330 core

// vertex data for all execs of this shader
layout(location = 0) in vec3 vertexPosition_modelspace;

// output data
out vec2 fragTexCoord;
out vec3 Normal;
out vec3 fragPos;

// values that stays const for whole mesh
uniform mat4 view;
uniform mat4 proj;

void main(){
	// output vertex xyzw in terms of clip space	
	gl_Position = proj * view * vec4(vertexPosition_modelspace,1);
}
