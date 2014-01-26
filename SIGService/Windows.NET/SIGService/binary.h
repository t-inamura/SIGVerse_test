/* $Id: binary.h,v 1.4 2011-09-13 06:57:22 okamoto Exp $ */
#ifndef binary_h
#define binary_h

#ifdef WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif

typedef unsigned short DataLengthType;
typedef unsigned short DataOffsetType;

#define BINARY_STRING_EXT_LEN(STRLEN) ( ( 1 + (STRLEN)/4)*4 )
#define BINARY_STRING_LEN(STRLEN) ( 4 + BINARY_STRING_EXT_LEN(STRLEN) )

#define BINARY_SET_DATA_S(PTR, TYPE, V) *(TYPE*)(PTR) = htons(V)
#define BINARY_SET_DATA_L(PTR, TYPE, V) *(TYPE*)(PTR) = htonl(V)

#define BINARY_SET_DATA_S_INCR(PTR, TYPE, V) BINARY_SET_DATA_S(PTR, TYPE, V); PTR += sizeof(TYPE);
#define BINARY_SET_DATA_L_INCR(PTR, TYPE, V) BINARY_SET_DATA_L(PTR, TYPE, V); PTR += sizeof(TYPE);

#define BINARY_SET_STRING_INCR(PTR, STR) \
        { int SLEN = (STR != NULL)? strlen(STR): 0; BINARY_SET_DATA_L_INCR(PTR, unsigned int, SLEN); if (SLE\
N > 0) { strncpy(PTR, STR, SLEN); PTR += BINARY_STRING_EXT_LEN(SLEN); } }

#define BINARY_SET_FLOAT_INCR(PTR, V) { float v_ = V; memcpy(PTR, &v_, sizeof(V)); PTR += sizeof(V); }
#define BINARY_SET_DOUBLE_INCR(PTR, V) { double v_ = V; memcpy(PTR, &v_, sizeof(V)); PTR += sizeof(V); }
#define BINARY_SET_BOOL_INCR(PTR, V) { bool v_ = V; memcpy(PTR, &v_, sizeof (V)); PTR += sizeof(V); }


#define BINARY_GET_DATA_S(PTR, TYPE) ntohs( *(TYPE*)PTR )
#define BINARY_GET_DATA_S_INCR(PTR, TYPE) ntohs( *(TYPE*)PTR ); PTR += sizeof(TYPE)

#define BINARY_GET_DATA_L(PTR, TYPE) ntohl( *(TYPE*)PTR )
#define BINARY_GET_DATA_L_INCR(PTR, TYPE) ntohl( *(TYPE*)PTR ); PTR += sizeof(TYPE)

#define BINARY_GET_FLOAT(PTR) *(float*)PTR
#define BINARY_GET_FLOAT_INCR(PTR) BINARY_GET_FLOAT(PTR); PTR += sizeof(float);

#define BINARY_GET_DOUBLE(PTR) *(double*)PTR
#define BINARY_GET_DOUBLE_INCR(PTR) BINARY_GET_DOUBLE(PTR); PTR += sizeof(double);

#define BINARY_GET_BOOL(PTR) *(bool*)PTR
#define BINARY_GET_BOOL_INCR(PTR) BINARY_GET_BOOL(PTR); PTR += sizeof(bool);

char * binary_get_string_incr(char **ptr);
void   binary_free_string(char **p);

#define BINARY_GET_STRING_INCR(PTR) binary_get_string_incr(&PTR)

#define BINARY_SKIP_STRING(PTR) { char *PP_ = BINARY_GET_STRING_INCR(PTR); BINARY_FREE_STRING(PP_); }

#define BINARY_FREE_STRING(P) binary_free_string(&P)


#endif /* binary_h */
