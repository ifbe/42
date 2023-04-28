in vec3 normal;
in vec2 texuvw;
in vec3 angle0;
out vec4 FragColor;
uniform float time;
uniform sampler2D dudvmap;
uniform sampler2D reflect;
void main(){
	vec2 du = texture(dudvmap, vec2(texuvw.x+time, texuvw.y)).rg;
	du = (2.0*du - vec2(1.0, 1.0)) / 64.0;
	vec2 dv = texture(dudvmap, vec2(texuvw.x+du.y, texuvw.y-time)).rg;
	dv = (2.0*dv - vec2(1.0, 1.0)) / 64.0;
	vec2 uv = texuvw + du + dv;
	uv = vec2(clamp(uv.x, 0.001, 0.999), clamp(uv.y, 0.001, 0.999));

	vec3 N = normalize(normal);
	vec3 A = normalize(angle0);
	vec3 c = texture(reflect, uv).rgb;
	FragColor = vec4(c, 1.0 - 0.9*abs(dot(N, A)));
}