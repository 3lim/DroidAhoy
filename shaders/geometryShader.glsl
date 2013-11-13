#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormals;
layout(location = 3) in vec2 vertexUV;

//out vec3 fragmentColor;

uniform mat4 MVP;

layout(std140) uniform Directions
{
  float dirs[];
};

layout(std140) uniform BoneData
{
  vec3 bones[];
};

void main()
{
	UV = in_UV
	//Apply animation transformation.
	int index = gl_LocalInvocationIndex;
	vec3 animPos = vertexPosition_modelspace + dirs[index] * (bones[index] -  vertexPosition_modelspace);
	
	gl_Position = MVP * vec4(vertexPosition_modelspace,1);	
	
	UV = vertexUV;
	
	//fragmentColor = vertexNormals;
}