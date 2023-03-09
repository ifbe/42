#ifndef _BIGWIRE_H
#define _BIGWIRE_H

#ifdef __cplusplus
extern "C" {
#endif




//one to one
void* samesrcprevdst(void*);
void* samesrcnextdst(void*);
void* samedstprevsrc(void*);
void* samedstnextsrc(void*);

//void* broadcast_allnear(void* name, int foottype, void* buf, int len);
//void* broadcast_foottype(void* name, int foottype, void* buf, int len);




//one to many




#ifdef __cplusplus
}
#endif

#endif