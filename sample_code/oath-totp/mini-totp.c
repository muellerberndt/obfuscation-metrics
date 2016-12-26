/* mini-totp.c
 * Self-contained OATH-TOTP implementation with a hardcoded seed.
 * Bernhard Mueller - Vantage Point Security
 *
 * Copy/paste job based on:
 *
 * OATH-Toolkit
 * Copyright (C) 2011 Simon Josefsson
 * https://github.com/seryl/oath-toolkit
 *
 * sha1.c - Functions to compute SHA1 message digest of files or
 * Copyright (C) 2000-2001, 2003-2006, 2008-2011 Free Software Foundation, Inc.
 * Written by Scott G. Miller
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1, or (at your
 * option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this file; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define IPAD 0x36
#define OPAD 0x5c

#ifndef SHA1_H
#define SHA1_H 1

#include "sha1.h"
#include "hmac.h"
#include "oath.h"

#include <stdio.h>
#include <stdint.h>

# ifdef __cplusplus
extern "C" {
# endif

#define SHA1_DIGEST_SIZE 20
#define GC_SHA1_DIGEST_SIZE 20
#define OATH_TOTP_DEFAULT_TIME_STEP_SIZE  30

/* Structure to save state of computation between the single steps.  */
struct sha1_ctx
{
  uint32_t A;
  uint32_t B;
  uint32_t C;
  uint32_t D;
  uint32_t E;

  uint32_t total[2];
  uint32_t buflen;
  uint32_t buffer[32];
};

enum Gc_rc
{
  GC_OK = 0,
  GC_MALLOC_ERROR,
  GC_INIT_ERROR,
  GC_RANDOM_ERROR,
  GC_INVALID_CIPHER,
  GC_INVALID_HASH,
  GC_PKCS5_INVALID_ITERATION_COUNT,
  GC_PKCS5_INVALID_DERIVED_KEY_LENGTH,
  GC_PKCS5_DERIVED_KEY_TOO_LONG
};
typedef enum Gc_rc Gc_rc;

/* Initialize structure containing state of computation. */
extern void sha1_init_ctx (struct sha1_ctx *ctx);

/* Starting with the result of former calls of this function (or the
   initialization function update the context for the next LEN bytes
   starting at BUFFER.
   It is necessary that LEN is a multiple of 64!!! */
extern void sha1_process_block (const void *buffer, size_t len,
                                struct sha1_ctx *ctx);

/* Starting with the result of former calls of this function (or the
   initialization function update the context for the next LEN bytes
   starting at BUFFER.
   It is NOT required that LEN is a multiple of 64.  */
extern void sha1_process_bytes (const void *buffer, size_t len,
                                struct sha1_ctx *ctx);

/* Process the remaining bytes in the buffer and put result from CTX
   in first 20 bytes following RESBUF.  The result is always in little
   endian byte order, so that a byte-wise output yields to the wanted
   ASCII representation of the message digest.  */
extern void *sha1_finish_ctx (struct sha1_ctx *ctx, void *resbuf);


/* Put result from CTX in first 20 bytes following RESBUF.  The result is
   always in little endian byte order, so that a byte-wise output yields
   to the wanted ASCII representation of the message digest.  */
extern void *sha1_read_ctx (const struct sha1_ctx *ctx, void *resbuf);


/* Compute SHA1 message digest for bytes read from STREAM.  The
   resulting message digest number will be written into the 20 bytes
   beginning at RESBLOCK.  */
extern int sha1_stream (FILE *stream, void *resblock);

/* Compute SHA1 message digest for LEN bytes beginning at BUFFER.  The
   result is always in little endian byte order, so that a byte-wise
   output yields to the wanted ASCII representation of the message
   digest.  */
extern void *sha1_buffer (const char *buffer, size_t len, void *resblock);

# ifdef __cplusplus
}
# endif

#endif



void *
memxor (void * dest, const void * src, size_t n)
{
  char const *s = src;
  char *d = dest;

  for (; n > 0; n--)
    *d++ ^= *s++;

  return dest;
}


