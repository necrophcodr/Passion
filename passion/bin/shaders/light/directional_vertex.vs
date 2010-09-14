varying vec2 uv;
varying vec4 gl_FrontColor;

uniform vec4 lightDiffuse;
uniform vec4 lightAmbient;
uniform vec3 lightPos;
uniform float diffuse;
uniform float ambient;

void main()
{
	gl_Position = ftransform();
	
	float ang = dot( normalize( lightPos ), gl_Normal );
	
	gl_FrontColor = lightDiffuse * diffuse * ang + lightAmbient * ambient;
	
	uv = gl_MultiTexCoord0.xy;
}