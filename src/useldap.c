/*
 * Copyright © 2015 Wikimedia Foundation, Inc.
 * Copyright © 2015 Marc-André Pelletier <mpelletier@wikimedia.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <nss.h>
#include <sys/time.h>
#include <unistd.h>


// This alters the nsswitch settings for the linked-with executable
// (by forcing execution before the main program) to use LDAP
// regardless of the system setting.

// The nscd daemon may operate under different rules, and it is
// therefore important that it not be used by the executable.  There
// is no public hook to disable using nscd if it is present, but there
// is a private function that can be used to do so (it is used
// by the nscd daemon itself to avoid recursion to itself).

// This defines the function, a faux callback that it requires, and
// the data structured passed to the later as cribbed from the
// nscd and glibc sources (viz. nss/nssqitch.h and nscd/nscd.h)

struct traced_file;
extern void __nss_disable_nscd (void (*) (size_t, struct traced_file *));

static void _faux_register_file(size_t idx, struct traced_file *finfo)
{
}

// ldap is configured to be searched first to ensure that any user
// existing both locally and in LDAP use the latter values (as would
// be the case for members of operations)

static void __attribute__((constructor)) _do_useldap(void)
{
    __nss_configure_lookup("passwd", "ldap files");
    __nss_configure_lookup("group", "ldap files");
    __nss_disable_nscd(_faux_register_file);
}

