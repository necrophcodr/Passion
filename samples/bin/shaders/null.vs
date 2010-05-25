varying vec2 uv;
varying vec4 gl_FrontColor;

void main()
{
	gl_FrontColor = gl_Color;
	uv = gl_MultiTexCoord0.xy;
	gl_Position = ftransform();
}