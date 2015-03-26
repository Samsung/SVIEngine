#ifndef __SVI_BASE_TYPES_H_
#define __SVI_BASE_TYPES_H_

#ifdef __cplusplus

typedef char			SVIChar;
typedef char*			SVIPChar;

typedef char			SVIByte;
typedef unsigned char	SVIUByte;

typedef double			SVIDouble;
typedef float			SVIFloat;
typedef float			SVIClampf;
	
typedef short			SVIShort;
typedef unsigned short	SVIUShort;

typedef unsigned int	SVIUInt;
typedef int				SVIInt;

typedef bool			SVIBool;

typedef void			SVIVoid;
typedef void *			SVIPVoid;

#ifndef SVITRUE			
#define SVITRUE			true
#endif

#ifndef SVIFALSE			
#define SVIFALSE			false
#endif

#else

typedef char			SVIChar;
typedef char*			SVIPChar;

typedef char			SVIByte;
typedef unsigned char	SVIUByte;

typedef double			SVIDouble;
typedef float			SVIFloat;
	
typedef short			SVIShort;
typedef unsigned short	SVIUShort;

typedef unsigned int	SVIUInt;
typedef int				SVIInt;

typedef int				SVIBool;


typedef void			SVIVoid;
typedef void *			SVIPVoid;

#ifndef SVITRUE			
#define SVITRUE			1;
#endif

#ifndef SVIFALSE			
#define SVIFALSE			0;
#endif

#endif //__cplusplus

#endif //__SVI_BASE_TYPES_H_