int
hmac_sha1 (const void *key, size_t keylen,
           const void *in, size_t inlen, void *resbuf)
{
  struct sha1_ctx inner;
  struct sha1_ctx outer;
  char optkeybuf[20];
  char block[64];
  char innerhash[20];

  /* Reduce the key's size, so that it becomes <= 64 bytes large.  */

  if (keylen > 64)
    {
      struct sha1_ctx keyhash;

      sha1_init_ctx (&keyhash);
      sha1_process_bytes (key, keylen, &keyhash);
      sha1_finish_ctx (&keyhash, optkeybuf);

      key = optkeybuf;
      keylen = 20;
    }

  /* Compute INNERHASH from KEY and IN.  */

  sha1_init_ctx (&inner);

  memset (block, IPAD, sizeof (block));
  memxor (block, key, keylen);

  sha1_process_block (block, 64, &inner);
  sha1_process_bytes (in, inlen, &inner);

  sha1_finish_ctx (&inner, innerhash);

  /* Compute result from KEY and INNERHASH.  */

  sha1_init_ctx (&outer);

  memset (block, OPAD, sizeof (block));
  memxor (block, key, keylen);

  sha1_process_block (block, 64, &outer);
  sha1_process_bytes (innerhash, 20, &outer);

  sha1_finish_ctx (&outer, resbuf);

  return 0;
}

int
oath_hotp_generate (const char *secret,
		    size_t secret_length,
		    uint64_t moving_factor,
		    unsigned digits,
		    bool add_checksum,
		    size_t truncation_offset, char *output_otp)
{
  char hs[GC_SHA1_DIGEST_SIZE];
  long S;

  (void) add_checksum;
  (void) truncation_offset;

  {
    char counter[sizeof (moving_factor)];
    size_t i;
    Gc_rc rc;

    for (i = 0; i < sizeof (counter); i++)
      counter[i] =
	(moving_factor >> ((sizeof (moving_factor) - i - 1) * 8)) & 0xFF;

    rc = hmac_sha1 (secret, secret_length,
		       counter, sizeof (moving_factor), hs);
    if (rc != GC_OK)
      return OATH_CRYPTO_ERROR;
  }

  {
    uint8_t offset = hs[sizeof (hs) - 1] & 0x0f;

    S = (((hs[offset] & 0x7f) << 24)
	 | ((hs[offset + 1] & 0xff) << 16)
	 | ((hs[offset + 2] & 0xff) << 8) | ((hs[offset + 3] & 0xff)));

#if DEBUG
    printf ("offset is %d hash is ", offset);
    for (i = 0; i < 20; i++)
      printf ("%02x ", hs[i] & 0xFF);
    printf ("\n");

    printf ("value: %d\n", S);
#endif
  }

  switch (digits)
    {
    case 6:
      S = S % 1000000;
      break;

    case 7:
      S = S % 10000000;
      break;

    case 8:
      S = S % 100000000;
      break;

    default:
      return OATH_INVALID_DIGITS;
      break;
    }

  {
    int len = snprintf (output_otp, digits + 1, "%.*ld", digits, S);
    output_otp[digits] = '\0';
    if (len <= 0 || ((unsigned) len) != digits)
      return OATH_PRINTF_ERROR;
  }

  return OATH_OK;
}





#ifdef WORDS_BIGENDIAN
# define SWAP(n) (n)
#else
# define SWAP(n) \
    (((n) << 24) | (((n) & 0xff00) << 8) | (((n) >> 8) & 0xff00) | ((n) >> 24))
#endif

#define BLOCKSIZE 32768
#if BLOCKSIZE % 64 != 0
# error "invalid BLOCKSIZE"
#endif

/* This array contains the bytes used to pad the buffer to the next
   64-byte boundary.  (RFC 1321, 3.1: Step 1)  */
static const unsigned char fillbuf[64] = { 0x80, 0 /* , 0, 0, ...  */ };


