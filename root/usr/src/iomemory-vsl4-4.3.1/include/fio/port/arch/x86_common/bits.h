//-----------------------------------------------------------------------------
// Copyright (c) 2006-2014, Fusion-io, Inc.(acquired by SanDisk Corp. 2014)
// Copyright (c) 2014 SanDisk Corp. and/or all its affiliates. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of the SanDisk Corp. nor the names of its contributors
//   may be used to endorse or promote products derived from this software
//   without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
// OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#ifndef _FIO_PORT_ARCH_x86_BITS_H
#define _FIO_PORT_ARCH_x86_BITS_H

#include <fio/port/compiler.h>

typedef uint32_t fusion_bits_t;

#if defined(_MSC_VER)  /* MS compiler for MS Windows and UEFI */
/**
 * @brief: returns 1 iff bit # v is set in *p else returns 0.
 */
static inline int fio_test_bit_atomic(int v, const volatile fusion_bits_t *p)
{
    return _bittest((LONG *) p, v );
}

/**
 * @brief atomic set of bit # v of *p.
 */
static inline void fio_set_bit_atomic(int v, volatile fusion_bits_t *p)
{
    (void)_interlockedbittestandset((LONG *) p, v);
}

/**
 * @brief atomic clear of bit # v of *p.
 */
static inline void fio_clear_bit_atomic(int v, volatile fusion_bits_t *p)
{
    (void)_interlockedbittestandreset((LONG *) p, v);
}

/**
 * @brief atomic set of bit # v of *p, returns old value.
 */
static inline unsigned char fio_test_and_set_bit_atomic(int v, volatile fusion_bits_t *p)
{
    return _interlockedbittestandset((LONG *) p, v);
}

/**
 * @brief atomic clear of bit # v of *p, returns old value.
 */
static inline unsigned char fio_test_and_clear_bit_atomic(int v, volatile fusion_bits_t *p)
{
    return _interlockedbittestandreset((LONG *) p, v);
}

/**
 * @brief: returns 1 if bit #v is set in *p else returns 0.
 */
static inline int fio_test_bit(int v, const fusion_bits_t *p)
{
    return _bittest((LONG *) p, v );
}

/**
 * @brief: non-atomic set of bit #v cwofin *p.
 */
static inline void fio_set_bit(int v, fusion_bits_t *p)
{
    (void)_bittestandset((LONG *) p, v);
}

/**
 * @brief non-atomic clear of bit #v of *p.
 */
static inline void fio_clear_bit(int v, fusion_bits_t *p)
{
    (void)_bittestandreset((LONG *) p, v);
}

/**
 * @brief non-atomic set of bit # v of *p, returns old value.
 */
static inline unsigned char fio_test_and_set_bit(int v, fusion_bits_t *p)
{
    return _bittestandset((LONG *) p, v);
}

/**
 * @brief non-atomic clear of bit # v of *p, returns old value.
 */
static inline unsigned char fio_test_and_clear_bit(int v, fusion_bits_t *p)
{
    return _interlockedbittestandreset((LONG *) p, v);
}

#elif defined(__GNUC__)  /* GCC for everything else */
/**
 * @brief: returns 1 iff bit # v is set in *p else returns 0.
 */
FIO_NONNULL_PARAMS static inline int fio_test_bit_atomic(int v, const volatile fusion_bits_t *p)
{
    return ((1U << (v & 31)) & *p) != 0;
}

/**
 * @brief atomic set of bit # v of *p.
 */
FIO_NONNULL_PARAMS static inline void fio_set_bit_atomic(int v, volatile fusion_bits_t *p)
{
    __asm__ __volatile__("\n\tlock; btsl %1, %0"
                         :"=m"(*p)
                         :"Ir"(v), "m"(*p)
                         : "cc", "memory" );

}

/**
 * @brief atomic clear of bit # v of *p.
 */
FIO_NONNULL_PARAMS static inline void fio_clear_bit_atomic(int v, volatile fusion_bits_t *p)
{
    __asm__ __volatile__("\n\tlock; btrl %1, %0"
                         :"=m"(*p)
                         :"Ir"(v), "m"(*p)
                         : "cc", "memory" );

}

/**
 * @brief atomic set of bit # v of *p, returns old value.
 */
FIO_NONNULL_PARAMS static inline unsigned char fio_test_and_set_bit_atomic(int v, volatile fusion_bits_t *p)
{
    unsigned char bit_set;

    __asm__ __volatile__("\n\tlock; btsl %2, %0"
                         "\n\tsetb %1"
                         :"=m"(*p), "=r"(bit_set)
                         :"Ir"(v), "m"(*p)
                         : "cc", "memory" );
    return bit_set;
}

/**
 * @brief atomic clear of bit # v of *p, returns old value.
 */
FIO_NONNULL_PARAMS static inline unsigned char fio_test_and_clear_bit_atomic(int v, volatile fusion_bits_t *p)
{
    unsigned char bit_set;

    __asm__ __volatile__("\n\tlock; btrl %2, %0"
                         "\n\tsetb %1"
                         :"=m"(*p), "=r"(bit_set)
                         :"Ir"(v), "m"(*p)
                         : "cc", "memory" );
    return bit_set;
}

/**
 * @brief atomic clear of bit # v of *p.
 */
FIO_NONNULL_PARAMS static inline void fio_clear_bit_atomic64(int v, volatile uint64_t *p)
{
    __asm__ __volatile__("\n\tlock; btrl %1, %0"
                         :"=m"(*p)
                         :"Jr"(v), "m"(*p)
                         : "cc", "memory" );

}

/**
 * @brief: returns 1 if bit #v is set in *p else returns 0.
 */
FIO_NONNULL_PARAMS static inline int fio_test_bit(int v, const fusion_bits_t *p)
{
    return ((1U << (v & 31)) & *p) != 0;
}

/**
 * @brief: non-atomic set of bit #v cwofin *p.
 */
FIO_NONNULL_PARAMS static inline void fio_set_bit(int v, fusion_bits_t *p)
{
    __asm__ ("\n\tbtsl %1, %0"
             :"=m"(*p)
             :"Ir"(v), "m"(*p)
             : "cc", "memory" );
}

/**
 * @brief non-atomic clear of bit #v of *p.
 */
FIO_NONNULL_PARAMS static inline void fio_clear_bit(int v, fusion_bits_t *p)
{
    __asm__ ("\n\t btrl %1, %0"
             :"=m"(*p)
             :"Ir"(v), "m"(*p)
             : "cc", "memory" );
}

/**
 * @brief non-atomic set of bit # v of *p, returns old value.
 */
FIO_NONNULL_PARAMS static inline unsigned char fio_test_and_set_bit(int v, fusion_bits_t *p)
{
    unsigned char bit_set;

    __asm__ ("\n\tbtsl %2, %0"
             "\n\tsetb %1"
             :"=m"(*p), "=r"(bit_set)
             :"Ir"(v), "m"(*p)
             : "cc", "memory" );
    return bit_set;
}

/**
 * @brief non-atomic clear of bit # v of *p, returns old value.
 */
FIO_NONNULL_PARAMS static inline unsigned char fio_test_and_clear_bit(int v, fusion_bits_t *p)
{
    unsigned char bit_set;

    __asm__ ("\n\tbtrl %2, %0"
             "\n\tsetb %1"
             :"=m"(*p), "=r"(bit_set)
             :"Ir"(v), "m"(*p)
             : "cc", "memory" );
    return bit_set;
}

#else /* Not GCC nor MS compiler */
#error "Please implement bit functions for your compiler."
#endif

#endif //_FIO_PORT_ARCH_x86_BITS_H
