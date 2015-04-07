#version 400 core

//  Transformation matrices
uniform Tranformations {
	// Projection Matrix
	mat4 ProjectionMatrix;
	// ModelView Matrix
	mat4 ModelViewMatrix;
	// Normal matrix;
	mat4 normalMatrix;
	// ModelViewProjection Matrix
	mat4 MVP;
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
 

//  Input from previous shader
in vec3 IPosition;
in vec3 INormal;
in vec3 FrontColor;
in vec2 ITextCoord;

uniform sampler2D text;

//  Fragment color
layout (location=0) out vec4 Fragcolor;

void main()
{
   Fragcolor = texture(text, ITextCoord.st)*vec4(FrontColor,1.0) * (globalLight.global + globalLight.ambient);
}
