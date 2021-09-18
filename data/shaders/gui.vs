attribute vec3 a_vertex;
attribute vec2 a_uv;

uniform mat4 u_viewprojection;

//this will store the color for the pixel shader
varying vec3 v_position;
varying vec3 v_world_position;
varying vec2 v_uv;

void main()
{	

	//calcule the vertex in object space
	v_position = a_vertex;
	v_world_position = (vec4( v_position, 1.0) ).xyz;

	//store the texture coordinates
	v_uv = a_uv;

	//calcule the position of the vertex using the matrices
	gl_Position = u_viewprojection * vec4( v_world_position, 1.0 );
}