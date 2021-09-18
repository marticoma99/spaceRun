varying vec2 v_uv;
uniform sampler2D u_texture;
uniform vec4 range;
uniform vec4 u_color;

uniform int partToPaint;

varying vec3 v_position;

void main()
{
	vec2 uv = v_uv;
	uv.x = range.z * uv.x + range.x;
	uv.y = range.w * uv.y + range.y;

	if(v_position.x > partToPaint) discard;

	gl_FragColor = u_color * texture2D( u_texture, uv );
}