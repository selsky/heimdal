/*
 * Copyright (c) 1995, 1996 Kungliga Tekniska H�gskolan (Royal Institute
 * of Technology, Stockholm, Sweden).
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by the Kungliga Tekniska
 *      H�gskolan and its contributors.
 * 
 * 4. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"

RCSID("$Id$");
#endif

#include <stdlib.h>
#include <string.h>

#include "md4.h"

#ifndef min
#define min(a,b) (((a)>(b))?(b):(a))
#endif

#define A m->counter[0]
#define B m->counter[1]
#define C m->counter[2]
#define D m->counter[3]
#define X data

void
md4_init (struct md4 *m)
{
  m->offset = 0;
  m->sz = 0;
  D = 0x10325476;
  C = 0x98badcfe;
  B = 0xefcdab89;
  A = 0x67452301;
}

static u_int32_t
cshift (u_int32_t x, unsigned n)
{
  return (x << n) | (x >> (32 - n));
}

#define F(x,y,z) ((x & y) | (~x & z))
#define G(x,y,z) ((x & y) | (x & z) | (y & z))
#define H(x,y,z) (x ^ y ^ z)

#define DOIT(a,b,c,d,k,s,i,OP) \
a = cshift(a + OP(b,c,d) + X[k] + i, s)

#define DO1(a,b,c,d,k,s,i) DOIT(a,b,c,d,k,s,i,F)
#define DO2(a,b,c,d,k,s,i) DOIT(a,b,c,d,k,s,i,G)
#define DO3(a,b,c,d,k,s,i) DOIT(a,b,c,d,k,s,i,H)

static void
calc (struct md4 *m, u_int32_t *data)
{
  u_int32_t AA, BB, CC, DD;

  AA = A;
  BB = B;
  CC = C;
  DD = D;

  /* Round 1 */

  DO1(A,B,C,D,0,3,0);
  DO1(D,A,B,C,1,7,0);
  DO1(C,D,A,B,2,11,0);
  DO1(B,C,D,A,3,19,0);

  DO1(A,B,C,D,4,3,0);
  DO1(D,A,B,C,5,7,0);
  DO1(C,D,A,B,6,11,0);
  DO1(B,C,D,A,7,19,0);

  DO1(A,B,C,D,8,3,0);
  DO1(D,A,B,C,9,7,0);
  DO1(C,D,A,B,10,11,0);
  DO1(B,C,D,A,11,19,0);

  DO1(A,B,C,D,12,3,0);
  DO1(D,A,B,C,13,7,0);
  DO1(C,D,A,B,14,11,0);
  DO1(B,C,D,A,15,19,0);

  /* Round 2 */

  DO2(A,B,C,D,0,3,0x5A827999);
  DO2(D,A,B,C,4,5,0x5A827999);
  DO2(C,D,A,B,8,9,0x5A827999);
  DO2(B,C,D,A,12,13,0x5A827999);

  DO2(A,B,C,D,1,3,0x5A827999);
  DO2(D,A,B,C,5,5,0x5A827999);
  DO2(C,D,A,B,9,9,0x5A827999);
  DO2(B,C,D,A,13,13,0x5A827999);

  DO2(A,B,C,D,2,3,0x5A827999);
  DO2(D,A,B,C,6,5,0x5A827999);
  DO2(C,D,A,B,10,9,0x5A827999);
  DO2(B,C,D,A,14,13,0x5A827999);

  DO2(A,B,C,D,3,3,0x5A827999);
  DO2(D,A,B,C,7,5,0x5A827999);
  DO2(C,D,A,B,11,9,0x5A827999);
  DO2(B,C,D,A,15,13,0x5A827999);

  /* Round 3 */

  DO3(A,B,C,D,0,3,0x6ED9EBA1);
  DO3(D,A,B,C,8,9,0x6ED9EBA1);
  DO3(C,D,A,B,4,11,0x6ED9EBA1);
  DO3(B,C,D,A,12,15,0x6ED9EBA1);

  DO3(A,B,C,D,2,3,0x6ED9EBA1);
  DO3(D,A,B,C,10,9,0x6ED9EBA1);
  DO3(C,D,A,B,6,11,0x6ED9EBA1);
  DO3(B,C,D,A,14,15,0x6ED9EBA1);

  DO3(A,B,C,D,1,3,0x6ED9EBA1);
  DO3(D,A,B,C,9,9,0x6ED9EBA1);
  DO3(C,D,A,B,5,11,0x6ED9EBA1);
  DO3(B,C,D,A,13,15,0x6ED9EBA1);

  DO3(A,B,C,D,3,3,0x6ED9EBA1);
  DO3(D,A,B,C,11,9,0x6ED9EBA1);
  DO3(C,D,A,B,7,11,0x6ED9EBA1);
  DO3(B,C,D,A,15,15,0x6ED9EBA1);

  A += AA;
  B += BB;
  C += CC;
  D += DD;
}

/*
 * From `Performance analysis of MD5' by Joseph D. Touch <touch@isi.edu>
 */

static u_int32_t
swap_u_int32_t (u_int32_t t)
{
#if defined(WORDS_BIGENDIAN)
#define ROL(x,n) ((x)<<(n))|((x)>>(32-(n)))
  u_int32_t temp1, temp2;

  temp1   = ROL(t,16);
  temp2   = temp1 >> 8;
  temp1  &= 0x00ff00ff;
  temp2  &= 0x00ff00ff;
  temp1 <<= 8;
  return temp1 | temp2;
#else
  return t;
#endif
}

void
md4_update (struct md4 *m, void *v, size_t len)
{
  u_char *p = (u_char *)v;
  m->sz += len;
  if (m->offset == 0 && len % 64 == 0) 
    while (len > 0) {
#if defined(WORDS_BIGENDIAN)
      {
	int i;
	u_int32_t *u = (u_int32_t *)p;

	for (i = 0; i < 16; ++i)
	  m->current[i] = swap_u_int32_t (u[i]);
      }
      calc (m, m->current);
#else
      calc (m, (u_int32_t *)p);
#endif
      p += 64;
      len -= 64;
    }
  else
    while (len > 0) {
      unsigned l;

      l = min(64 - m->offset, len);
      memcpy ((char *)m->current + m->offset, p, l);
      p += l;
      len -= l;
      m->offset += l;
      if (m->offset == 64) {
#if defined(WORDS_BIGENDIAN)
	int i;

	for (i = 0; i < 16; ++i)
	  m->current[i] = swap_u_int32_t (m->current[i]);
#endif
	calc (m, m->current);
	m->offset = 0;
      }
    }
}

void
md4_finito (struct md4 *m, void *res)
{
  static u_char zeros[72];
  u_int32_t len;
  unsigned dstart = (120 - m->offset - 1) % 64 + 1;

  *zeros = 0x80;
  memset (zeros + 1, 0, sizeof(zeros) - 1);
  len = 8 * m->sz;
  len = swap_u_int32_t (len);
  memcpy (zeros + dstart, &len, sizeof(len));
  md4_update (m, zeros, dstart + 8);
  {
    int i;
    u_int32_t *r = (u_int32_t *)res;

    for (i = 0; i < 4; ++i)
      r[i] = swap_u_int32_t (m->counter[i]);
  }
}
