#ifdef VS

layout(location = 0) in vec3 loc_position; // The position variable has attribute position 0
layout(location = 1) in vec2 loc_modeUV;

uniform mat4 model; //X
uniform mat4 modelScale; //X
uniform mat4 projview; //X

out vec2 out_uvs;

void main(void) 
{
	out_uvs = loc_modeUV;
	//gl_Position = projview * model * modelScale * vec4(loc_position, 1.0f);
	gl_Position =  vec4(loc_position, 1.0f);
}

#endif

#ifdef FS

in vec2 out_uvs;

out vec4 finalColor;
//layout (location = 1) out vec4 bloomColor;

uniform vec4 color; //X
uniform float mIntensity; //X
uniform sampler2D texture_0; //X

void main() 
{
	finalColor = texture(texture_0, out_uvs);
	//finalColor = vec4(1.0, 0.0, 0.0, 1.0) + texture(texture_0, out_uvs);
  //bloomColor = vec4(0.0);
}

#endif
