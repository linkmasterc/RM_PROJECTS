#ifndef RM_REDEFINE_TYPES_H
#define RM_REDEFINE_TYPES_H

typedef unsigned char  		UCHAR8;                  /** defined for unsigned 8-bits integer variable 	    无符号8位整型变量       */
typedef signed   char  		SCHAR8;                  /** defined for signed 8-bits integer variable		    有符号8位整型变量       */
typedef unsigned short 		USHORT16;                /** defined for unsigned 16-bits integer variable 	    无符号16位整型变量      */
typedef signed   short 		SSHORT16;                /** defined for signed 16-bits integer variable 	    有符号16位整型变量      */
typedef unsigned int   		UINT32;                  /** defined for unsigned 32-bits integer variable 	    无符号32位整型变量      */
typedef int   				SINT32;                  /** defined for signed 32-bits integer variable         有符号32位整型变量      */
typedef float          		FP32;                    /** single precision floating point variable (32bits)   单精度浮点数（32位长度） */
typedef double         		DB64;                    /** double precision floating point variable (64bits)   双精度浮点数（64位长度） */

typedef UCHAR8            u8;                      /** defined for unsigned 8-bits integer variable 	        无符号8位整型变量  */
typedef USHORT16          u16;                     /** defined for unsigned 16-bits integer variable 	    无符号16位整型变量 */
typedef UINT32            u32;                     /** defined for unsigned 32-bits integer variable 	    无符号32位整型变量 */
typedef SCHAR8            s8;                      /** defined for unsigned 8-bits integer variable 	        无符号8位整型变量  */
typedef SSHORT16          s16;                     /** defined for unsigned 16-bits integer variable 	    无符号16位整型变量 */
typedef SINT32            s32;                     /** defined for unsigned 32-bits integer variable 	    无符号32位整型变量 */


    /* exact-width signed integer types */
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;


    /* exact-width unsigned integer types */
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;

typedef enum {FALSE = 0, TRUE = !FALSE} bool;

#endif


