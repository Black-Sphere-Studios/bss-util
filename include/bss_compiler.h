// Copyright �2011 Black Sphere Studios
// For conditions of distribution and use, see copyright notice in "bss_util.h"

#ifndef __BSS_COMPILER_H__
#define __BSS_COMPILER_H__

// CPU Architecture (possible pre-defined macros found on http://predef.sourceforge.net/prearch.html)
#if defined(_M_X64) || defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_LP64)
#define BSS_CPU_x86_64  //x86-64 architecture
#define BSS_64BIT
#elif defined(__ia64__) || defined(_IA64) || defined(__IA64__) || defined(__ia64) || defined(_M_IA64)
#define BSS_CPU_IA_64 //Itanium (IA-64) architecture
#define BSS_64BIT
#elif defined(_M_IX86) || defined(__i386) || defined(__i386__) || defined(__X86__) || defined(_X86_) || defined(__I86__) || defined(__THW_INTEL__) || defined(__INTEL__)
#define BSS_CPU_x86  //x86 architecture
#define BSS_32BIT
#elif defined(__arm__) || defined(__thumb__) || defined(__TARGET_ARCH_ARM) || defined(__TARGET_ARCH_THUMB) || defined(_ARM)
#define BSS_CPU_ARM //ARM architecture
//#ifndef(???) //ARMv8 will support 64-bit so we'll have to detect that somehow
#define BSS_32BIT
//#else
//#define BSS_64BIT
//#endif
#elif defined(__mips__) || defined(mips) || defined(_MIPS_ISA) || defined(__mips) || defined(__MIPS__)
#define BSS_CPU_MIPS
#define BSS_64BIT
#elif defined(__powerpc) || defined(__powerpc__) || defined(__POWERPC__) || defined(__ppc__) || defined(_M_PPC) || defined(_ARCH_PPC)
#define BSS_CPU_POWERPC
#define BSS_32BIT
#else
#define BSS_CPU_UNKNOWN //Unknown CPU architecture (should force architecture independent C implementations for all utilities)
#endif

// Compiler detection and macro generation
#if defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC) // Intel C++ compiler
#define BSS_COMPILER_DLLEXPORT __declspec(dllexport)
#define BSS_COMPILER_DLLIMPORT __declspec(dllimport)
#define MEMBARRIER_READWRITE __memory_barrier()
#define MEMBARRIER_READ MEMBARRIER_READWRITE
#define MEMBARRIER_WRITE MEMBARRIER_READWRITE
#define BSS_COMPILER_FASTCALL
#define BSS_COMPILER_STDCALL
#define BSS_COMPILER_NAKED
#define BSS_FORCEINLINE

# elif defined __GNUC__ // GCC
#define BSS_COMPILER_DLLEXPORT __attribute__((dllexport))
#define BSS_COMPILER_DLLIMPORT __attribute__((dllimport))
#define MEMBARRIER_READWRITE __asm__ __volatile__ ("" ::: "memory");
#define MEMBARRIER_READ MEMBARRIER_READWRITE
#define MEMBARRIER_WRITE MEMBARRIER_READWRITE
#define BSS_COMPILER_FASTCALL __attribute__((fastcall))
#define BSS_COMPILER_STDCALL __attribute__((BSS_COMPILER_STDCALL__))
#define BSS_COMPILER_NAKED __attribute__((naked)) // Will only work on ARM, AVR, MCORE, RX and SPU. 
#define BSS_FORCEINLINE __attribute__((always_inline))

typedef char __int8;
typedef short __int16;
typedef int __int32;
typedef long long __int64;
//typedef __int128 __int128; // GCC doesn't have __int64/32/16/8, but it does have __int128 for whatever reason.

#elif defined _MSC_VER // VC++
#define BSS_COMPILER_DLLEXPORT __declspec(dllexport)
#define BSS_COMPILER_DLLIMPORT __declspec(dllimport)
#define MEMBARRIER_READWRITE _ReadWriteBarrier()
#define MEMBARRIER_READ _ReadBarrier()
#define MEMBARRIER_WRITE _WriteBarrier()
#define BSS_COMPILER_FASTCALL __fastcall
#define BSS_COMPILER_STDCALL __stdcall
#define BSS_COMPILER_NAKED __declspec(naked) 
#define BSS_FORCEINLINE __forceinline

#ifndef BSS_CPU_x86 // The only platform VC++ supports inline assembly on is x86, because its a piece of shit.
#define BSS_MSC_NOASM
#endif
#endif

// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#define BSS_PLATFORM_WIN32
#elif defined(_POSIX_VERSION) || defined(_XOPEN_VERSION)
#define BSS_PLATFORM_POSIX
#endif

#ifdef _WIN32_WCE
#define BSS_PLATFORM_WIN32_CE // Implies WIN32
#elif defined(__APPLE__) || defined(__MACH__)
#define BSS_PLATFORM_APPLE // Should also define POSIX, use only for Apple OS specific features
#elif defined(__CYGWIN__)
#define BSS_PLATFORM_CYGWIN // Should also define POSIX, use only to deal with Cygwin weirdness
#endif

#if defined(__linux__) || defined(__linux)
#define BSS_PLATFORM_LINUX // Should also define POSIX, use only for linux specific features
#endif

#if !(defined(BSS_PLATFORM_WIN32) || defined(BSS_PLATFORM_POSIX) || defined(BSS_PLATFORM_WIN32_CE) || defined(BSS_PLATFORM_APPLE))
#error "Unknown Platform"
#endif

#endif