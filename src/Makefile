.PHONY: all clean install uninstall

all: useldap.so

clean:
	rm -f useldap.so

distclean:
	rm -f useldap.so

useldap.so: useldap.c
	gcc -rdynamic -fPIC -shared -Wl,-soname,useldap.so -o useldap.so useldap.c

install: useldap.so rpc.mountd
	install -d $(DESTDIR)/usr/lib/nfsd-ldap
	install -m 0755 useldap.so $(DESTDIR)/usr/lib/nfsd-ldap/useldap.so
	install -d $(DESTDIR)/usr/sbin
	install -m 0755 rpc.mountd $(DESTDIR)/usr/sbin/rpc.mountd
