varying vec2 uv;
varying vec4 gl_FrontColor;

void main()
{
	gl_Position = ftransform();
	
	gl_FrontColor = vec4( abs( gl_Normal.x ), abs( gl_Normal.y ), abs( gl_Normal.z ), 1.0f );
	uv = gl_MultiTexCoord0.xy;
}