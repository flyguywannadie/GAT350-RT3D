#version 430

#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2

#define ALBEDO_TEXTURE_MASK (1 << 0) // 0001
#define SPECULAR_TEXTURE_MASK (1 << 1) // 0010
#define NORMAL_TEXTURE_MASK (1 << 2) // 0100
#define EMISSIVE_TEXTURE_MASK (1 << 3) // 1000

in layout(location = 0) vec3 fposition;
in layout(location = 1) vec3 fnormal;
in layout(location = 2) vec2 ftexcoord;
//in layout(location = 3) vec4 fcolor;

out layout(location = 0) vec4 ocolor;

uniform struct Material
{
	uint params;
	vec3 albedo;
	vec3 specular;
	vec3 emissive;
	float shininess;

	vec2 offset;
	vec2 tiling;
} material ;

uniform struct Light
{
	int type;
	vec3 position;
	vec3 direction;
	vec3 diffuseColor;
	float innerAngle;
	float outerAngle;
	float intensity;
	float range;
} lights[3];

uniform int numLights = 3;

uniform vec3 ambientColor = vec3(0.2, 0.2, 0.2);

layout(binding = 0) uniform sampler2D albedoTexture;
layout(binding = 1) uniform sampler2D specularTexture;
layout(binding = 2) uniform sampler2D normalTexture;
layout(binding = 3) uniform sampler2D emissiveTexture;

float attenuation(in vec3 position1, in vec3 position2, in float range)
{
	float distanceSqr = dot(position1 - position2, position1 - position2);
	float rangeSqr = pow(range, 2.0);
	float attenuation = max(0, 1 - pow((distanceSqr / rangeSqr), 2.0));
	attenuation = pow(attenuation, 2.0);
 
	return attenuation;
}

void phong(in Light light, in vec3 position, in vec3 normal, out vec3 diffuse, out vec3 specular){
	
	// DIFFUSE LIGHT CALCULATION
	vec3 lightDir = (light.type == DIRECTIONAL) ? normalize(-light.direction) : normalize(light.position - position);
	
	float spotIntensity = 1;
	if (light.type == SPOT)
	{
		float angle = acos(dot(light.direction, -lightDir));
		//if (angle > light.innerAngle) spotIntensity = 0;
		spotIntensity = smoothstep(light.outerAngle, light.innerAngle, angle);
	}
	
	float intensity = max(dot(lightDir, normal), 0) * spotIntensity;

	diffuse = (light.diffuseColor * intensity) * light.intensity;

	// SPECULAR LIGHT CALCULATION
	specular = vec3(0);
	if (intensity > 0) {
		vec3 reflection = reflect(-lightDir, normal);
		vec3 viewDir = normalize(-position);
		intensity = max(dot(reflection, viewDir),0) * spotIntensity;
		intensity = pow(intensity, material.shininess);
		specular = vec3(intensity * light.intensity);
	}
}

/*
vec3 ads(vec3 position, vec3 normal){
	// AMBIENT LIGHT CALCULATION
	vec3 ambient = ambientColor;

	// ATTENUATION
	float attenuation = 1;
	if (light.type !=  DIRECTIONAL) {
		float distanceSqr = dot(light.position - position, light.position - position);
		float rangeSqr = pow(light.range, 2.0);
		attenuation = max(0, 1 - pow((distanceSqr/rangeSqr), 2.0));
		attenuation = pow(attenuation, 2.0);
	}

	return ambient + ((diffuse + specular) * attenuation);
}
*/

void main()
{
	vec4 albedoColor = bool(material.params & ALBEDO_TEXTURE_MASK) ? texture(albedoTexture, ftexcoord) : vec4(material.albedo, 1);
	vec4 specularColor = bool(material.params & SPECULAR_TEXTURE_MASK) ? texture(specularTexture, ftexcoord) : vec4(material.specular, 1);
	vec4 emissiveColor = bool(material.params & EMISSIVE_TEXTURE_MASK) ? texture(emissiveTexture, ftexcoord) : vec4(material.emissive, 1);

	// set ambient light + emissive color
	ocolor = vec4(ambientColor, 1) * albedoColor + emissiveColor;

 
	// set lights
	for (int i = 0; i < numLights; i++)
	{
		vec3 diffuse;
		vec3 specular;
 
		float attenuation = (lights[i].type == DIRECTIONAL) ? 1 : attenuation(lights[i].position, fposition, lights[i].range);
 
		phong(lights[i], fposition, fnormal, diffuse, specular);
		ocolor += ((vec4(diffuse, 1) * albedoColor) + (vec4(specular, 1) * specularColor)) * lights[i].intensity * attenuation;
	}
}