#ifndef PHNX_COMMON_H
#define PHNX_COMMON_H

// Standard C Includes
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>

// Common type aliases
typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int8_t I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;

typedef float F32;
typedef double F64;

typedef char* cstr_t;
#define PX_STRINGIFIY(x) #x
#define PHNX_LOG(fmt, ...) do { printf("\x1b[32m[Phoenix::LOG] - "##fmt##"\x1b[39m\n", ##__VA_ARGS__); } while(0)
#define PHNX_DEBUG(fmt, ...) do { printf("\x1b[36m[Phoenix::DEBUG] - "##fmt##"\x1b[39m\n", ##__VA_ARGS__); } while(0)
#define PHNX_WARN(fmt, ...) do { printf("\x1b[33m[Phoenix::WARN] - "##fmt##"\x1b[39m\n", ##__VA_ARGS__); } while(0)
#define PHNX_FATAL(fmt, ...) do { printf("\x1b[37;41m[Phoenix::FATAL] - "##fmt##"\x1b[0m\n", ##__VA_ARGS__); exit(-1); } while(0)
#define PHNX_ASSERT_FMT(cond, fmt, ...) do { printf("\x1b[37;41m[Phoenix::ASSERT] - "##fmt##"\x1b[0m\n", ##__VA_ARGS__); __debugbreak(); } while(0)
#define PHNX_ASSERT(cond) do { if (!(cond)) { printf("\x1b[37;41m[Phoenix::ASSERT] - Condition '"##PX_STRINGIFIY(cond)##"' Failed\x1b[0m\n"); __debugbreak(); } } while(0)
#define PHNX_VMAJOR 1
#define PHNX_VMINOR 0

#define PHNX_NAME "Phoenix"





#endif // !PHNX_COMMON_H