/* Take a pointer to a 160 bit block of data (five 32 bit ints) and
   initialize it to the start constants of the SHA1 algorithm.  This
   must be called before using hash in the call to sha1_hash.  */
void
sha1_init_ctx (struct sha1_ctx *ctx)
{
  ctx->A = 0x67452301;
  ctx->B = 0xefcdab89;
  ctx->C = 0x98badcfe;
  ctx->D = 0x10325476;
  ctx->E = 0xc3d2e1f0;

  ctx->total[0] = ctx->total[1] = 0;
  ctx->buflen = 0;
}

/* Copy the 4 byte value from v into the memory location pointed to by *cp,
   If your architecture allows unaligned access this is equivalent to
   * (uint32_t *) cp = v  */
static inline void
set_uint32 (char *cp, uint32_t v)
{
  memcpy (cp, &v, sizeof v);
}

/* Put result from CTX in first 20 bytes following RESBUF.  The result
   must be in little endian byte order.  */
void *
sha1_read_ctx (const struct sha1_ctx *ctx, void *resbuf)
{
  char *r = resbuf;
  set_uint32 (r + 0 * sizeof ctx->A, SWAP (ctx->A));
  set_uint32 (r + 1 * sizeof ctx->B, SWAP (ctx->B));
  set_uint32 (r + 2 * sizeof ctx->C, SWAP (ctx->C));
  set_uint32 (r + 3 * sizeof ctx->D, SWAP (ctx->D));
  set_uint32 (r + 4 * sizeof ctx->E, SWAP (ctx->E));

  return resbuf;
}

/* Process the remaining bytes in the internal buffer and the usual
   prolog according to the standard and write the result to RESBUF.  */
void *
sha1_finish_ctx (struct sha1_ctx *ctx, void *resbuf)
{
  /* Take yet unprocessed bytes into account.  */
  uint32_t bytes = ctx->buflen;
  size_t size = (bytes < 56) ? 64 / 4 : 64 * 2 / 4;

  /* Now count remaining bytes.  */
  ctx->total[0] += bytes;
  if (ctx->total[0] < bytes)
    ++ctx->total[1];

  /* Put the 64-bit file length in *bits* at the end of the buffer.  */
  ctx->buffer[size - 2] = SWAP ((ctx->total[1] << 3) | (ctx->total[0] >> 29));
  ctx->buffer[size - 1] = SWAP (ctx->total[0] << 3);

  memcpy (&((char *) ctx->buffer)[bytes], fillbuf, (size - 2) * 4 - bytes);

  /* Process last bytes.  */
  sha1_process_block (ctx->buffer, size * 4, ctx);

  return sha1_read_ctx (ctx, resbuf);
}

/* Compute SHA1 message digest for bytes read from STREAM.  The
   resulting message digest number will be written into the 16 bytes
   beginning at RESBLOCK.  */
int
sha1_stream (FILE *stream, void *resblock)
{
  struct sha1_ctx ctx;
  size_t sum;

  char *buffer = malloc (BLOCKSIZE + 72);
  if (!buffer)
    return 1;

  /* Initialize the computation context.  */
  sha1_init_ctx (&ctx);

  /* Iterate over full file contents.  */
  while (1)
    {
      /* We read the file in blocks of BLOCKSIZE bytes.  One call of the
         computation function processes the whole buffer so that with the
         next round of the loop another block can be read.  */
      size_t n;
      sum = 0;

      /* Read block.  Take care for partial reads.  */
      while (1)
        {
          n = fread (buffer + sum, 1, BLOCKSIZE - sum, stream);

          sum += n;

          if (sum == BLOCKSIZE)
            break;

          if (n == 0)
            {
              /* Check for the error flag IFF N == 0, so that we don't
                 exit the loop after a partial read due to e.g., EAGAIN
                 or EWOULDBLOCK.  */
              if (ferror (stream))
                {
                  free (buffer);
                  return 1;
                }
              goto process_partial_block;
            }

          /* We've read at least one byte, so ignore errors.  But always
             check for EOF, since feof may be true even though N > 0.
             Otherwise, we could end up calling fread after EOF.  */
          if (feof (stream))
            goto process_partial_block;
        }

      /* Process buffer with BLOCKSIZE bytes.  Note that
                        BLOCKSIZE % 64 == 0
       */
      sha1_process_block (buffer, BLOCKSIZE, &ctx);
    }

 process_partial_block:;

  /* Process any remaining bytes.  */
  if (sum > 0)
    sha1_process_bytes (buffer, sum, &ctx);

  /* Construct result in desired memory.  */
  sha1_finish_ctx (&ctx, resblock);
  free (buffer);
  return 0;
}

