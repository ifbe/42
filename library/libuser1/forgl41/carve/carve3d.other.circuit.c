#include "libuser.h"




void gl41line_pmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt)
{
	int j,k;
	vec3 t1,t2;
	vec3 tc,tr,tf;

	//gate
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j];
		tr[j] = vc[j] -vr[j]*9/16;
	}
	gl41line(wnd, irgb, tc,tr);

	for(j=0;j<3;j++){
		t1[j] = vc[j] -vr[j]*9/16 -vf[j]/4;
		t2[j] = vc[j] -vr[j]*9/16 +vf[j]/4;
	}
	gl41line(wnd, irgb, t1, t2);

	//p,n,p
	for(k=0;k<3;k++){
		for(j=0;j<3;j++){
			t1[j] = vc[j] -vr[j]/2 + vf[j]*(3*k-4)/16;
			t2[j] = t1[j] +vf[j]*2/16;
		}
		gl41line(wnd, orgb, t1,t2);
	}

	//s,d,b
	for(k=-1;k<2;k+=2){
		for(j=0;j<3;j++){
			t1[j] = vc[j] - vr[j]/2 + vf[j]*k*3/16;
			t2[j] = vc[j] + vf[j]*k*3/16;
		}
		gl41line(wnd, orgb, t1,t2);
	}
	for(j=0;j<3;j++){
		t1[j] = vc[j] - vr[j]/2;
		t2[j] = vc[j];
	}
	gl41line_arrow(wnd, orgb, t1, t2, vt);

	//s,d
	for(j=0;j<3;j++){
		t1[j] = vc[j];
		t2[j] = vc[j] + vf[j];
	}
	gl41line(wnd, orgb, t1,t2);

	for(j=0;j<3;j++){
		t1[j] = vc[j] - vf[j]*3/16;
		t2[j] = vc[j] - vf[j];
	}
	gl41line(wnd, orgb, t1,t2);
}
void gl41line_nmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt)
{
	int j,k;
	vec3 t1,t2;
	vec3 tc,tr,tf;

	//gate
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j];
		tr[j] = vc[j] -vr[j]*9/16;
	}
	gl41line(wnd, irgb, tc,tr);

	for(j=0;j<3;j++){
		t1[j] = vc[j] -vr[j]*9/16 -vf[j]/4;
		t2[j] = vc[j] -vr[j]*9/16 +vf[j]/4;
	}
	gl41line(wnd, irgb, t1, t2);

	//p,n,p
	for(k=0;k<3;k++){
		for(j=0;j<3;j++){
			t1[j] = vc[j] -vr[j]/2 + vf[j]*(3*k-4)/16;
			t2[j] = t1[j] +vf[j]*2/16;
		}
		gl41line(wnd, orgb, t1,t2);
	}

	//s,d,b
	for(k=-1;k<2;k+=2){
		for(j=0;j<3;j++){
			t1[j] = vc[j] - vr[j]/2 + vf[j]*k*3/16;
			t2[j] = vc[j] + vf[j]*k*3/16;
		}
		gl41line(wnd, orgb, t1,t2);
	}
	for(j=0;j<3;j++){
		t1[j] = vc[j] - vr[j]/2;
		t2[j] = vc[j];
	}
	gl41line_arrow(wnd, orgb, t2, t1, vt);

	//s,d
	for(j=0;j<3;j++){
		t1[j] = vc[j] + vf[j]*3/16;
		t2[j] = vc[j] + vf[j];
	}
	gl41line(wnd, orgb, t1,t2);

	for(j=0;j<3;j++){
		t1[j] = vc[j];
		t2[j] = vc[j] - vf[j];
	}
	gl41line(wnd, orgb, t1,t2);
}
