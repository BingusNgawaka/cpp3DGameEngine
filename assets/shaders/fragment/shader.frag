#version 330 core

// consts
const int MAX_POINT_LIGHTS = 1;
const vec3 ambientLightColor = vec3(1.0);
const float ambientLightStrength = 0.01;
const float screenGamma = 2.2;

// structs
struct dirLight{
	vec3 pos;
	vec3 diffuse;
	vec3 specular;
	vec3 dir;
};

struct pointLight{
	vec3 pos;
	vec3 diffuse;
	vec3 specular;
	float specStrength;
	float diffStrength;
};

// output data
out vec3 color;

// input data
in vec2 fragTexCoord;
in vec3 Normal;
in vec3 fragPos;

// uniforms
uniform sampler2D textureSampler;
uniform vec3 viewPos;
uniform float MATERIALSHININESS;
uniform pointLight pointLights[MAX_POINT_LIGHTS];

vec3 calcPointLightContrib(pointLight light, vec3 normal, vec3 fragPos, vec3 viewPos){
	vec3 ambient = ambientLightStrength * ambientLightColor;
	
	vec3 norm = normalize(normal);
	vec3 lightDir = light.pos - fragPos;
	float distance = length(lightDir);
	distance = distance * distance;
	lightDir = normalize(lightDir);
	
	float lambertian = max(dot(lightDir, norm), 0.0);
	float specular = 0.0;
	if(lambertian > 0.0){
		vec3 viewDir = normalize(viewPos-fragPos);
		vec3 halfDir = normalize(lightDir + viewDir);
		float specAngle = max(dot(halfDir, normal), 0.0);
		specular = pow(specAngle, MATERIALSHININESS/4.0);
	}
	
	vec3 spec = specular*light.specular*light.specStrength/distance;
	vec3 diffuse = lambertian*light.diffuse*light.diffStrength/distance;
	vec3 linearCol = ambient + diffuse + spec;
	vec3 gammaCorrected = pow(linearCol, vec3(1.0/screenGamma));
	return gammaCorrected;
}

void main(){
	vec3 k = vec3(0.0);
	for(int i = 0; i < MAX_POINT_LIGHTS; i++){
		k += calcPointLightContrib(pointLights[i], Normal, fragPos, viewPos);
	}
	color =  k * texture(textureSampler, fragTexCoord).rgb;
}
