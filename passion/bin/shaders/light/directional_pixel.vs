varying vec2 uv;
varying vec4 gl_FrontColor;

varying vec3 normal;
varying vec3 pos;

void main()
{
	gl_Position = ftransform();
	
	normal = gl_Normal;
	pos = gl_Position.xyz;
	
	gl_FrontColor = gl_Color;	
	uv = gl_MultiTexCoord0.xy;
}