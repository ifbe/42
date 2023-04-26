layout(triangles) in;
//layout(line_strip, max_vertices = 6) out;
layout(triangle_strip, max_vertices = 6) out;
in mediump vec3 uvw[];
in mediump vec3 xyz[];
out mediump vec3 objxyz;
out mediump vec3 normal;
out mediump vec3 texuvw;
void main(){
	vec3 a = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
	vec3 n = normalize(cross(a, b));
	gl_Position = gl_in[0].gl_Position;
	objxyz = xyz[0];
	texuvw = uvw[0];
	normal = n;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position;
	objxyz = xyz[1];
	texuvw = uvw[1];
	normal = n;
	EmitVertex();
	gl_Position = gl_in[2].gl_Position;
	objxyz = xyz[2];
	texuvw = uvw[2];
	normal = n;
	EmitVertex();
	EndPrimitive();
}