/* Compute SHA1 message digest for LEN bytes beginning at BUFFER.  The
   result is always in little endian byte order, so that a byte-wise
   output yields to the wanted ASCII representation of the message
   digest.  */
void *
sha1_buffer (const char *buffer, size_t len, void *resblock)
{
  struct sha1_ctx ctx;

  /* Initialize the computation context.  */
  sha1_init_ctx (&ctx);

  /* Process whole buffer but last len % 64 bytes.  */
  sha1_process_bytes (buffer, len, &ctx);

  /* Put result in desired memory area.  */
  return sha1_finish_ctx (&ctx, resblock);
}

void
sha1_process_bytes (const void *buffer, size_t len, struct sha1_ctx *ctx)
{
  /* When we already have some bits in our internal buffer concatenate
     both inputs first.  */
  if (ctx->buflen != 0)
    {
      size_t left_over = ctx->buflen;
      size_t add = 128 - left_over > len ? len : 128 - left_over;

      memcpy (&((char *) ctx->buffer)[left_over], buffer, add);
      ctx->buflen += add;

      if (ctx->buflen > 64)
        {
          sha1_process_block (ctx->buffer, ctx->buflen & ~63, ctx);

          ctx->buflen &= 63;
          /* The regions in the following copy operation cannot overlap.  */
          memcpy (ctx->buffer,
                  &((char *) ctx->buffer)[(left_over + add) & ~63],
                  ctx->buflen);
        }

      buffer = (const char *) buffer + add;
      len -= add;
    }

  /* Process available complete blocks.  */
  if (len >= 64)
    {
#if !_STRING_ARCH_unaligned
# define alignof(type) offsetof (struct { char c; type x; }, x)
# define UNALIGNED_P(p) (((size_t) p) % alignof (uint32_t) != 0)
      if (UNALIGNED_P (buffer))
        while (len > 64)
          {
            sha1_process_block (memcpy (ctx->buffer, buffer, 64), 64, ctx);
            buffer = (const char *) buffer + 64;
            len -= 64;
          }
      else
#endif
        {
          sha1_process_block (buffer, len & ~63, ctx);
          buffer = (const char *) buffer + (len & ~63);
          len &= 63;
        }
    }

  /* Move remaining bytes in internal buffer.  */
  if (len > 0)
    {
      size_t left_over = ctx->buflen;

      memcpy (&((char *) ctx->buffer)[left_over], buffer, len);
      left_over += len;
      if (left_over >= 64)
        {
          sha1_process_block (ctx->buffer, 64, ctx);
          left_over -= 64;
          memcpy (ctx->buffer, &ctx->buffer[16], left_over);
        }
      ctx->buflen = left_over;
    }
}

/* --- Code below is the primary difference between md5.c and sha1.c --- */

/* SHA1 round constants */
#define K1 0x5a827999
#define K2 0x6ed9eba1
#define K3 0x8f1bbcdc
#define K4 0xca62c1d6

/* Round functions.  Note that F2 is the same as F4.  */
#define F1(B,C,D) ( D ^ ( B & ( C ^ D ) ) )
#define F2(B,C,D) (B ^ C ^ D)
#define F3(B,C,D) ( ( B & C ) | ( D & ( B | C ) ) )
#define F4(B,C,D) (B ^ C ^ D)

