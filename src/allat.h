#ifndef ALLAT_H
#define ALLAT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

// === Precise Types ===

typedef void U0;
typedef char C8;
typedef int8_t I8;
typedef uint8_t U8;
typedef int16_t I16;
typedef uint16_t U16;
typedef int32_t I32;
typedef uint32_t U32;
typedef int64_t I64;
typedef uint64_t U64;
typedef double F64;
typedef long double F128;

#define SCNI8 SCNd8
#define SCNU8 SCNu8
#define SCNI16 SCNd16
#define SCNU16 SCNu16
#define SCNI32 SCNd32
#define SCNU32 SCNu32
#define SCNI64 SCNd64
#define SCNU64 SCNu64
#define SCNF64 "lf"
#define SCNF128 "Lf"

#define PRII8 PRId8
#define PRIU8 PRIu8
#define PRII16 PRId16
#define PRIU16 PRIu16
#define PRII32 PRId32
#define PRIU32 PRIu32
#define PRII64 PRId64
#define PRIU64 PRIu64
#define PRIF64 "lf"
#define PRIF128 "Lf"

// === END Precise Types ===
//

#endif
