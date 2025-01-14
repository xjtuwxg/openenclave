// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

/**
 * @file exception.h
 *
 * This file defines data structures to set up vectored exception handlers in
 * the enclave.
 *
 */
#ifndef _OE_BITS_EXCEPTION_H
#define _OE_BITS_EXCEPTION_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/types.h>

OE_EXTERNC_BEGIN

/** Return value used by an enclave vectored exception handler to indicate
 *  to the dispatcher that it should continue searching for the next handler. */
#define OE_EXCEPTION_CONTINUE_SEARCH 0x0

/** Return value used by an enclave vectored exception handler to indicate
 *  to the dispatcher that it should stop searching and continue execution. */
#define OE_EXCEPTION_CONTINUE_EXECUTION 0xFFFFFFFF

/** Return value used by an enclave vectored exception handler to indicate
 *  to the dispatcher that it should stop searching and abort the execution. */
#define OE_EXCEPTION_ABORT_EXECUTION 0xFFFFFFF0

/**
 * Divider exception code, used by vectored exception handler.
 */
#define OE_EXCEPTION_DIVIDE_BY_ZERO 0x0
/**
 * Debug exception code, used by vectored exception handler.
 */
#define OE_EXCEPTION_BREAKPOINT 0x1
/**
 * Bound range exceeded exception code, used by vectored exception handler.
 */
#define OE_EXCEPTION_BOUND_OUT_OF_RANGE 0x2
/**
 * Illegal instruction exception code, used by vectored exception handler.
 */
#define OE_EXCEPTION_ILLEGAL_INSTRUCTION 0x3
/**
 * Access violation exception code, used by vectored exception handler.
 */
#define OE_EXCEPTION_ACCESS_VIOLATION 0x4
/**
 * Page fault exception code, used by vectored exception handler.
 */
#define OE_EXCEPTION_PAGE_FAULT 0x5
/**
 * x87 floating point exception code, used by vectored exception handler.
 */
#define OE_EXCEPTION_X87_FLOAT_POINT 0x6
/**
 * Alignment check exception code, used by vectored exception handler.
 */
#define OE_EXCEPTION_MISALIGNMENT 0x7
/**
 * SIMD floating point exception code, used by vectored exception handler.
 */
#define OE_EXCEPTION_SIMD_FLOAT_POINT 0x8
/**
 * Unknown exception code, used by vectored exception handler.
 */
#define OE_EXCEPTION_UNKNOWN 0xFFFFFFFF

/**
 * Hardware exception flag, set when enclave software exited due to hardware
 * exception
 */
#define OE_EXCEPTION_FLAGS_HARDWARE 0x1
/**
 * Software exception flag, set when enclave software exited due to software
 * exception
 */
#define OE_EXCEPTION_FLAGS_SOFTWARE 0x2

/**
 * The following flags are used to interpret the error_code of the
 * oe_exception_record_t struct when a PF or GP exception occurs. Note that
 * these exceptions are captured only with SGX2 CPU and the MISCSELCT[0] is set
 * to 1.
 */

/**
 * Page-protection violation flag
 * 0 - The fault was caused by a non-present page.
 * 1 - The fault was caused by a page-protection violation.
 */
#define OE_SGX_PAGE_FAULT_P_FLAG 0x1
/**
 * Read/Write flag
 * 0 - The fault was caused by a read.
 * 1 - The fault was caused by a write.
 */
#define OE_SGX_PAGE_FAULT_WR_FLAG 0x2
/**
 * U/S flag
 * 0 - The fault was caused by a supervisor-mode access.
 * 1 - The fault was caused by a user-mode access.
 */
#define OE_SGX_PAGE_FAULT_US_FLAG 0x4
/**
 * RSVD flag
 * 0 - The fault was not caused by a reserved bit violation.
 * 1 - The fault was caused by a reserved bit set to 1
 */
#define OE_SGX_PAGE_FAULT_RSVD 0x8
/**
 * I/D flag
 * 0 - The fault was not caused by an instruction fetch.
 * 1 - The fault was caused by an instruction fetch.
 */
#define OE_SGX_PAGE_FAULT_ID_FLAG 0x10
/**
 * Protection Key flag
 * 0 - The fault was not caused by protection keys.
 * 1 - The fault was caused by protection-key violation.
 */
#define OE_SGX_PAGE_FAULT_PK_FLAG 0x20
/**
 * SGX flag
 * 0 - The fault was not related to SGX.
 * 1 - The fault is SGX-specific (e.g., access violation).
 */
#define OE_SGX_PAGE_FAULT_SGX_FLAG 0x8000

/**
 * Blob that contains X87 and SSE data.
 */
typedef struct _oe_basic_xstate
{
    uint8_t blob[512]; /**< Holds XState i.e. X87 and SSE data */
} OE_ALIGNED(16) oe_basic_xstate_t;
/**< typedef struct _oe_basic_xstate oe_basic_xstate_t*/

/**
 * Register state to be saved before an exception and
 * restored after the exception has been handled in the enclave.
 */
typedef struct _oe_context
{
    /**
     * Exception flags.
     * OE_EXCEPTION_FLAGS_HARDWARE | OE_EXCEPTION_FLAGS_SOFTWARE
     */
    uint64_t flags;

    uint64_t rax; /**< Integer register rax */

    uint64_t rbx; /**< Integer register rbx */

    uint64_t rcx; /**< Integer register rcx */

    uint64_t rdx; /**< Integer register rdx */

    uint64_t rbp; /**< Integer register rbp */

    uint64_t rsp; /**< Integer register rsp */

    uint64_t rdi; /**< Integer register rdi */

    uint64_t rsi; /**< Integer register rsi */

    uint64_t r8; /**< Integer register r8 */

    uint64_t r9; /**< Integer register r9 */

    uint64_t r10; /**< Integer register r10 */

    uint64_t r11; /**< Integer register r11 */

    uint64_t r12; /**< Integer register r12 */

    uint64_t r13; /**< Integer register r13 */

    uint64_t r14; /**< Integer register r14 */

    uint64_t r15; /**< Integer register r15 */

    uint64_t rip; /**< Integer register rip */

    // Don't need to manipulate the segment registers directly.
    // Ignore them: CS, DS, ES, SS, GS, and FS.

    uint32_t mxcsr; /**< SSE control flags */

    oe_basic_xstate_t basic_xstate; /**< Basic XSTATE */

    // Don't need to manipulate other XSTATE (AVX etc.).
} oe_context_t;
/**< typedef struct _oe_context oe_context_t*/

/**
 * Exception context structure with the exception code, flags, address and
 * calling context of the exception.
 */
typedef struct _oe_exception_record
{
    uint32_t code; /**< Exception code */

    uint32_t flags; /**< Exception flags */

    uint64_t address; /**< Exception address */

    /* Information for PF/GP exceptions, which are only available
     * for SGX2 and requires application opt-in (set CapturePFGPExceptions=1).
     */
    uint64_t faulting_address;
    uint32_t error_code;

    oe_context_t* context; /**< Exception context */
} oe_exception_record_t;
/**< typedef struct _oe_exception_record oe_exception_record_t*/

/**
 * oe_vectored_exception_handler_t - Function pointer for a vectored exception
 * handler in an enclave.
 *
 * @param[in] exception_context The record of exception information to be
 * handled by the function which includes any flags, the failure code, faulting
 * address and calling context for the exception.
 */
typedef uint64_t (*oe_vectored_exception_handler_t)(
    oe_exception_record_t* exception_context);

OE_EXTERNC_END

#endif /* _OE_BITS_EXCEPTION_H */
