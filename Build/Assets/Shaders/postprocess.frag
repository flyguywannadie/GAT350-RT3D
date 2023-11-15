#version 430

#define INVERT_MASK		(1 << 0)
#define GRAYSCALE_MASK	(1 << 1)
#define COLOR_TINT_MASK	(1 << 2)
#define SCANLINE_MASK	(1 << 3)
#define GRAIN_MASK		(1 << 4)
#define CSGOKNIFE_MASK	(1 << 5)

in layout(location = 0) vec2 ftexcoord;
out layout(location = 0) vec4 ocolor;

uniform float intensity = 0;
uniform uint params = 0;
uniform vec3 tint;
uniform int scanlineSize = 4;
uniform float time = 0;

layout(binding = 0) uniform sampler2D screenTexture;

float random(vec2 st) 
{
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec4 invert(in vec4 color){
	return vec4(vec3(1)-color.rgb, color.a);
}

vec4 grayscale(in vec4 color){
	return vec4(vec3((color.r + color.b + color.g))/3, color.a);
}

vec4 colortint(in vec4 color, in vec3 tint){
	return vec4(vec3(color.r * tint.r, color.b * tint.b, color.g * tint.g), color.a);
}

vec4 grain(in vec4 color){
	return color * random(gl_FragCoord.xy + time);
}

vec4 csgoknife(in vec4 color){
	return vec4(random(vec2(color.r)),random(vec2(color.g)),random(vec2(color.b)),color.a);
}

vec4 scanline(in vec4 color){
	return (int(gl_FragCoord.y) % scanlineSize != 0) ? vec4(0,0,0, color.a) : color;
}

void main()
{
	vec4 basecolor = texture(screenTexture, ftexcoord);
	vec4 postprocess = basecolor;

	// 0001 <- mask
	// 0000 <- params

	if (bool(params & INVERT_MASK)) postprocess = invert(basecolor);
	if (bool(params & COLOR_TINT_MASK)) postprocess = colortint(postprocess, tint);
	if (bool(params & GRAYSCALE_MASK)) postprocess = grayscale(postprocess);
	if (bool(params & SCANLINE_MASK)) postprocess = scanline(postprocess);
	if (bool(params & GRAIN_MASK)) postprocess = grain(postprocess);
	if (bool(params & CSGOKNIFE_MASK)) postprocess = csgoknife(postprocess);

	ocolor = mix(basecolor, postprocess, intensity);
}