#version 430

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
	vec3 position;
	vec3 diffuseColor;
} light ;

uniform vec3 ambientColor = vec3(0.2, 0.2, 0.2);

layout(binding = 0) uniform sampler2D tex;

vec3 ads(vec3 position, vec3 normal){
	// AMBIENT LIGHT CALCULATION
	vec3 ambient = ambientColor;

	// DIFFUSE LIGHT CALCULATION
	vec3 lightDir = normalize(light.position - position);
	float intensity = max(dot(lightDir, normal), 0);

	vec3 diffuse = material.diffuse * (light.diffuseColor * intensity);

	// SPECULAR LIGHT CALCULATION
	vec3 specular = vec3(0);
	if (intensity > 0) {
		vec3 reflection = reflect(-lightDir, normal);
		vec3 viewDir = normalize(-position);
		intensity = max(dot(reflection, viewDir),0);
		intensity = pow(intensity, material.shininess);
		specular = material.specular * intensity;
	}

	return ambient + diffuse + specular;
}

void main()
{
	vec4 texColor = texture(tex,ftexcoord);
	//if (texColor.a < 0.8) discard;
	//combines the texture color with the ads return color
	ocolor = texColor * vec4(ads(fposition, fnormal), 1);
}