/* Process LEN bytes of BUFFER, accumulating context into CTX.
   It is assumed that LEN % 64 == 0.
   Most of this code comes from GnuPG's cipher/sha1.c.  */

void
sha1_process_block (const void *buffer, size_t len, struct sha1_ctx *ctx)
{
  const uint32_t *words = buffer;
  size_t nwords = len / sizeof (uint32_t);
  const uint32_t *endp = words + nwords;
  uint32_t x[16];
  uint32_t a = ctx->A;
  uint32_t b = ctx->B;
  uint32_t c = ctx->C;
  uint32_t d = ctx->D;
  uint32_t e = ctx->E;

  /* First increment the byte count.  RFC 1321 specifies the possible
     length of the file up to 2^64 bits.  Here we only compute the
     number of bytes.  Do a double word increment.  */
  ctx->total[0] += len;
  if (ctx->total[0] < len)
    ++ctx->total[1];

#define rol(x, n) (((x) << (n)) | ((uint32_t) (x) >> (32 - (n))))

#define M(I) ( tm =   x[I&0x0f] ^ x[(I-14)&0x0f] \
                    ^ x[(I-8)&0x0f] ^ x[(I-3)&0x0f] \
               , (x[I&0x0f] = rol(tm, 1)) )

#define R(A,B,C,D,E,F,K,M)  do { E += rol( A, 5 )     \
                                      + F( B, C, D )  \
                                      + K             \
                                      + M;            \
                                 B = rol( B, 30 );    \
                               } while(0)

  while (words < endp)
    {
      uint32_t tm;
      int t;
      for (t = 0; t < 16; t++)
        {
          x[t] = SWAP (*words);
          words++;
        }

      R( a, b, c, d, e, F1, K1, x[ 0] );
      R( e, a, b, c, d, F1, K1, x[ 1] );
      R( d, e, a, b, c, F1, K1, x[ 2] );
      R( c, d, e, a, b, F1, K1, x[ 3] );
      R( b, c, d, e, a, F1, K1, x[ 4] );
      R( a, b, c, d, e, F1, K1, x[ 5] );
      R( e, a, b, c, d, F1, K1, x[ 6] );
      R( d, e, a, b, c, F1, K1, x[ 7] );
      R( c, d, e, a, b, F1, K1, x[ 8] );
      R( b, c, d, e, a, F1, K1, x[ 9] );
      R( a, b, c, d, e, F1, K1, x[10] );
      R( e, a, b, c, d, F1, K1, x[11] );
      R( d, e, a, b, c, F1, K1, x[12] );
      R( c, d, e, a, b, F1, K1, x[13] );
      R( b, c, d, e, a, F1, K1, x[14] );
      R( a, b, c, d, e, F1, K1, x[15] );
      R( e, a, b, c, d, F1, K1, M(16) );
      R( d, e, a, b, c, F1, K1, M(17) );
      R( c, d, e, a, b, F1, K1, M(18) );
      R( b, c, d, e, a, F1, K1, M(19) );
      R( a, b, c, d, e, F2, K2, M(20) );
      R( e, a, b, c, d, F2, K2, M(21) );
      R( d, e, a, b, c, F2, K2, M(22) );
      R( c, d, e, a, b, F2, K2, M(23) );
      R( b, c, d, e, a, F2, K2, M(24) );
      R( a, b, c, d, e, F2, K2, M(25) );
      R( e, a, b, c, d, F2, K2, M(26) );
      R( d, e, a, b, c, F2, K2, M(27) );
      R( c, d, e, a, b, F2, K2, M(28) );
      R( b, c, d, e, a, F2, K2, M(29) );
      R( a, b, c, d, e, F2, K2, M(30) );
      R( e, a, b, c, d, F2, K2, M(31) );
      R( d, e, a, b, c, F2, K2, M(32) );
      R( c, d, e, a, b, F2, K2, M(33) );
      R( b, c, d, e, a, F2, K2, M(34) );
      R( a, b, c, d, e, F2, K2, M(35) );
      R( e, a, b, c, d, F2, K2, M(36) );
      R( d, e, a, b, c, F2, K2, M(37) );
      R( c, d, e, a, b, F2, K2, M(38) );
      R( b, c, d, e, a, F2, K2, M(39) );
      R( a, b, c, d, e, F3, K3, M(40) );
      R( e, a, b, c, d, F3, K3, M(41) );
      R( d, e, a, b, c, F3, K3, M(42) );
      R( c, d, e, a, b, F3, K3, M(43) );
      R( b, c, d, e, a, F3, K3, M(44) );
      R( a, b, c, d, e, F3, K3, M(45) );
      R( e, a, b, c, d, F3, K3, M(46) );
      R( d, e, a, b, c, F3, K3, M(47) );
      R( c, d, e, a, b, F3, K3, M(48) );
      R( b, c, d, e, a, F3, K3, M(49) );
      R( a, b, c, d, e, F3, K3, M(50) );
      R( e, a, b, c, d, F3, K3, M(51) );
      R( d, e, a, b, c, F3, K3, M(52) );
      R( c, d, e, a, b, F3, K3, M(53) );
      R( b, c, d, e, a, F3, K3, M(54) );
      R( a, b, c, d, e, F3, K3, M(55) );
      R( e, a, b, c, d, F3, K3, M(56) );
      R( d, e, a, b, c, F3, K3, M(57) );
      R( c, d, e, a, b, F3, K3, M(58) );
      R( b, c, d, e, a, F3, K3, M(59) );
      R( a, b, c, d, e, F4, K4, M(60) );
      R( e, a, b, c, d, F4, K4, M(61) );
      R( d, e, a, b, c, F4, K4, M(62) );
      R( c, d, e, a, b, F4, K4, M(63) );
      R( b, c, d, e, a, F4, K4, M(64) );
      R( a, b, c, d, e, F4, K4, M(65) );
      R( e, a, b, c, d, F4, K4, M(66) );
      R( d, e, a, b, c, F4, K4, M(67) );
      R( c, d, e, a, b, F4, K4, M(68) );
      R( b, c, d, e, a, F4, K4, M(69) );
      R( a, b, c, d, e, F4, K4, M(70) );
      R( e, a, b, c, d, F4, K4, M(71) );
      R( d, e, a, b, c, F4, K4, M(72) );
      R( c, d, e, a, b, F4, K4, M(73) );
      R( b, c, d, e, a, F4, K4, M(74) );
      R( a, b, c, d, e, F4, K4, M(75) );
      R( e, a, b, c, d, F4, K4, M(76) );
      R( d, e, a, b, c, F4, K4, M(77) );
      R( c, d, e, a, b, F4, K4, M(78) );
      R( b, c, d, e, a, F4, K4, M(79) );

      a = ctx->A += a;
      b = ctx->B += b;
      c = ctx->C += c;
      d = ctx->D += d;
      e = ctx->E += e;
    }
}

static char secret[] = "1234567890123456";

int main(int argc, char *argv[]) {

	char output_otp[7];

	oath_totp_generate(secret, 16, time(NULL), 30, 0, 6, output_otp);
	output_otp[6] = 0;

	printf("OTP: %s\n", output_otp);

	return 0;
}


int
oath_totp_generate (const char *secret,
		    size_t secret_length,
		    time_t now,
		    unsigned time_step_size,
		    time_t start_offset, unsigned digits, char *output_otp)
{
  uint64_t nts;

  if (time_step_size == 0)
    time_step_size = OATH_TOTP_DEFAULT_TIME_STEP_SIZE;

  nts = (now - start_offset) / time_step_size;

  return oath_hotp_generate (secret,
			     secret_length,
			     nts,
			     digits,
			     false, OATH_HOTP_DYNAMIC_TRUNCATION, output_otp);
}
