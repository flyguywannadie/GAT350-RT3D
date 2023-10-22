#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 1) vec2 vtexcoord;
in layout(location = 2) vec3 vnormal;

out layout(location = 0) vec3 oposition;
out layout(location = 1) vec3 onormal;
out layout(location = 2) vec2 otexcoord;
out layout(location = 3) vec4 ocolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

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

vec3 ads(in vec3 position, in vec3 normal)
{
	// AMBIENT
	vec3 ambient = ambientColor;

	// DIFFUSE
	vec3 lightDir = normalize(light.position - position);
	float intensity = max(dot(normal, lightDir), 0);

	vec3 diffuse = material.diffuse * (light.diffuseColor * intensity);

	// SPECULAR
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
	mat4 modelView = view * model;

	// convert position and normal to world-view space
	otexcoord = (vtexcoord * material.tiling) + material.offset;
	onormal = normalize(mat3(modelView) * vnormal);
	oposition = vec3(modelView * vec4(vposition, 1));

	ocolor = vec4(ads(oposition, onormal), 1);

	mat4 mvp = projection * view * model;

	gl_Position = mvp * vec4(vposition , 1.0);

}