#version 400 core

//  Transformation matrices
uniform Tranformations {
	// Projection Matrix
	mat4 ProjectionMatrix;
	// Model Matrix
	mat4 ModelMatrix;
	// View Matrix
	mat4 ViewMatrix;
	// Normal matrix;
	mat4 NormalMatrix;
} tranformations;

uniform GlobalLight {
	// Colors
	vec4 global;
	vec4 ambient;
} globalLight;

uniform Light {
	// Position
	vec4 position;
	
	// Colors
	vec4 diffuse;
	vec4 specular;
} light;
 
//  Shadows matrices
uniform Shadows {
	// Projection Matrix
	mat4 depthMVP;
	// Model Matrix
	mat4 DepthBiasMVP;
} shadows;

//  Input from previous shader
in vec3 IPosition;
in vec3 INormal;
in vec3 FrontColor;
in vec2 ITextCoord;

uniform sampler2D text;
uniform sampler2D depthText;

//  Fragment color
layout (location=0) out vec4 Fragcolor;

void main()
{
   Fragcolor = texture(text, ITextCoord.st)*vec4(FrontColor,1.0) * (globalLight.global + globalLight.ambient);
}
