#version 430

#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2

in layout(location = 0) vec3 fposition;
in layout(location = 1) vec3 fnormal;
in layout(location = 2) vec2 ftexcoord;
//in layout(location = 3) vec4 fcolor;

out layout(location = 0) vec4 ocolor;

uniform struct Material
{
	 vec3 diffuse;
	 vec3 specular;
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
} light ;

uniform vec3 ambientColor = vec3(0.2, 0.2, 0.2);

layout(binding = 0) uniform sampler2D tex;

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

	vec3 diffuse = material.diffuse * (light.diffuseColor * intensity) * light.intensity;

	// SPECULAR LIGHT CALCULATION
	vec3 specular = vec3(0);
	if (intensity > 0) {
		vec3 reflection = reflect(-lightDir, normal);
		vec3 viewDir = normalize(-position);
		intensity = max(dot(reflection, viewDir),0) * spotIntensity;
		intensity = pow(intensity, material.shininess);
		specular = material.specular * intensity * light.intensity;
	}

	return ambient + ((diffuse + specular) * attenuation);
}

void main()
{
	vec4 texColor = texture(tex,ftexcoord);
	//if (texColor.a < 0.8) discard;
	//combines the texture color with the ads return color
	ocolor = texColor * vec4(ads(fposition, fnormal), 1);
}