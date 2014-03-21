PREFIX=usr/local
install:
	cat aproj.tmp | sed "s|SET_ANTARES_DIRECTORY|\"$$(dirname `pwd`)\"|g" > aproj
	install aproj $(DESTDIR)/$(PREFIX)/bin
