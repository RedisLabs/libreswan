/*
 * header file for Libreswan library functions
 * Copyright (C) 1998, 1999, 2000  Henry Spencer.
 * Copyright (C) 1999, 2000, 2001  Richard Guy Briggs
 * Copyright (C) 2019 Andrew Cagney <cagney@gnu.org>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <https://www.gnu.org/licenses/lgpl-2.1.txt>.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
 * License for more details.
 *
 */

#ifndef IP_ADDRESS_H
#define IP_ADDRESS_H

#include "shunk.h"
#include "err.h"

extern bool log_ip; /* false -> redact (aka sanitize) ip addresses */

#include <netinet/in.h>		/* for struct sockaddr_in */
#ifdef HAVE_INET6_IN6_H
#include <netinet6/in6.h>	/* for struct sockaddr_in6 */
#endif

/*
 * XXX: These macros are obsolete.
 *
 * All the cases of code using the below can be reduced to
 * {in,in6}_addr and address_from_*_addr().
 */
#ifdef NEED_SIN_LEN
#define SET_V4(a)	{ (a).u.v4.sin_family = AF_INET; (a).u.v4.sin_len = sizeof(struct sockaddr_in); }
#define SET_V6(a)	{ (a).u.v6.sin6_family = AF_INET6; (a).u.v6.sin6_len = sizeof(struct sockaddr_in6); }
#else
#define SET_V4(a)	{ (a).u.v4.sin_family = AF_INET; }
#define SET_V6(a)	{ (a).u.v6.sin6_family = AF_INET6; }
#endif

struct lswlog;

/*
 * Basic data types for the address-handling functions.
 * ip_address and ip_subnet are supposed to be opaque types; do not
 * use their definitions directly, they are subject to change!
 */

typedef struct {
#if 0
	/*
	 * XXX: Embedding all of sockaddr_in* in ip_address is seems
	 * like overkill - it should only needs the fields below.
	 * Unfortunately code is directly manipulating other fields
	 * (sinin_port - use ip_endpoint).  So much for an immutable
	 * abstraction.
	 */
	const sa_family_t family; /* descriminator */
	const union {
		struct in_addr in;
		struct in6_addr in6;
	} u;
#else
	union {
		struct sockaddr_in v4;
		struct sockaddr_in6 v6;
	} u;
#endif
} ip_address;

ip_address address_from_in_addr(const struct in_addr *in);
ip_address address_from_in6_addr(const struct in6_addr *sin6);

/* network byte ordered */
int nportof(const ip_address *src);
ip_address nsetportof(int port, ip_address dst);

/* host byte ordered */
int hportof(const ip_address *src);
ip_address hsetportof(int port, const ip_address dst);

/* XXX: compatibility */
#define portof(SRC) nportof((SRC))
#define setportof(PORT, DST) { *(DST) = nsetportof(PORT, *(DST)); }

struct sockaddr *sockaddrof(const ip_address *src);
size_t sockaddrlenof(const ip_address *src);

/*
 * Convert an address to a string:
 *
 * This implements https://tools.ietf.org/html/rfc5952 where zeros in
 * the middle of an IPv6 address are suppressed.  If the IP address is
 * "sensitive" use *_address_sensitive().
 */

typedef struct {
	char buf[(4+1)*8/*0000:...*/ + 1/*\0*/ + 1/*CANARY*/];
} address_buf;

void jam_address(struct lswlog *buf, const ip_address *src);
const char *str_address(const ip_address *src, address_buf *dst);

/*
 * sensitive: don't print address when !log_ip
 *
 * reversed: in-addr format.

 * raw: This is not the format function you are looking for. For IPv6
 * include all zeros, vis :0:..:0:; when SEPC != '\0' use it as the
 * separator instead of '.' (IPv4) or ':' (IPv6).
 */

typedef struct {
	/* string includes NUL, add 1 for canary */
	char buf[sizeof("4.0.0.0.3.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.2.0.0.0.1.0.0.0.IP6.ARPA.") + 1];
}  address_reversed_buf;

void jam_address_sensitive(struct lswlog *buf, const ip_address *src);
void jam_address_reversed(struct lswlog *buf, const ip_address *src);
void jam_address_raw(struct lswlog *buf, const ip_address *src, char sepc);

const char *str_address_sensitive(const ip_address *src, address_buf *dst);
const char *str_address_reversed(const ip_address *src, address_reversed_buf *buf);
const char *str_address_raw(const ip_address *src, char sepc, address_buf *dst);

typedef address_buf ipstr_buf;
const char *ipstr(const ip_address *src, ipstr_buf *b);
const char *sensitive_ipstr(const ip_address *src, ipstr_buf *b);

/*
 * isvalidaddr(): true when ADDR contains some sort of IPv4 or IPv6
 * address.
 *
 * The relationship !isvalidaddr() IFF ipstr()=="<invalid>" is ment to
 * hold.  Both the *addrtot() (used by ipstr()) and *portof() seem to
 * use the same check.  hportof() just happens to be an easy way to
 * access it.
 *
 * The routine isanyaddr() isn't used as, in addition to "<invalid>"
 * it includes magic "any" IPv4 and IPv6 addresses.
 */

#define isvalidaddr(ADDR) (hportof(ADDR) >= 0)

/*
 * Raw address bytes as a shunk - since that does const.
 */
shunk_t address_as_shunk(const ip_address *address);

/*
 * Old style.
 */

/* looks up names in DNS */
extern err_t ttoaddr(const char *src, size_t srclen, int af, ip_address *dst);

/* does not look up names in DNS */
extern err_t ttoaddr_num(const char *src, size_t srclen, int af, ip_address *dst);

/* RFC 1886 old IPv6 reverse-lookup format is the bulkiest */
#define ADDRTOT_BUF     sizeof(address_reversed_buf)
extern err_t tnatoaddr(const char *src, size_t srclen, int af, ip_address *dst);
extern size_t addrtot(const ip_address *src, int format, char *buf, size_t buflen);

/* initializations */
extern err_t loopbackaddr(int af, ip_address *dst);
extern err_t unspecaddr(int af, ip_address *dst);
extern err_t anyaddr(int af, ip_address *dst);
extern err_t initaddr(const unsigned char *src, size_t srclen, int af,
	       ip_address *dst);
extern err_t add_port(int af, ip_address *addr, unsigned short port);

/* misc. conversions and related */
extern int addrtypeof(const ip_address *src);
extern size_t addrlenof(const ip_address *src);
extern size_t addrbytesptr_read(const ip_address *src, const unsigned char **dst);
extern size_t addrbytesptr_write(ip_address *src, unsigned char **dst);
extern size_t addrbytesof(const ip_address *src, unsigned char *dst, size_t dstlen);
extern int masktocount(const ip_address *src);

/* tests */
extern bool sameaddr(const ip_address *a, const ip_address *b);
extern int addrcmp(const ip_address *a, const ip_address *b);
extern bool sameaddrtype(const ip_address *a, const ip_address *b);
extern int isanyaddr(const ip_address *src);
extern int isunspecaddr(const ip_address *src);
extern int isloopbackaddr(const ip_address *src);

#endif
