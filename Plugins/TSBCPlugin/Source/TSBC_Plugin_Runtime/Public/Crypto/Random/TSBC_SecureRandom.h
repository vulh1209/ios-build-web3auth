// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#pragma once
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4668)
#endif

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

#ifndef _WIN32
	#include <unistd.h>
#endif

#include <stdlib.h>
#include <sys/types.h>

#ifndef _WIN32
	#include <sys/stat.h>
	#include <sys/time.h>
#endif

#ifdef __linux__
#ifdef __dietlibc__
		#define _LINUX_SOURCE
#else
		#include <sys/syscall.h>
#endif
	#include <poll.h>
#endif

#ifdef _WIN32
#undef TEXT
#include <windows.h>
#undef TEXT
#define TEXT(x) TEXT_PASTE(x)
#define RtlGenRandom SystemFunction036
extern "C" BOOLEAN NTAPI RtlGenRandom(PVOID RandomBuffer, ULONG RandomBufferLength);
#pragma comment(lib, "advapi32.lib")
#endif

#ifndef SSIZE_MAX
#define SSIZE_MAX (SIZE_MAX / 2 - 1)
#endif

#ifdef _WIN32
#pragma warning(pop)
#endif

class TSBC_PLUGIN_RUNTIME_API CTSBC_SecureRandom
{
public:
    static void randombytes_sysrandom_buf(void* const buf, const size_t size);

#if !defined(__OpenBSD__) && !defined(__CloudABI__)
private:
    static bool initialized;
    static bool getrandom_available;
    static int random_data_source_fd;

#ifndef _WIN32
private:
static ssize_t safe_read(const int fd, void * const buf_, size_t size);

#if defined(__linux__) && !defined(USE_BLOCKING_RANDOM) && !defined(NO_BLOCKING_RANDOM_POLL)
    static int randombytes_block_on_dev_random(void);
#endif

    static int randombytes_sysrandom_random_dev_open(void);

#if defined(__dietlibc__) || (defined(SYS_getrandom) && defined(__NR_getrandom))
    static int _randombytes_linux_getrandom(void * const buf, const size_t size);
    static int randombytes_linux_getrandom(void * const buf_, size_t size);
#endif
#endif

private:
    static void randombytes_sysrandom_init(void);
    static void randombytes_sysrandom_stir(void);
    static void randombytes_sysrandom_stir_if_needed(void);
    static int randombytes_sysrandom_close(void);
#endif
};