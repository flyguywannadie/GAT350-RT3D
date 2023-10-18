#version 430

in layout(location = 0) vec2 ftexcoord;
in layout(location = 1) vec3 fnormal;
in layout(location = 2) vec4 fcolor;

out layout(location = 0) vec4 ocolor;

layout(binding = 0) uniform sampler2D tex;

void main()
{
	vec4 texColor = texture(tex,ftexcoord);
	//if (texColor.a < 0.8) discard;
	ocolor = texColor * fcolor;
}