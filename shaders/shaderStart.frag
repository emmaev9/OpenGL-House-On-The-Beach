#version 410 core

in vec3 normal;
in vec4 fragPosEye;
in vec4 fragPosLightSpace;
in vec2 fragTexCoords;

out vec4 fColor;

//lighting and texture
uniform	mat3 normalMatrix;
uniform mat3 lightDirMatrix;
uniform	vec3 lightColor;
uniform	vec3 lightDir;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;
uniform samplerCube skyBox;
uniform float alpha;
uniform mat4 view;
uniform int enableLight;
uniform bool glass;
uniform bool masinaPornita;
uniform bool inHouse;

vec3 ambient;
float ambientStrength = 0.1f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 32.0f;

uniform	vec3 lightPos1;
uniform	vec3 lightPos2;
uniform	vec3 lightPos3;
uniform vec3 lightPos4;
uniform vec3 lightPos5;
uniform vec3 far1;
uniform vec3 far2;
uniform vec3 far3;
uniform vec3 far4;

//point light attributes
float ambientPoint = 1.0f;
float specularStrengthPoint = 2.9f;
float shininessPoint = 64.0f;

float constant = 1.0f;
float linear = 0.7f;
float quadratic = 1.8f;


float cutOff;


vec3 o;

float computeFog()
{
   float fogDensity=0.008f;
   float fragmentDistance=length(fragPosEye);
   float fogFactor = exp(-pow(fragmentDistance * fogDensity,0.9));

   return clamp(fogFactor,0.0f,1.0f);
}


float computeShadow()
{
	vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	normalizedCoords = normalizedCoords * 0.5 + 0.5;

	if (normalizedCoords.z > 1.0f)
		return 0.0f;

	float closestDepth = texture(shadowMap, normalizedCoords.xy).r;
	float currentDepth = normalizedCoords.z;

	float bias = 0.005f;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	return shadow;

}



vec3 computePointLight(vec4 lightPosEye, vec3 lightColor, float constant, float linear, float quadratic)
{
    vec3 cameraPosEye = vec3(0.0f);
	vec3 normalEye = normalize(normalMatrix * normal);
	vec3 lightDirN = normalize(lightPosEye.xyz - fragPosEye.xyz);
	vec3 viewDirN = normalize(cameraPosEye - fragPosEye.xyz);
	
	vec3 ambient = ambientPoint * lightColor;
	vec3 diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	vec3 halfVector = normalize(lightDirN + viewDirN);
	vec3 reflection = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(normalEye, halfVector), 0.0f), shininessPoint);
	vec3 specular = specularStrengthPoint * specCoeff * lightColor;
	
	float distance = length(lightPosEye.xyz - fragPosEye.xyz);
	
	float att = 1.0f / (constant + linear * distance + quadratic * distance * distance);
	
	ambient *= vec3(texture(diffuseTexture, fragTexCoords));
	diffuse *= vec3(texture(diffuseTexture, fragTexCoords));
	specular *= vec3(texture(specularTexture, fragTexCoords));
	
	return (ambient + diffuse + specular) * att;
} 


vec3 computeLightComponents()

{	//in eye coordinates, the viewer is situated at the origin	
	vec3 cameraPosEye = vec3(0.0f);
	
	//transform normal
	vec3 normalEye = normalize(normalMatrix * normal);	
	
	//compute light direction
	vec3 lightDirN = normalize(/*lightDirMatrix * */lightDir);	

	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fragPosEye.xyz);
	
	//compute half vector
	vec3 halfVector = normalize(lightDirN + viewDirN);
		
	//compute ambient light
	ambient = ambientStrength * lightColor;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	//compute specular light
	//vec3 ref = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(halfVector, normalEye), 0.0f), shininess);
	specular = specularStrength * specCoeff * lightColor;

	ambient *= vec3(texture(diffuseTexture, fragTexCoords));
	diffuse *= vec3(texture(diffuseTexture, fragTexCoords));
	specular *= vec3(texture(specularTexture, fragTexCoords));
	
	float ratio = 1.00 /0.52;

	vec3 refractedVector = refract(viewDirN, normalEye, ratio);
	vec3 reflectedVector = reflect(viewDirN, normalEye);
	float s = computeShadow();
	if(glass)
	{
			return min(((1.0f-s/3.0f)*ambient + (1.0f - s)*diffuse) + (1.0f - s)*specular + (texture(skyBox,reflectedVector)).rgb *(texture(skyBox,refractedVector)).rgb , 1.0f);	
	}
	else
	{
		if(inHouse){
			return (ambient + diffuse + specular); 
		}
		else{
			return min((1.0f-(s/2.0f))*ambient + (1.0f - s)*diffuse + (1.0f - s)*specular , 1.0f);
		}
	}
}

void main() 
{
	vec3 color = computeLightComponents();

	vec4 lightPos1 = view * vec4(lightPos1, 1.0f);
	vec4 lightPos2 = view * vec4(lightPos2, 1.0f);
	vec4 lightPos3 = view * vec4(lightPos3, 1.0f);
	vec4 lightPos4 = view * vec4(lightPos4, 1.0f);
	vec4 lightPos5 = view * vec4(lightPos5, 1.0f);
	vec4 far1 = view * vec4(far1, 1.0f);
	vec4 far2 = view * vec4(far2, 1.0f);
	vec4 far3 = view * vec4(far3, 1.0f);
	vec4 far4 = view * vec4(far4, 1.0f);

	     
    //color += computePointLight(lightPos1, vec3(1.0f, 1.0f, 0.0f),  1.0f, 0.2f, 0.20f);
    color += computePointLight(lightPos2, vec3(2.0f, 2.0f, 0.2f), 1.0f, 0.22f, 0.20f);
  	color += computePointLight(lightPos2, vec3(1.0f, 0.47f, 0.78f), 1.0f, 0.22f, 0.20f);
  //	color += computePointLight(lightPos3, vec3(1.0f, 0.1f, 0.3f), 1.0f, 0.35f, 0.4f);
	//color += computePointLight(lightPos4, vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.22f, 0.20f);  
	color += computePointLight(lightPos5, vec3(1.0f, 0.0f, 1.0f), 1.0f, 0.22f, 0.2f);

	if(masinaPornita){
		color += computePointLight(far1, vec3(1.0f, 1.0f, 0.4f), 1.0f, 1.22f, 0.40f);
		color += computePointLight(far2, vec3(1.0f, 1.0f, 0.4f), 1.0f, 1.22f, 0.40f);
		color += computePointLight(far3, vec3(1.0f, 1.0f, 0.4f), 1.0f, 1.22f, 0.40f);
		color += computePointLight(far4, vec3(1.0f, 1.0f, 0.4f), 1.0f, 1.22f, 0.40f);
	}
	
	vec4 computedColor = vec4(color, 1.0f);
	
	float fogFactor=computeFog();
	vec4 fogColor=vec4( 0.098f, 0.17f, 0.3f,1.0f);
    fColor = fogColor * (1-fogFactor) + computedColor * fogFactor;

	fColor.a = alpha;

}

