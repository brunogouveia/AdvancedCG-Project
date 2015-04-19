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
in vec4 PosModelCoord;

uniform sampler2D text;
uniform sampler2D depthText;

//  Fragment color
layout (location=0) out vec4 Fragcolor;


vec4 phong()
{
	// Compute PosModelCoord in light coordinates
	vec4 ShadowCoord = shadows.DepthBiasMVP * PosModelCoord;
	ShadowCoord /= ShadowCoord.w;

	if (texture(depthText, ShadowCoord.xy).z + 0.001 > ShadowCoord.z || texture(depthText, ShadowCoord.xy).z == 1.0) {
		// Position in eye coordinates
		vec3 pos = IPosition;

		// Normal in eye coordinates
		vec3 N = normalize(INormal);

		// Light vector
		vec3 L = normalize(vec3(tranformations.ViewMatrix * light.position) - pos);

		// Reflection vector
		vec3 R = reflect(-L, N);

		// View vector in eye coordinates
		vec3 V = normalize(-pos);

		// Diffuse light intensity
		float Id = max(0.0, dot(N, L));

		// Specular light intensity
		float Is = (Id > 0.0) ? pow(max(0.0, dot(R, V)), 100) : 0.0;

		return Id*light.diffuse + Is*light.specular;
	} else {
		return vec4(0);
	}
}


void main()
{
	// Compute PosModelCoord in light coordinates
	vec4 ShadowCoord = shadows.DepthBiasMVP * PosModelCoord;
	ShadowCoord /= ShadowCoord.w;
	
	Fragcolor = texture(text, ITextCoord.st)*vec4(FrontColor,1.0) * phong();
	// Fragcolor = vec4(texture(depthText, ITextCoord.st));
	// Fragcolor = vec4(ShadowCoord.xyz, 1.0);
	// Fragcolor = vec4(texture(depthText, ShadowCoord.xy).z);
}
