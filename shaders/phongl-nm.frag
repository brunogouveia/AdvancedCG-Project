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
uniform sampler2DShadow depthText;
uniform sampler2D normalMap;

//  Fragment color
layout (location=0) out vec4 Fragcolor;

mat3 cotangent_frame( vec3 N, vec3 p, vec2 uv )
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
 
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    // construct a scale-invariant frame 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}

vec4 phong()
{
	// Compute PosModelCoord in light coordinates
	vec4 ShadowCoord = shadows.DepthBiasMVP * PosModelCoord;
	ShadowCoord /= ShadowCoord.w;

	if (textureProj(depthText, ShadowCoord) == 1.0 || (ShadowCoord.x < 0.0 || ShadowCoord.x > 1.0) || (ShadowCoord.y < 0.0 || ShadowCoord.y > 1.0)) {
		// Position in eye coordinates
		vec3 pos = IPosition;

		// Normal in eye coordinates
		vec3 N = normalize(INormal);

		// Matrix that transforms from eye coordinate splace to text coordinate space
		mat3 TBN = transpose(cotangent_frame(N, pos, ITextCoord.st));
		// Compute new normal
		N = normalize((texture(normalMap, ITextCoord.st)).rgb*2 - 1);

		// Light vector
		vec3 L = normalize((vec3(tranformations.ViewMatrix * light.position)) - pos);
		// Tranfrom L to text coordinate space
		L = normalize(TBN * L);

		// Reflection vector
		vec3 R = reflect(-L, N);

		// View vector in eye coordinates
		vec3 V = normalize(-pos);
		// V in text coordinate space
		V = normalize(TBN*V);

		// Diffuse light intensity
		float Id = max(0.0, dot(N, L));

		// Specular light intensity
		float Is = (Id > 0.0) ? pow(max(0.0, dot(R, V)), 100) : 0.0;

		return Id*light.diffuse + Is*light.specular;
	} else {
		vec4(0.0);
	}
}


void main()
{
	// Compute PosModelCoord in light coordinates
	vec4 ShadowCoord = shadows.DepthBiasMVP * PosModelCoord;
	ShadowCoord /= ShadowCoord.w;
	
	Fragcolor = texture(text, ITextCoord.st) * phong();
	// Fragcolor = vec4(texture(depthText, ITextCoord.st));
	// Fragcolor = vec4(ShadowCoord.xyz, 1.0);
	// Fragcolor = vec4(texture(depthText, ShadowCoord.xy).z);
}