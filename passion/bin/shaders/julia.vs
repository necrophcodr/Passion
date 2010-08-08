varying vec2 texCoord;

uniform float centerRe;
uniform float centerIm;
uniform float scaleRe;
uniform float scaleIm;

uniform float time;

void main()
{
	vec4 v = vec4( gl_Vertex );
	gl_Position = gl_ModelViewProjectionMatrix * v;
	
	texCoord = gl_MultiTexCoord0.xy;
}