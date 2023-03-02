// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Crypto/Random/TSBC_SecureRandom.h"

#if defined(__OpenBSD__) || defined(__CloudABI__)
	void UTSBC_CryptoSecureRandomBytes::randombytes_sysrandom_buf(void* const buf, const size_t size)
	{
		return arc4random_buf(buf, size);
	}
#else /* !__OpenBSD__ */
bool CTSBC_SecureRandom::initialized = false;
bool CTSBC_SecureRandom::getrandom_available = false;
int CTSBC_SecureRandom::random_data_source_fd = -1;

#ifndef _WIN32
		ssize_t CTSBC_SecureRandom::safe_read(const int fd, void* const buf_, size_t size)
		{
			unsigned char* buf = (unsigned char*) buf_;
			ssize_t        readnb;

			assert(size > (size_t) 0U);
			assert(size <= SSIZE_MAX);
			do
			{
				while((readnb = read(fd, buf, size)) < (ssize_t) 0 && (errno == EINTR || errno == EAGAIN)); /* LCOV_EXCL_LINE */
				if(readnb < (ssize_t) 0)
				{
					return readnb; /* LCOV_EXCL_LINE */
				}
				if(readnb == (ssize_t) 0)
				{
					break; /* LCOV_EXCL_LINE */
				}
				size -= (size_t) readnb;
				buf += readnb;
			} while(size >(ssize_t) 0);

			return (ssize_t) (buf - (unsigned char*) buf_);
		}

#if defined(__linux__) && !defined(USE_BLOCKING_RANDOM) && !defined(NO_BLOCKING_RANDOM_POLL)
			int CTSBC_SecureRandom::randombytes_block_on_dev_random(void)
			{
				struct pollfd pfd;
				int           fd;
				int           pret;

				fd = open("/dev/random", O_RDONLY);
				if(fd == -1)
				{
					return 0;
				}
				pfd.fd = fd;
				pfd.events = POLLIN;
				pfd.revents = 0;
				do
				{
					pret = poll(&pfd, 1, -1);
				} while(pret < 0 && (errno == EINTR || errno == EAGAIN));
				if(pret != 1)
				{
					(void) close(fd);
					errno = EIO;
					return -1;
				}
				return close(fd);
			}
#endif

		int CTSBC_SecureRandom::randombytes_sysrandom_random_dev_open(void)
		{
			/* LCOV_EXCL_START */
			struct stat        st;
			static const char* devices[] = {
#ifndef USE_BLOCKING_RANDOM
					"/dev/urandom",
#endif
				"/dev/random",
				NULL
			};
			const char**       device = devices;
			int                fd;

#if defined(__linux__) && !defined(USE_BLOCKING_RANDOM) && !defined(NO_BLOCKING_RANDOM_POLL)
				if(randombytes_block_on_dev_random() != 0)
				{
					return -1;
				}
#endif

			do
			{
				fd = open(*device, O_RDONLY);
				if(fd != -1)
				{
					if
					(
						fstat(fd, &st) == 0 &&
#ifdef __COMPCERT__
							1
#elif defined(S_ISNAM)
							(S_ISNAM(st.st_mode) || S_ISCHR(st.st_mode))
#else
							S_ISCHR(st.st_mode)
#endif
					)
					{
#if defined(F_SETFD) && defined(FD_CLOEXEC)
							(void) fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);
#endif
						return fd;
					}
					(void) close(fd);
				}
				else if(errno == EINTR)
				{
					continue;
				}
				device++;
			} while(*device != NULL);

			errno = EIO;
			return -1;
			/* LCOV_EXCL_STOP */
		}

#if defined(__dietlibc__) || (defined(SYS_getrandom) && defined(__NR_getrandom))
			int CTSBC_SecureRandom::_randombytes_linux_getrandom(void* const buf, const size_t size)
			{
				int readnb;

				assert(size <= 256U);
				do
				{
#ifdef __dietlibc__
						readnb = getrandom(buf, size, 0);
#else
						readnb = syscall(SYS_getrandom, buf, (int) size, 0);
#endif
				} while(readnb < 0 && (errno == EINTR || errno == EAGAIN));

				return (readnb == (int) size) - 1;
			}

			int CTSBC_SecureRandom::randombytes_linux_getrandom(void* const buf_, size_t size)
			{
				unsigned char* buf = (unsigned char*) buf_;
				size_t         chunk_size = 256U;

				do
				{
					if(size < chunk_size)
					{
						chunk_size = size;
						assert(chunk_size >(size_t) 0U);
					}
					if(_randombytes_linux_getrandom(buf, chunk_size) != 0)
					{
						return -1;
					}
					size -= chunk_size;
					buf += chunk_size;
				} while(size > (size_t) 0U);

				return 0;
			}
#endif

		void CTSBC_SecureRandom::randombytes_sysrandom_init(void)
		{
			const int errno_save = errno;

#if defined(SYS_getrandom) && defined(__NR_getrandom)
				{
					unsigned char fodder[16];

					if(randombytes_linux_getrandom(fodder, sizeof fodder) == 0)
					{
						getrandom_available = true;
						errno = errno_save;
						return;
					}
					getrandom_available = false;
				}
#endif

			if((random_data_source_fd = randombytes_sysrandom_random_dev_open()) == -1)
			{
				abort(); /* LCOV_EXCL_LINE */
			}
			errno = errno_save;
		}
#else /* _WIN32 */
void CTSBC_SecureRandom::randombytes_sysrandom_init(void)
{
}
#endif

void CTSBC_SecureRandom::randombytes_sysrandom_stir(void)
{
    if(!initialized)
    {
        randombytes_sysrandom_init();
        initialized = true;
    }
}

void CTSBC_SecureRandom::randombytes_sysrandom_stir_if_needed(void)
{
    if(!initialized)
    {
        randombytes_sysrandom_stir();
    }
}

int CTSBC_SecureRandom::randombytes_sysrandom_close(void)
{
    int ret = -1;

#ifndef _WIN32
			if((random_data_source_fd != -1) && (close(random_data_source_fd) == 0))
			{
				random_data_source_fd = -1;
				initialized = false;
				ret = 0;
			}
#if defined(SYS_getrandom) && defined(__NR_getrandom)
				if(getrandom_available)
				{
					ret = 0;
				}
#endif
#else /* _WIN32 */
    if(initialized)
    {
        initialized = false;
        ret = 0;
    }
#endif
    return ret;
}

void CTSBC_SecureRandom::randombytes_sysrandom_buf(void* const buf, const size_t size)
{
    randombytes_sysrandom_stir_if_needed();
#if defined(ULONG_LONG_MAX) && defined(SIZE_MAX)
#if SIZE_MAX > ULONG_LONG_MAX
				/* coverity[result_independent_of_operands] */
				assert(size <= ULONG_LONG_MAX);
#endif
#endif
#ifndef _WIN32
#if defined(SYS_getrandom) && defined(__NR_getrandom)
				if(getrandom_available)
				{
					if(randombytes_linux_getrandom(buf, size) != 0)
					{
						abort();
					}
					return;
				}
#endif
			if(random_data_source_fd == -1 || safe_read(CTSBC_SecureRandom::random_data_source_fd, buf, size) != (ssize_t) size)
			{
				abort(); /* LCOV_EXCL_LINE */
			}
#else
    if(size > (size_t)0xffffffff)
    {
        abort(); /* LCOV_EXCL_LINE */
    }
    if(!RtlGenRandom((PVOID)buf, (ULONG)size))
    {
        abort(); /* LCOV_EXCL_LINE */
    }
#endif
}
#endif /* !__OpenBSD__ */