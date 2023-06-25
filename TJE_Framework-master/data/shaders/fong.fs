
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;

uniform vec3 u_camera_pos;
uniform vec3 u_light_pos;

void main()
{
	vec3 N = normalize(v_normal);
	vec3 L = normalize(u_light_pos - v_world_position);
	vec3 V = normalize(u_camera_pos - v_world_position);
	vec3 R = normalize(reflect(-L, N));

	vec3 Ka = vec3(1,1,1);
	vec3 Kd = texture2D( u_texture, v_uv ).rgb;
	vec3 Ks = vec3(1,1,1);

	vec3 Ia = vec3(0.05,0.05,0.05);
	vec3 Id = vec3(1,1,1);
	vec3 Is = vec3(1,1,1);

	vec3 ambient = Ka*Ia;
	vec3 diffuse =  Kd* max(0.0, dot(L,N)) * Id;
	vec3 specular = Ks * pow(max(dot(R,V), 0.0), 20) * Is;

	gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}
