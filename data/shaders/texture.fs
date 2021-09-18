varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;

//material
uniform vec3 ambient; //reflected ambient light
uniform vec3 diffuse; //reflected diffuse light
uniform vec3 specular; //reflected specular light
uniform float shininess; //glosiness coefficient (plasticity)

//light
uniform vec3 position; //where is the light
uniform vec3 diffuse_color; //the amount (and color) of diffuse
uniform vec3 specular_color; //the amount (and color) of specular
uniform vec3 ambientLight; // the amount (and color) of ambient

uniform vec3 eye;

void main()
{

	//here write the computations for PHONG.
	vec3 l_vector = position - v_position;
	l_vector = normalize(l_vector);
	vec3 r_vector = 2*(dot(v_normal, l_vector))*v_normal-l_vector;
	r_vector = normalize(r_vector);
	vec3 v_vector = eye - v_position;
	v_vector = normalize(v_vector);

	vec3 ambientL = ambient * ambientLight;
	vec3 diffuseL = diffuse *(dot(normalize(v_normal),l_vector)) * diffuse_color;
	vec3 specularL = specular * (pow(clamp(dot(v_vector, r_vector), 0, 1),shininess)) * specular_color;
	
	vec3 wLight = ambientL + diffuseL + specularL;

	vec2 uv = v_uv;
	gl_FragColor = texture2D( u_texture, uv ) * vec4( wLight , 1.0 ) * 1.0;
}
