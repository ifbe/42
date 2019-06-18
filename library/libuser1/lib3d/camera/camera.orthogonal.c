#include "libuser.h"




static void ortho_v(mat4 m, struct fstyle* s)
{
	float cx = s->vc[0];
	float cy = s->vc[1];
	float cz = s->vc[2];

	m[0][0] = s->vr[0];
	m[0][1] = s->vr[1];
	m[0][2] = s->vr[2];
	m[0][3] = -(cx*m[0][0] + cy*m[0][1] + cz*m[0][2]);

	m[1][0] = s->vt[0];
	m[1][1] = s->vt[1];
	m[1][2] = s->vt[2];
	m[1][3] = -(cx*m[1][0] + cy*m[1][1] + cz*m[1][2]);

	m[2][0] =-s->vf[0];
	m[2][1] =-s->vf[1];
	m[2][2] =-s->vf[2];
	m[2][3] = -(cx*m[2][0] + cy*m[2][1] + cz*m[2][2]);

	m[3][0] = 0.0;
	m[3][1] = 0.0;
	m[3][2] = 0.0;
	m[3][3] = 1.0;
}
static void ortho_p(mat4 m, struct fstyle* s)
{
	float l = s->vl[3];
	float r = s->vr[3];
	float b = s->vb[3];
	float t = s->vt[3];
	float n = s->vn[3];
	float f = s->vf[3];

	m[0][0] = 2.0 / (r-l);
	m[0][1] = 0.0;
	m[0][2] = 0.0;
	m[0][3] = (l+r) / (l-r);

	m[1][0] = 0.0;
	m[1][1] = 2.0 / (t-b);
	m[1][2] = 0.0;
	m[1][3] = (b+t) / (b-t);

	m[2][0] = 0.0;
	m[2][1] = 0.0;
	m[2][2] = 2.0 / (n-f);
	m[2][3] = (n+f) / (n-f);

	m[3][0] = 0.0;
	m[3][1] = 0.0;
	m[3][2] = 0.0;
	m[3][3] = 1.0;
}
void ortho_mvp(mat4 m, struct fstyle* s)
{
	mat4 t;
	ortho_p(m, s);

	//ortho_o(t, s);
	//mat4_multiply(m, t);

	//ortho_p(t, s);
	//mat4_multiply(m, t);

	ortho_v(t, s);
	mat4_multiply(m, t);
}