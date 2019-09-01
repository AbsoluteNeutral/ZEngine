/////////////////////////////////////////////////////////////////////////////
//
//\file			null.glsl
//\project name	Tenebrous
//\author			Kee Hui Yao
//\brief
//
//Copyright (C) 2017-2019 DigiPen Institute of Technology. Reproduction or
//disclosure of this file or its contents without the prior written consent of
//DigiPen Institute of Technology is prohibited.
//
/////////////////////////////////////////////////////////////////////////////

#ifdef VS

layout(location = 0) in vec3 loc_position; // The position variable has attribute position 0
layout(location = 2) in vec2 loc_modeUV;

uniform mat4 model; //X
uniform mat4 modelScale; //X
uniform mat4 projview; //X

out vec2 out_uvs;

void main(void) 
{
	out_uvs = loc_modeUV;
	gl_Position = projview * model * modelScale * vec4(loc_position, 1.0f);
}

#endif

#ifdef FS

in vec2 out_uvs;

layout (location = 0) out vec4 finalColor;
layout (location = 1) out vec4 bloomColor;

uniform vec4 color; //X
uniform float mIntensity; //X
uniform sampler2D texture_0; //X

void main() 
{
	finalColor = texture(texture_0, out_uvs);
  bloomColor = vec4(0.0);
}

#endif
