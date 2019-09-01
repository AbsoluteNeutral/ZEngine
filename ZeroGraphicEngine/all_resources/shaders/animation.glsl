#ifdef VS
                                  
layout (location = 0) in vec3 loc_position; // The position variable has attribute position 0 \n
layout (location = 1) in vec3 loc_normal;
layout (location = 2) in vec2 loc_uv;
layout (location = 3) in vec4 loc_weights;
layout (location = 4) in vec4 loc_joints;
layout (location = 5) in vec3 loc_tangent;

//const int NUM_OF_LIGHTS = 16;
//struct LightSource
//{
//  mat4 lightMatrix;
//  vec4 lightPos;
//  vec4 lightDir;
//  vec4 ambient;
//  vec4 diffuse;
//  vec4 specular;
//  vec3 attenuation;
//  vec2 cutoff;
//  float intensity;
//};

//uniform LightSource Lights[NUM_OF_LIGHTS]; //X

uniform int  anim;
uniform float scalingFactor;
uniform mat4 projview;
uniform mat4 model;

const int MAX_BONES = 100;
uniform mat4 gBones[MAX_BONES];

out vec3 Normal;
out vec2 uvCoords;

//out VS_OUT
//{
//  vec2 uvCoords;
//  vec3 wPos;
//  vec4 lsPos[NUM_OF_LIGHTS];
//  mat3 TBN;
//} vs_out;

void main() 
{
  mat3 diagScaleMatrix = mat3(scalingFactor);
  mat4 modelScale = mat4(diagScaleMatrix);
 
  mat4 boneMat = mat4(1.0);
  if(anim == 1)
  {
    boneMat      = gBones[int(loc_joints[0])] * loc_weights[0];
    boneMat     += gBones[int(loc_joints[1])] * loc_weights[1];
    boneMat     += gBones[int(loc_joints[2])] * loc_weights[2];
    boneMat     += gBones[int(loc_joints[3])] * loc_weights[3];
  }
  Normal = (boneMat * vec4(loc_normal, 0.0)).xyz;
  gl_Position = projview * model * modelScale * boneMat * vec4(loc_position, 1.0f);
  uvCoords = loc_uv;
  
  //mat3 normalMat = transpose(inverse(mat3(model * boneMat)));
  //vec3 T = normalize(normalMat * loc_tangent);
  //vec3 N = normalize(normalMat * loc_normal);
  //
  //T = normalize(T - dot(T, N) * N);
  //vec3 B = cross(N, T);
  //vs_out.TBN = mat3(T, B, N);
  //
  //vs_out.uvCoords = loc_uv;
  //vs_out.wPos = (model * modelScale * vec4(loc_position, 1.0f)).xyz;
  //
  //for(int i = 0; i < NUM_OF_LIGHTS; ++i)
  //{
  //  vs_out.lsPos[i] = Lights[i].lightMatrix * model * modelScale * boneMat * vec4(loc_position, 1.0f);
  //}
}

#endif

#ifdef FS

//in VS_OUT
//{
//  vec2 uvCoords;
//  vec3 wPos;
//  vec4 lsPos[NUM_OF_LIGHTS];
//  mat3 TBN;
//} fs_in;
out vec4 finalcolor;

in vec2 uvCoords;
in vec3 Normal;
uniform vec4 color;

uniform sampler2D diffuseMap;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_ambient1;

void main()
{   
    //finalcolor = color;
    //finalcolor = vec4(Normal, color.w);
    //finalcolor = vec4(1.0, 0.0, 0.0, 1.0);
    finalcolor = texture(diffuseMap, uvCoords) * color;
}
#endif
