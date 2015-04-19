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


//  Vertex attributes (input)
layout(location = 0) in vec4 Vertex;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 Color;
layout(location = 3) in vec2 TextCoord;


//  Output to next shader
out vec3 IPosition;
out vec3 INormal;
out vec3 FrontColor;
out vec2 ITextCoord;
out vec4 PosModelCoord;

void main()
{	
   //  Pass all attributes to fragment shader (will be interpolated)
   IPosition = vec3(tranformations.ViewMatrix * tranformations.ModelMatrix * Vertex);
   INormal = normalize(mat3(tranformations.NormalMatrix) * Normal);
   FrontColor = Color;
   ITextCoord = TextCoord;
   //  Position in model coordinates
   PosModelCoord = tranformations.ModelMatrix * Vertex;

   //  Set transformed vertex location
   gl_Position =  tranformations.ProjectionMatrix * tranformations.ViewMatrix * tranformations.ModelMatrix * Vertex;
}
