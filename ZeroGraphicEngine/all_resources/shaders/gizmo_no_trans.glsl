/////////////////////////////////////////////////////////////////////////////
//
//\file			gizmo_no_trans.glsl
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
													
layout (location = 0) in vec3 loc_position;

uniform mat4 projview; //X

void main(void) { 																				
	gl_Position = projview * vec4(loc_position, 1.0f);
}
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef FS

out vec4 finalcolor;
uniform vec4 color; //X

void main(){
    finalcolor = color;
}
#endif