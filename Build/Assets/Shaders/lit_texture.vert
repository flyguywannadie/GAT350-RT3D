#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 1) vec2 vtexcoord;
in layout(location = 2) vec3 vnormal;

out layout(location = 0) vec2 otexcoord;
out layout(location = 1) vec3 onormal;
out layout(location = 2) vec4 ocolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform struct Material
{
	 vec4 color;
	 vec2 offset;
	 vec2 tiling;
} material ;

uniform struct Light
{
	vec3 position;
	vec3 diffuseColor;
} light ;

uniform vec3 ambientColor = vec3(0.2, 0.2, 0.2);

void main()
{
	otexcoord = (vtexcoord * material.tiling) + material.offset;
	onormal = vnormal;

	mat4 modelView = view * model;
	vec4 position = modelView * vec4(vposition,1);
	vec3 normal = mat3(modelView) * vnormal;
	vec3 lightDir = normalize(light.position - position.xyz);
	float intensity = max(dot(lightDir, normal), 0);

	vec3 lightColor = (light.diffuseColor * intensity) + ambientColor;

	ocolor = material.color * vec4(lightColor, 1);

	mat4 mvp = projection * view * model;

	gl_Position = mvp * vec4(vposition , 1.0);

}