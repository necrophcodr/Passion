varying vec2 uv;
varying vec4 gl_FrontColor;

uniform float time;

void main()
{
	gl_Vertex.z += sin( gl_Vertex.x / 50.0f + gl_Vertex.y / 50.0f + time ) * 20.0f;
	gl_Position = ftransform();
	
	gl_FrontColor = gl_Color;
	uv = gl_MultiTexCoord0.xy;
}