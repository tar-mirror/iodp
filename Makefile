VERSION=0.2.4
PACKAGE=iodp
prefix=/usr/local
EXTRA_DIST=$(AUTOMAN) install.list Makefile.pre
CLEANFILES=check.out

DISTFILES=README NEWS ChangeLog $(MAN) docheck check.exp \
  _alloc.h   _dns.h    _pathexec.h   _readwrite.h \
  _buffer.h  _getln.h  _readclose.h  _stralloc.h \
  _str.h _ip4.h  fifo.h \
  typesize.h iodptools.h close.h \
   _scan.h iodp_get.h _sig.h iprules.h \
   trysgact.c choose.sh iopause.sh \
  $(EXTRA_DIST)


TAR=tar
CC=gcc
L=./link.sh
C=./compile.sh
W=./compilew.sh
AR=ar
RANLIB=ranlib
GZIP=gzip --force --best
CLEANFILES+=distcheck.tmp distcheck.tmp2
#
PACKAGE_VERSION=$(PACKAGE)-$(VERSION)
INCLUDES=-DPACKAGE_VERSION=\"$(PACKAGE)-$(VERSION)\" \
 -DPACKAGE=\"$(PACKAGE)\" -DVERSION=\"$(VERSION)\"
#
# GNU standard variables
prefix=/usr/local
exec_prefix=$(prefix)
bindir=$(exec_prefix)/bin
sbindir=$(exec_prefix)/sbin
libexecdir=$(exec_prefix)/libexec
datadir=$(prefix)/share
sysconfdir=$(prefix)/etc
sharedstatedir=$(prefix)/com
localstatedir=$(prefix)/var
libdir=$(prefix)/lib
infodir=$(prefix)/info
lispdir=$(prefix)/emacs/site-lisp
includedir=$(prefix)/include
oldincludedir=/usr/include
mandir=$(prefix)/man
man1dir=$(mandir)/man1
man2dir=$(mandir)/man2
man3dir=$(mandir)/man3
man4dir=$(mandir)/man4
man5dir=$(mandir)/man5
man6dir=$(mandir)/man6
man7dir=$(mandir)/man7
man8dir=$(mandir)/man8
man9dir=$(mandir)/man9


#!S:seek.c:seek_set seek_cur seek_end:uo_fhandle.h
#!S:fifo_make.c:fifo_make:uo_fhandle.h
#!S:open.c:open_read open_trunc open_write open_append open_create \
#  open_read_nb open_write_nb:uo_fhandle.h
#!S:close_on_exe.c:close_on_exec:uo_fhandle.h
#!p:wait_pid.c:wait_pid.co:wait_pid.cp
#!p:wait_nohang.c:wait_nohang.co:wait_nohang.cp
#!p:wait.h:wait.ho:wait.hp

libdns_a_SOURCES= dns_ip.c dns_packet.c dns_domain.c dns_sortip.c \
dns_random.c dns_dfd.c dns_resolve.c dns_rcip.c dns_transmit.c \
dns_ipq.c dns_rcrw.c dns_name.c dns_nd.c dns_dtda.c 
libdns_a_OBJECTS=$(libdns_a_SOURCES:.c=.o)
 
libunix_a_SOURCES= error_str.c error.c socket_recv.c socket_peek.c \
socket_ntohs.c socket_htons.c socket_udp.c socket_bind.c \
socket_local.c socket_send.c socket_conn.c socket_tcp.c socket_connt.c \
socket_listen.c socket_bindr.c socket_ipopt.c socket_tcpde.c \
error_temp.c socket_dgrux.c socket_strux.c socket_binux.c \
socket_conux.c socket_accept.c iopause.c seek.c open_read.c \
readclose.c alloc.c alloc_re.c stralloc_opys.c stralloc_opyb.c \
stralloc_eady.c stralloc_free.c stralloc_pend.c stralloc_catb.c \
stralloc_cats.c stralloc_copy.c stralloc_cat.c stralloc_num.c buffer.c \
buffer_put.c buffer_get.c buffer_0.c buffer_1.c getln.c getln2.c \
fifo.c ndelay_on.c ndelay_off.c prot.c coe.c pathexec_run.c \
pathexec_env.c env_get.c openreadclose.c open_trunc.c fd_move.c \
fd_copy.c sig.c sig_catch.c 
libunix_a_OBJECTS=$(libunix_a_SOURCES:.c=.o)
 
libbyte_a_SOURCES= byte_copy.c byte_diff.c ip4_fmt.c ip4_scan.c \
byte_zero.c str_chr.c fmt_ulong.c byte_chr.c case_diffb.c \
case_lwrtab.c uint16_unpack.c uint16_pack.c uint32_unpack.c \
case_diffs.c str_len.c byte_cr.c scan_ulong.c str_start.c str_diff.c \
scan_long.c scan_sign.c str_rchr.c 
libbyte_a_OBJECTS=$(libbyte_a_SOURCES:.c=.o)
 
libuomisc_a_SOURCES= uogetopt.c str_ulong.c timing.c str2ulong.c \
bailout.c str2long.c str_ulong_fmt.c uogetopt_join.c 
libuomisc_a_OBJECTS=$(libuomisc_a_SOURCES:.c=.o)
 
libtai_a_SOURCES= taia_now.c taia_less.c taia_uint.c taia_add.c \
taia_pack.c tai_pack.c taia_sub.c taia_approx.c taia_frac.c \
taia_half.c tai_sub.c tai_now.c taia_tai.c 
libtai_a_OBJECTS=$(libtai_a_SOURCES:.c=.o)
 
libstrhash_a_SOURCES= strhash_hash.c strhash_crea.c strhash_ente.c \
strhash_look.c strhash_chng.c strhash_dest.c strhash_get.c 
libstrhash_a_OBJECTS=$(libstrhash_a_SOURCES:.c=.o)
 
libcdb_a_SOURCES= cdb.c cdb_hash.c 
libcdb_a_OBJECTS=$(libcdb_a_SOURCES:.c=.o)
 
#

LIBRARIES += libdns.a libunix.a libbyte.a libuomisc.a libtai.a \
libstrhash.a libcdb.a
 
MANS += 
 
EXTRA_DIST +=  hassgact.h1 hassgact.h2 typesize.sh gettimeofday.sh \
fsync.c fsync.sh socket.sh checktype.sh docheck
 
PODFILES += udpsocket.pod iodp-udp4.pod iodp-tcp4.pod \
uxdgramsocket.pod echoserver.pod uxstreamsocket.pod tcpsocket.pod \
iodp-uxdgram.pod iodp.pod iodp-file.pod iodp-uxstream.pod \
file-iodp.pod
 
TARGETS += udpsocket tcpsocket uxstreamsocket uxdgramsocket file-iodp \
echoserver uoinstall
 
KEEPTARGETS += udpsocket.8 iodp-udp4.5 iodp-tcp4.5 uxdgramsocket.8 \
echoserver.8 uxstreamsocket.8 tcpsocket.8 iodp-uxdgram.5 iodp.5 \
iodp-file.5 iodp-uxstream.5 file-iodp.8
 
DISTFILES += Makefile  dns_ip.c dns_packet.c dns_domain.c dns_sortip.c \
dns_random.c dns_dfd.c dns_resolve.c dns_rcip.c dns_transmit.c \
dns_ipq.c dns_rcrw.c dns_name.c dns_nd.c dns_dtda.c error_str.c \
error.c socket_recv.c socket_peek.c socket_ntohs.c socket_htons.c \
socket_udp.c socket_bind.c socket_local.c socket_send.c socket_conn.c \
socket_tcp.c socket_connt.c socket_listen.c socket_bindr.c \
socket_ipopt.c socket_tcpde.c error_temp.c socket_dgrux.c \
socket_strux.c socket_binux.c socket_conux.c socket_accept.c iopause.c \
seek.c open_read.c readclose.c alloc.c alloc_re.c stralloc_opys.c \
stralloc_opyb.c stralloc_eady.c stralloc_free.c stralloc_pend.c \
stralloc_catb.c stralloc_cats.c stralloc_copy.c stralloc_cat.c \
stralloc_num.c buffer.c buffer_put.c buffer_get.c buffer_0.c \
buffer_1.c getln.c getln2.c fifo.c ndelay_on.c ndelay_off.c prot.c \
coe.c pathexec_run.c pathexec_env.c env_get.c openreadclose.c \
open_trunc.c fd_move.c fd_copy.c sig.c sig_catch.c byte_copy.c \
byte_diff.c ip4_fmt.c ip4_scan.c byte_zero.c str_chr.c fmt_ulong.c \
byte_chr.c case_diffb.c case_lwrtab.c uint16_unpack.c uint16_pack.c \
uint32_unpack.c case_diffs.c str_len.c byte_cr.c scan_ulong.c \
str_start.c str_diff.c scan_long.c scan_sign.c str_rchr.c uogetopt.c \
str_ulong.c timing.c str2ulong.c bailout.c str2long.c str_ulong_fmt.c \
uogetopt_join.c taia_now.c taia_less.c taia_uint.c taia_add.c \
taia_pack.c tai_pack.c taia_sub.c taia_approx.c taia_frac.c \
taia_half.c tai_sub.c tai_now.c taia_tai.c strhash_hash.c \
strhash_crea.c strhash_ente.c strhash_look.c strhash_chng.c \
strhash_dest.c strhash_get.c cdb.c cdb_hash.c main.c udpsocket.c \
verb_socket.c move_fd.c set_ugid.c sock4.c iodp_set.c tcpsocket.c \
uxstrmsocket.c sockux.c uxdgrmsocket.c file-iodp.c echoserver.c \
iodp_get.c iprules.c uoinstall.c dns.h error.h socket.h byte.h \
ndelay.h readwrite.h socket_ux.h seek.h open.h readclose.h alloc.h \
stralloc.h gen_allocdefs.h buffer.h getln.h fifo.h prot.h coe.h \
pathexec.h env.h openreadclose.h fd.h sig.h ip4.h str.h fmt.h case.h \
uint16.h uint32.h scan.h uogetopt.h str_ulong.h timing.h str2num.h \
bailout.h taia.h tai.h typesize.h uint64.h strhash.h strhashi.h cdb.h \
gen_alloc.h  hassgact.h1 hassgact.h2 typesize.sh gettimeofday.sh \
fsync.c fsync.sh socket.sh checktype.sh docheck udpsocket.pod \
udpsocket.8 iodp-udp4.pod iodp-udp4.5 iodp-tcp4.pod iodp-tcp4.5 \
uxdgramsocket.pod uxdgramsocket.8 echoserver.pod echoserver.8 \
uxstreamsocket.pod uxstreamsocket.8 tcpsocket.pod tcpsocket.8 \
iodp-uxdgram.pod iodp-uxdgram.5 iodp.pod iodp.5 iodp-file.pod \
iodp-file.5 iodp-uxstream.pod iodp-uxstream.5 file-iodp.pod \
file-iodp.8 udpsocket.html iodp-udp4.html iodp-tcp4.html \
uxdgramsocket.html echoserver.html uxstreamsocket.html tcpsocket.html \
iodp-uxdgram.html iodp.html iodp-file.html iodp-uxstream.html \
file-iodp.html
 
DISTFILES += LICENSES.LOCAL
 
CLEANFILES += fsync.o fsync.lib gettimeofday.lib inet_aton.o \
inet_aton.lib socket.lib socklen_t.h socklen_t.stamp ssize_t.h \
ssize_t.stamp typesize2.h typesize2.stamp iopause.h hassgprm.h \
hassgact.h have_futimes.h utimes.o utimes.lib vfork.o vfork.lib \
udpsocket.8.t iodp-udp4.5.t iodp-tcp4.5.t uxdgramsocket.8.t \
echoserver.8.t uxstreamsocket.8.t tcpsocket.8.t iodp-uxdgram.5.t \
iodp.5.t iodp-file.5.t iodp-uxstream.5.t file-iodp.8.t \
udpsocket.html.t iodp-udp4.html.t iodp-tcp4.html.t \
uxdgramsocket.html.t echoserver.html.t uxstreamsocket.html.t \
tcpsocket.html.t iodp-uxdgram.html.t iodp.html.t iodp-file.html.t \
iodp-uxstream.html.t file-iodp.html.t pod2html-itemcache \
pod2html-dircache pod2htmi.x~~ pod2htmd.x~~
 
CLEANFILES += *.stamp
 
MAINTAINERCLEANFILES += udpsocket.8 iodp-udp4.5 iodp-tcp4.5 \
uxdgramsocket.8 echoserver.8 uxstreamsocket.8 tcpsocket.8 \
iodp-uxdgram.5 iodp.5 iodp-file.5 iodp-uxstream.5 file-iodp.8 \
udpsocket.html iodp-udp4.html iodp-tcp4.html uxdgramsocket.html \
echoserver.html uxstreamsocket.html tcpsocket.html iodp-uxdgram.html \
iodp.html iodp-file.html iodp-uxstream.html file-iodp.html
 

all: $(TARGETS) $(KEEPTARGETS)
 
iopause.h: iopause.sh compile.sh
	sh iopause.sh >iopause.t && mv iopause.t $@
hassgprm.h: hassgprm.h1 hassgprm.h2 compile.sh link.sh trysgprm.c \
choose.sh
	sh choose.sh cl trysgprm $@1 $@2 >$@.t && mv $@.t $@
hassgact.h: hassgact.h1 hassgact.h2 compile.sh link.sh trysgact.c \
choose.sh
	sh choose.sh cl trysgact $@1 $@2 >$@.t && mv $@.t $@
have_futimes.h: have_func.sh compile.sh link.sh
	sh have_func.sh futimes "" "futimes(0,0)" "" "" >$@.t && mv $@.t \
$@
libdns.a: $(libdns_a_OBJECTS)
	$(AR) cru $@ $(libdns_a_OBJECTS)
	$(RANLIB) $@
libunix.a: $(libunix_a_OBJECTS)
	$(AR) cru $@ $(libunix_a_OBJECTS)
	$(RANLIB) $@
libbyte.a: $(libbyte_a_OBJECTS)
	$(AR) cru $@ $(libbyte_a_OBJECTS)
	$(RANLIB) $@
libuomisc.a: $(libuomisc_a_OBJECTS)
	$(AR) cru $@ $(libuomisc_a_OBJECTS)
	$(RANLIB) $@
libtai.a: $(libtai_a_OBJECTS)
	$(AR) cru $@ $(libtai_a_OBJECTS)
	$(RANLIB) $@
libstrhash.a: $(libstrhash_a_OBJECTS)
	$(AR) cru $@ $(libstrhash_a_OBJECTS)
	$(RANLIB) $@
libcdb.a: $(libcdb_a_OBJECTS)
	$(AR) cru $@ $(libcdb_a_OBJECTS)
	$(RANLIB) $@
udpsocket:  main.o udpsocket.o verb_socket.o move_fd.o set_ugid.o \
sock4.o iodp_set.o libdns.a libuomisc.a libunix.a libuomisc.a libtai.a \
libbyte.a socket.lib gettimeofday.lib link.sh
	$L udpsocket  main.o udpsocket.o verb_socket.o move_fd.o set_ugid.o \
sock4.o iodp_set.o libdns.a libuomisc.a libunix.a libuomisc.a libtai.a \
libbyte.a `cat socket.lib` `cat gettimeofday.lib` 
tcpsocket:  main.o tcpsocket.o verb_socket.o move_fd.o set_ugid.o \
sock4.o iodp_set.o libdns.a libuomisc.a libunix.a libuomisc.a libtai.a \
libbyte.a socket.lib gettimeofday.lib link.sh
	$L tcpsocket  main.o tcpsocket.o verb_socket.o move_fd.o set_ugid.o \
sock4.o iodp_set.o libdns.a libuomisc.a libunix.a libuomisc.a libtai.a \
libbyte.a `cat socket.lib` `cat gettimeofday.lib` 
uxstreamsocket:  main.o uxstrmsocket.o verb_socket.o move_fd.o \
set_ugid.o sockux.o iodp_set.o libuomisc.a libunix.a libuomisc.a \
libtai.a libbyte.a socket.lib gettimeofday.lib link.sh
	$L uxstreamsocket  main.o uxstrmsocket.o verb_socket.o move_fd.o \
set_ugid.o sockux.o iodp_set.o libuomisc.a libunix.a libuomisc.a \
libtai.a libbyte.a `cat socket.lib` `cat gettimeofday.lib` \

uxdgramsocket:  main.o uxdgrmsocket.o verb_socket.o move_fd.o \
set_ugid.o sockux.o iodp_set.o libuomisc.a libunix.a libuomisc.a \
libtai.a libbyte.a socket.lib gettimeofday.lib link.sh
	$L uxdgramsocket  main.o uxdgrmsocket.o verb_socket.o move_fd.o \
set_ugid.o sockux.o iodp_set.o libuomisc.a libunix.a libuomisc.a \
libtai.a libbyte.a `cat socket.lib` `cat gettimeofday.lib` 
file-iodp:  main.o file-iodp.o move_fd.o set_ugid.o iodp_set.o \
libuomisc.a libunix.a libuomisc.a libtai.a libbyte.a gettimeofday.lib \
link.sh
	$L file-iodp  main.o file-iodp.o move_fd.o set_ugid.o iodp_set.o \
libuomisc.a libunix.a libuomisc.a libtai.a libbyte.a `cat \
gettimeofday.lib` 
echoserver:  echoserver.o iodp_get.o iprules.o libdns.a libuomisc.a \
libcdb.a libstrhash.a libunix.a libuomisc.a libtai.a libbyte.a \
gettimeofday.lib link.sh
	$L echoserver  echoserver.o iodp_get.o iprules.o libdns.a libuomisc.a \
libcdb.a libstrhash.a libunix.a libuomisc.a libtai.a libbyte.a `cat \
gettimeofday.lib` 
uoinstall:  uoinstall.o libuomisc.a libunix.a libbyte.a fsync.lib \
link.sh
	$L uoinstall  uoinstall.o libuomisc.a libunix.a libbyte.a `cat \
fsync.lib` 
udpsocket.8: udpsocket.pod
	pod2man --center iodp --release 0.2.4 udpsocket.pod >$@.t && \
	mv $@.t $@
iodp-udp4.5: iodp-udp4.pod
	pod2man --center iodp --release 0.2.4 iodp-udp4.pod >$@.t && \
	mv $@.t $@
iodp-tcp4.5: iodp-tcp4.pod
	pod2man --center iodp --release 0.2.4 iodp-tcp4.pod >$@.t && \
	mv $@.t $@
uxdgramsocket.8: uxdgramsocket.pod
	pod2man --center iodp --release 0.2.4 uxdgramsocket.pod >$@.t && \
	mv $@.t $@
echoserver.8: echoserver.pod
	pod2man --center iodp --release 0.2.4 echoserver.pod >$@.t && \
	mv $@.t $@
uxstreamsocket.8: uxstreamsocket.pod
	pod2man --center iodp --release 0.2.4 uxstreamsocket.pod >$@.t && \
\
	mv $@.t $@
tcpsocket.8: tcpsocket.pod
	pod2man --center iodp --release 0.2.4 tcpsocket.pod >$@.t && \
	mv $@.t $@
iodp-uxdgram.5: iodp-uxdgram.pod
	pod2man --center iodp --release 0.2.4 iodp-uxdgram.pod >$@.t && \
	mv $@.t $@
iodp.5: iodp.pod
	pod2man --center iodp --release 0.2.4 iodp.pod >$@.t && \
	mv $@.t $@
iodp-file.5: iodp-file.pod
	pod2man --center iodp --release 0.2.4 iodp-file.pod >$@.t && \
	mv $@.t $@
iodp-uxstream.5: iodp-uxstream.pod
	pod2man --center iodp --release 0.2.4 iodp-uxstream.pod >$@.t && \
	mv $@.t $@
file-iodp.8: file-iodp.pod
	pod2man --center iodp --release 0.2.4 file-iodp.pod >$@.t && \
	mv $@.t $@
udpsocket.html: udpsocket.pod
	pod2html --htmlroot=. --podpath=. --outfile $@.t --noindex --title \
"create an UDP socket" udpsocket.pod >$@.t && mv $@.t \
$@
iodp-udp4.html: iodp-udp4.pod
	pod2html --htmlroot=. --podpath=. --outfile $@.t --noindex --title \
"IODP for UDP over IPv4" iodp-udp4.pod >$@.t && mv $@.t \
$@
iodp-tcp4.html: iodp-tcp4.pod
	pod2html --htmlroot=. --podpath=. --outfile $@.t --noindex --title \
"IODP for TCP over IPv4" iodp-tcp4.pod >$@.t && mv $@.t \
$@
uxdgramsocket.html: uxdgramsocket.pod
	pod2html --htmlroot=. --podpath=. --outfile $@.t --noindex --title \
"create a unix domain datagram socket" uxdgramsocket.pod >$@.t && mv \
$@.t $@
echoserver.html: echoserver.pod
	pod2html --htmlroot=. --podpath=. --outfile $@.t --noindex --title \
"echo / discard service" echoserver.pod >$@.t && mv $@.t \
$@
uxstreamsocket.html: uxstreamsocket.pod
	pod2html --htmlroot=. --podpath=. --outfile $@.t --noindex --title \
"create a unix domain stream socket" uxstreamsocket.pod >$@.t && mv \
$@.t $@
tcpsocket.html: tcpsocket.pod
	pod2html --htmlroot=. --podpath=. --outfile $@.t --noindex --title \
"create an TCP socket" tcpsocket.pod >$@.t && mv $@.t \
$@
iodp-uxdgram.html: iodp-uxdgram.pod
	pod2html --htmlroot=. --podpath=. --outfile $@.t --noindex --title \
"IODP over unix domain datagram sockets" iodp-uxdgram.pod >$@.t && mv \
$@.t $@
iodp.html: iodp.pod
	pod2html --htmlroot=. --podpath=. --outfile $@.t --noindex --title \
"the IODP protocol" iodp.pod >$@.t && mv $@.t $@
iodp-file.html: iodp-file.pod
	pod2html --htmlroot=. --podpath=. --outfile $@.t --noindex --title \
"using IODP to describe file handles" iodp-file.pod >$@.t && mv $@.t \
$@
iodp-uxstream.html: iodp-uxstream.pod
	pod2html --htmlroot=. --podpath=. --outfile $@.t --noindex --title \
"IODP over unix domain stream sockets" iodp-uxstream.pod >$@.t && mv \
$@.t $@
file-iodp.html: file-iodp.pod
	pod2html --htmlroot=. --podpath=. --outfile $@.t --noindex --title \
"set IODP environment for files" file-iodp.pod >$@.t && mv $@.t \
$@
html:  udpsocket.html iodp-udp4.html iodp-tcp4.html uxdgramsocket.html \
echoserver.html uxstreamsocket.html tcpsocket.html iodp-uxdgram.html \
iodp.html iodp-file.html iodp-uxstream.html file-iodp.html \
$(EXTRA_HTML)

 
docclean:
	 rm -f   udpsocket.html iodp-udp4.html iodp-tcp4.html \
uxdgramsocket.html echoserver.html uxstreamsocket.html tcpsocket.html \
iodp-uxdgram.html iodp.html iodp-file.html iodp-uxstream.html \
file-iodp.html
 
doc:   udpsocket.html iodp-udp4.html iodp-tcp4.html uxdgramsocket.html \
echoserver.html uxstreamsocket.html tcpsocket.html iodp-uxdgram.html \
iodp.html iodp-file.html iodp-uxstream.html file-iodp.html
 
stralloc_copy.o: stralloc_copy.c byte.h stralloc.h gen_alloc.h compilew.sh
	$W  -c stralloc_copy.c
dns_resolve.o: dns_resolve.c iopause.h taia.h tai.h uint64.h \
 typesize.h typesize2.h byte.h dns.h stralloc.h gen_alloc.h compile.sh
	$C  -c dns_resolve.c
strhash_crea.o: strhash_crea.c strhashi.h uint32.h typesize.h \
 typesize2.h strhash.h compilew.sh
	$W  -c strhash_crea.c
move_fd.o: move_fd.c fd.h str2num.h str_ulong.h bailout.h iodptools.h \
 typesize.h typesize2.h uogetopt.h error.h compilew.sh
	$W  -c move_fd.c
taia_less.o: taia_less.c taia.h tai.h uint64.h typesize.h typesize2.h compilew.sh
	$W  -c taia_less.c
timing.o: timing.c timing.h compilew.sh
	$W  -c timing.c
getln.o: getln.c byte.h getln.h buffer.h stralloc.h gen_alloc.h compile.sh
	$C  -c getln.c
stralloc_opyb.o: stralloc_opyb.c stralloc.h gen_alloc.h byte.h compilew.sh
	$W  -c stralloc_opyb.c
socket_accept.o: socket_accept.c byte.h socket.h uint16.h typesize.h \
 typesize2.h socklen_t.h compilew.sh
	$W  -c socket_accept.c
case_diffb.o: case_diffb.c case.h compilew.sh
	$W  -c case_diffb.c
readclose.o: readclose.c readwrite.h error.h readclose.h stralloc.h \
 gen_alloc.h compile.sh
	$C  -c readclose.c
str_ulong.o: str_ulong.c str_ulong.h compilew.sh
	$W  -c str_ulong.c
sock4.o: sock4.c iodptools.h typesize.h typesize2.h uogetopt.h \
 bailout.h str2num.h str_ulong.h env.h _dns.h _stralloc.h gen_alloc.h \
 iopause.h taia.h tai.h uint64.h socket.h uint16.h error.h _ip4.h \
 fmt.h byte.h _buffer.h _readwrite.h prot.h ndelay.h _pathexec.h compilew.sh
	$W  -c sock4.c
strhash_get.o: strhash_get.c strhashi.h uint32.h typesize.h \
 typesize2.h compilew.sh
	$W  -c strhash_get.c
dns_domain.o: dns_domain.c error.h alloc.h case.h byte.h dns.h \
 stralloc.h gen_alloc.h iopause.h taia.h tai.h uint64.h typesize.h \
 typesize2.h compile.sh
	$C  -c dns_domain.c
byte_copy.o: byte_copy.c byte.h compilew.sh
	$W  -c byte_copy.c
case_lwrtab.o: case_lwrtab.c case.h compilew.sh
	$W  -c case_lwrtab.c
uogetopt.o: uogetopt.c _readwrite.h _str.h env.h scan.h uogetopt.h compilew.sh
	$W  -c uogetopt.c
getln2.o: getln2.c byte.h getln.h buffer.h stralloc.h gen_alloc.h compile.sh
	$C  -c getln2.c
stralloc_cat.o: stralloc_cat.c byte.h stralloc.h gen_alloc.h compilew.sh
	$W  -c stralloc_cat.c
socket_local.o: socket_local.c byte.h socket.h uint16.h typesize.h \
 typesize2.h socklen_t.h compilew.sh
	$W  -c socket_local.c
taia_tai.o: taia_tai.c taia.h tai.h uint64.h typesize.h typesize2.h compilew.sh
	$W  -c taia_tai.c
pathexec_run.o: pathexec_run.c error.h stralloc.h gen_alloc.h str.h \
 env.h pathexec.h compile.sh
	$C  -c pathexec_run.c
socket_binux.o: socket_binux.c byte.h socket_ux.h uint16.h typesize.h \
 typesize2.h _stralloc.h gen_alloc.h compilew.sh
	$W  -c socket_binux.c
dns_ip.o: dns_ip.c stralloc.h gen_alloc.h uint16.h typesize.h \
 typesize2.h byte.h dns.h iopause.h taia.h tai.h uint64.h compile.sh
	$C  -c dns_ip.c
openreadclose.o: openreadclose.c error.h open.h readclose.h stralloc.h \
 gen_alloc.h openreadclose.h compilew.sh
	$W  -c openreadclose.c
str2ulong.o: str2ulong.c str2num.h compilew.sh
	$W  -c str2ulong.c
error_temp.o: error_temp.c error.h compilew.sh
	$W  -c error_temp.c
taia_frac.o: taia_frac.c taia.h tai.h uint64.h typesize.h typesize2.h compilew.sh
	$W  -c taia_frac.c
dns_dtda.o: dns_dtda.c stralloc.h gen_alloc.h dns.h iopause.h taia.h \
 tai.h uint64.h typesize.h typesize2.h compile.sh
	$C  -c dns_dtda.c
uogetopt_join.o: uogetopt_join.c uogetopt.h compilew.sh
	$W  -c uogetopt_join.c
socket_conux.o: socket_conux.c byte.h socket_ux.h uint16.h typesize.h \
 typesize2.h _stralloc.h gen_alloc.h compilew.sh
	$W  -c socket_conux.c
socket_strux.o: socket_strux.c ndelay.h socket_ux.h uint16.h \
 typesize.h typesize2.h _stralloc.h gen_alloc.h compilew.sh
	$W  -c socket_strux.c
taia_pack.o: taia_pack.c taia.h tai.h uint64.h typesize.h typesize2.h compilew.sh
	$W  -c taia_pack.c
verb_socket.o: verb_socket.c iodptools.h typesize.h typesize2.h \
 uogetopt.h compilew.sh
	$W  -c verb_socket.c
uxstrmsocket.o: uxstrmsocket.c _stralloc.h gen_alloc.h error.h \
 uogetopt.h _alloc.h fd.h fmt.h env.h byte.h bailout.h iodptools.h \
 typesize.h typesize2.h coe.h pathexec.h compilew.sh
	$W  -c uxstrmsocket.c
buffer_1.o: buffer_1.c readwrite.h buffer.h compile.sh
	$C  -c buffer_1.c
socket_ntohs.o: socket_ntohs.c socket.h uint16.h typesize.h \
 typesize2.h compilew.sh
	$W  -c socket_ntohs.c
uoinstall.o: uoinstall.c error.h _buffer.h _str.h env.h str_ulong.h \
 str2num.h open.h seek.h _stralloc.h gen_alloc.h bailout.h _getln.h \
 buffer.h compilew.sh
	$W  -c uoinstall.c
iodp_get.o: iodp_get.c _stralloc.h gen_alloc.h env.h bailout.h _scan.h \
 fmt.h str.h iodp_get.h compilew.sh
	$W  -c iodp_get.c
str_start.o: str_start.c str.h compilew.sh
	$W  -c str_start.c
byte_cr.o: byte_cr.c byte.h compilew.sh
	$W  -c byte_cr.c
ndelay_off.o: ndelay_off.c ndelay.h compilew.sh
	$W  -c ndelay_off.c
str2long.o: str2long.c str2num.h compilew.sh
	$W  -c str2long.c
socket_connt.o: socket_connt.c _readwrite.h byte.h socket.h uint16.h \
 typesize.h typesize2.h socklen_t.h compilew.sh
	$W  -c socket_connt.c
tai_pack.o: tai_pack.c tai.h uint64.h typesize.h typesize2.h compilew.sh
	$W  -c tai_pack.c
scan_long.o: scan_long.c scan.h compilew.sh
	$W  -c scan_long.c
dns_packet.o: dns_packet.c error.h dns.h stralloc.h gen_alloc.h \
 iopause.h taia.h tai.h uint64.h typesize.h typesize2.h compile.sh
	$C  -c dns_packet.c
alloc.o: alloc.c alloc.h error.h compile.sh
	$C  -c alloc.c
tcpsocket.o: tcpsocket.c _stralloc.h gen_alloc.h error.h uogetopt.h \
 _alloc.h fd.h str2num.h env.h byte.h fmt.h bailout.h ip4.h socket.h \
 uint16.h typesize.h typesize2.h pathexec.h iodptools.h coe.h compilew.sh
	$W  -c tcpsocket.c
dns_random.o: dns_random.c dns.h stralloc.h gen_alloc.h iopause.h \
 taia.h tai.h uint64.h typesize.h typesize2.h uint32.h compile.sh
	$C  -c dns_random.c
scan_sign.o: scan_sign.c scan.h compilew.sh
	$W  -c scan_sign.c
coe.o: coe.c coe.h compilew.sh
	$W  -c coe.c
str_diff.o: str_diff.c str.h compilew.sh
	$W  -c str_diff.c
dns_nd.o: dns_nd.c byte.h fmt.h dns.h stralloc.h gen_alloc.h iopause.h \
 taia.h tai.h uint64.h typesize.h typesize2.h compile.sh
	$C  -c dns_nd.c
stralloc_pend.o: stralloc_pend.c alloc.h stralloc.h gen_alloc.h \
 gen_allocdefs.h compile.sh
	$C  -c stralloc_pend.c
stralloc_free.o: stralloc_free.c stralloc.h gen_alloc.h alloc.h compile.sh
	$C  -c stralloc_free.c
strhash_hash.o: strhash_hash.c strhashi.h uint32.h typesize.h \
 typesize2.h strhash.h compilew.sh
	$W  -c strhash_hash.c
taia_approx.o: taia_approx.c taia.h tai.h uint64.h typesize.h \
 typesize2.h compilew.sh
	$W  -c taia_approx.c
pathexec_env.o: pathexec_env.c stralloc.h gen_alloc.h alloc.h str.h \
 byte.h env.h pathexec.h compile.sh
	$C  -c pathexec_env.c
uint16_unpack.o: uint16_unpack.c uint16.h typesize.h typesize2.h compilew.sh
	$W  -c uint16_unpack.c
byte_diff.o: byte_diff.c byte.h compilew.sh
	$W  -c byte_diff.c
taia_add.o: taia_add.c taia.h tai.h uint64.h typesize.h typesize2.h compilew.sh
	$W  -c taia_add.c
uint16_pack.o: uint16_pack.c uint16.h typesize.h typesize2.h compilew.sh
	$W  -c uint16_pack.c
dns_transmit.o: dns_transmit.c socket.h uint16.h typesize.h \
 typesize2.h alloc.h error.h byte.h readwrite.h dns.h stralloc.h \
 gen_alloc.h iopause.h taia.h tai.h uint64.h compile.sh
	$C  -c dns_transmit.c
stralloc_catb.o: stralloc_catb.c stralloc.h gen_alloc.h byte.h compilew.sh
	$W  -c stralloc_catb.c
socket_recv.o: socket_recv.c byte.h socket.h uint16.h typesize.h \
 typesize2.h socklen_t.h compilew.sh
	$W  -c socket_recv.c
dns_rcrw.o: dns_rcrw.c taia.h tai.h uint64.h typesize.h typesize2.h \
 env.h byte.h str.h openreadclose.h stralloc.h gen_alloc.h dns.h \
 iopause.h compile.sh
	$C  -c dns_rcrw.c
ip4_fmt.o: ip4_fmt.c fmt.h ip4.h compilew.sh
	$W  -c ip4_fmt.c
env_get.o: env_get.c env.h compilew.sh
	$W  -c env_get.c
dns_sortip.o: dns_sortip.c byte.h dns.h stralloc.h gen_alloc.h \
 iopause.h taia.h tai.h uint64.h typesize.h typesize2.h compile.sh
	$C  -c dns_sortip.c
str_rchr.o: str_rchr.c str.h compilew.sh
	$W  -c str_rchr.c
strhash_dest.o: strhash_dest.c strhashi.h uint32.h typesize.h \
 typesize2.h strhash.h compilew.sh
	$W  -c strhash_dest.c
taia_sub.o: taia_sub.c taia.h tai.h uint64.h typesize.h typesize2.h compilew.sh
	$W  -c taia_sub.c
dns_name.o: dns_name.c stralloc.h gen_alloc.h uint16.h typesize.h \
 typesize2.h byte.h dns.h iopause.h taia.h tai.h uint64.h compile.sh
	$C  -c dns_name.c
str_ulong_fmt.o: str_ulong_fmt.c str_ulong.h compilew.sh
	$W  -c str_ulong_fmt.c
socket_listen.o: socket_listen.c socket.h uint16.h typesize.h \
 typesize2.h compilew.sh
	$W  -c socket_listen.c
buffer_put.o: buffer_put.c buffer.h str.h byte.h error.h compile.sh
	$C  -c buffer_put.c
bailout.o: bailout.c error.h bailout.h fmt.h compilew.sh
	$W  -c bailout.c
iodp_set.o: iodp_set.c _stralloc.h gen_alloc.h env.h fmt.h _pathexec.h \
 _str.h bailout.h iodptools.h typesize.h typesize2.h uogetopt.h compilew.sh
	$W  -c iodp_set.c
str_len.o: str_len.c str.h compilew.sh
	$W  -c str_len.c
strhash_chng.o: strhash_chng.c strhashi.h uint32.h typesize.h \
 typesize2.h strhash.h compilew.sh
	$W  -c strhash_chng.c
alloc_re.o: alloc_re.c alloc.h byte.h compile.sh
	$C  -c alloc_re.c
uint32_unpack.o: uint32_unpack.c uint32.h typesize.h typesize2.h compilew.sh
	$W  -c uint32_unpack.c
socket_bind.o: socket_bind.c byte.h socket.h uint16.h typesize.h \
 typesize2.h compilew.sh
	$W  -c socket_bind.c
fmt_ulong.o: fmt_ulong.c fmt.h compilew.sh
	$W  -c fmt_ulong.c
buffer.o: buffer.c buffer.h compile.sh
	$C  -c buffer.c
sig_catch.o: sig_catch.c sig.h hassgact.h compile.sh
	$C  -c sig_catch.c
fifo.o: fifo.c fifo.h compilew.sh
	$W  -c fifo.c
fd_move.o: fd_move.c fd.h compile.sh
	$C  -c fd_move.c
tai_sub.o: tai_sub.c tai.h uint64.h typesize.h typesize2.h compilew.sh
	$W  -c tai_sub.c
ndelay_on.o: ndelay_on.c ndelay.h compilew.sh
	$W  -c ndelay_on.c
main.o: main.c _stralloc.h gen_alloc.h error.h uogetopt.h _alloc.h \
 fd.h str2num.h env.h str_ulong.h bailout.h dns.h stralloc.h iopause.h \
 taia.h tai.h uint64.h typesize.h typesize2.h iodptools.h ip4.h \
 socket.h uint16.h _pathexec.h coe.h compilew.sh
	$W  -c main.c
set_ugid.o: set_ugid.c iodptools.h typesize.h typesize2.h uogetopt.h \
 bailout.h str2num.h str_ulong.h env.h _dns.h _stralloc.h gen_alloc.h \
 iopause.h taia.h tai.h uint64.h socket.h uint16.h error.h _ip4.h \
 byte.h _buffer.h _readwrite.h prot.h compilew.sh
	$W  -c set_ugid.c
stralloc_opys.o: stralloc_opys.c byte.h str.h stralloc.h gen_alloc.h compilew.sh
	$W  -c stralloc_opys.c
case_diffs.o: case_diffs.c case.h compilew.sh
	$W  -c case_diffs.c
echoserver.o: echoserver.c iopause.h taia.h tai.h uint64.h typesize.h \
 typesize2.h _alloc.h fmt.h iodp_get.h bailout.h error.h _readwrite.h \
 _ip4.h byte.h close.h socket.h uint16.h _sig.h _scan.h open.h \
 _stralloc.h gen_alloc.h str.h strhash.h uint32.h uogetopt.h iprules.h \
 dns.h stralloc.h compilew.sh
	$W  -c echoserver.c
iopause.o: iopause.c taia.h tai.h uint64.h typesize.h typesize2.h \
 iopause.h compile.sh
	$C  -c iopause.c
taia_now.o: taia_now.c taia.h tai.h uint64.h typesize.h typesize2.h compilew.sh
	$W  -c taia_now.c
socket_tcpde.o: socket_tcpde.c socket.h uint16.h typesize.h \
 typesize2.h socklen_t.h compilew.sh
	$W  -c socket_tcpde.c
error_str.o: error_str.c error.h compile.sh
	$C  -c error_str.c
dns_dfd.o: dns_dfd.c error.h alloc.h byte.h dns.h stralloc.h \
 gen_alloc.h iopause.h taia.h tai.h uint64.h typesize.h typesize2.h compile.sh
	$C  -c dns_dfd.c
stralloc_num.o: stralloc_num.c stralloc.h gen_alloc.h compile.sh
	$C  -c stralloc_num.c
dns_ipq.o: dns_ipq.c stralloc.h gen_alloc.h case.h byte.h str.h dns.h \
 iopause.h taia.h tai.h uint64.h typesize.h typesize2.h compile.sh
	$C  -c dns_ipq.c
ip4_scan.o: ip4_scan.c scan.h ip4.h compile.sh
	$C  -c ip4_scan.c
socket_dgrux.o: socket_dgrux.c ndelay.h socket_ux.h uint16.h \
 typesize.h typesize2.h _stralloc.h gen_alloc.h compilew.sh
	$W  -c socket_dgrux.c
error.o: error.c error.h compilew.sh
	$W  -c error.c
byte_zero.o: byte_zero.c byte.h compilew.sh
	$W  -c byte_zero.c
iprules.o: iprules.c cdb.h uint32.h typesize.h typesize2.h iprules.h \
 _stralloc.h gen_alloc.h compilew.sh
	$W  -c iprules.c
fd_copy.o: fd_copy.c fd.h compile.sh
	$C  -c fd_copy.c
cdb.o: cdb.c readwrite.h error.h seek.h byte.h cdb.h uint32.h \
 typesize.h typesize2.h compile.sh
	$C  -c cdb.c
byte_chr.o: byte_chr.c byte.h compilew.sh
	$W  -c byte_chr.c
socket_send.o: socket_send.c byte.h socket.h uint16.h typesize.h \
 typesize2.h compilew.sh
	$W  -c socket_send.c
tai_now.o: tai_now.c tai.h uint64.h typesize.h typesize2.h compile.sh
	$C  -c tai_now.c
taia_uint.o: taia_uint.c taia.h tai.h uint64.h typesize.h typesize2.h compilew.sh
	$W  -c taia_uint.c
strhash_ente.o: strhash_ente.c strhashi.h uint32.h typesize.h \
 typesize2.h strhash.h compilew.sh
	$W  -c strhash_ente.c
socket_tcp.o: socket_tcp.c ndelay.h socket.h uint16.h typesize.h \
 typesize2.h compilew.sh
	$W  -c socket_tcp.c
scan_ulong.o: scan_ulong.c scan.h compilew.sh
	$W  -c scan_ulong.c
cdb_hash.o: cdb_hash.c cdb.h uint32.h typesize.h typesize2.h compile.sh
	$C  -c cdb_hash.c
socket_bindr.o: socket_bindr.c byte.h socket.h uint16.h typesize.h \
 typesize2.h compilew.sh
	$W  -c socket_bindr.c
prot.o: prot.c prot.h compile.sh
	$C  -c prot.c
dns_rcip.o: dns_rcip.c taia.h tai.h uint64.h typesize.h typesize2.h \
 openreadclose.h stralloc.h gen_alloc.h byte.h ip4.h env.h dns.h \
 iopause.h compile.sh
	$C  -c dns_rcip.c
socket_udp.o: socket_udp.c ndelay.h socket.h uint16.h typesize.h \
 typesize2.h compilew.sh
	$W  -c socket_udp.c
sig.o: sig.c sig.h compile.sh
	$C  -c sig.c
buffer_get.o: buffer_get.c buffer.h byte.h error.h compile.sh
	$C  -c buffer_get.c
strhash_look.o: strhash_look.c strhashi.h uint32.h typesize.h \
 typesize2.h strhash.h compilew.sh
	$W  -c strhash_look.c
uxdgrmsocket.o: uxdgrmsocket.c error.h uogetopt.h fd.h str2num.h \
 str_ulong.h bailout.h iodptools.h typesize.h typesize2.h socket.h \
 uint16.h socket_ux.h _stralloc.h gen_alloc.h coe.h compilew.sh
	$W  -c uxdgrmsocket.c
buffer_0.o: buffer_0.c readwrite.h buffer.h compile.sh
	$C  -c buffer_0.c
stralloc_eady.o: stralloc_eady.c alloc.h stralloc.h gen_alloc.h \
 gen_allocdefs.h compile.sh
	$C  -c stralloc_eady.c
seek.o: seek.c seek.h compilew.sh
	$W  -c seek.c
socket_peek.o: socket_peek.c byte.h socket.h uint16.h typesize.h \
 typesize2.h socklen_t.h compilew.sh
	$W  -c socket_peek.c
socket_ipopt.o: socket_ipopt.c socket.h uint16.h typesize.h \
 typesize2.h compilew.sh
	$W  -c socket_ipopt.c
udpsocket.o: udpsocket.c _stralloc.h gen_alloc.h error.h uogetopt.h \
 _alloc.h fd.h str2num.h env.h str_ulong.h bailout.h dns.h stralloc.h \
 iopause.h taia.h tai.h uint64.h typesize.h typesize2.h iodptools.h \
 ip4.h socket.h uint16.h _pathexec.h coe.h compilew.sh
	$W  -c udpsocket.c
sockux.o: sockux.c iodptools.h typesize.h typesize2.h uogetopt.h \
 bailout.h str2num.h str_ulong.h env.h str.h _dns.h _stralloc.h \
 gen_alloc.h iopause.h taia.h tai.h uint64.h socket.h uint16.h \
 socket_ux.h error.h ip4.h byte.h _buffer.h prot.h ndelay.h \
 _pathexec.h fmt.h compilew.sh
	$W  -c sockux.c
socket_htons.o: socket_htons.c socket.h uint16.h typesize.h \
 typesize2.h compilew.sh
	$W  -c socket_htons.c
str_chr.o: str_chr.c str.h compilew.sh
	$W  -c str_chr.c
socket_conn.o: socket_conn.c byte.h socket.h uint16.h typesize.h \
 typesize2.h compilew.sh
	$W  -c socket_conn.c
file-iodp.o: file-iodp.c _stralloc.h gen_alloc.h error.h uogetopt.h \
 _alloc.h fd.h str2num.h byte.h fmt.h bailout.h pathexec.h iodptools.h \
 typesize.h typesize2.h fifo.h _str.h seek.h compilew.sh
	$W  -c file-iodp.c
stralloc_cats.o: stralloc_cats.c byte.h str.h stralloc.h gen_alloc.h compilew.sh
	$W  -c stralloc_cats.c
taia_half.o: taia_half.c taia.h tai.h uint64.h typesize.h typesize2.h compile.sh
	$C  -c taia_half.c
open_read.o: open_read.c open.h compilew.sh
	$W  -c open_read.c
open_trunc.o: open_trunc.c open.h compilew.sh
	$W  -c open_trunc.c
typesize2.h: typesize.stamp
typesize.stamp: typesize.sh compile.sh link.sh
	CC=$(CC) sh typesize.sh >typesize.t
	@if cmp typesize.t typesize2.h 2>/dev/null >/dev/null ; then : ; else mv typesize.t typesize2.h ; fi
	rm -f typesize.t
	touch $@

gettimeofday.lib: gettimeofday.sh compile.sh link.sh
	sh gettimeofday.sh >gettimeofday.t && mv gettimeofday.t $@

fsync.lib: fsync.sh compile.sh link.sh
	sh fsync.sh >fsync.t && mv fsync.t $@

socket.lib: socket.sh compile.sh link.sh
	sh socket.sh >socket.t && mv socket.t $@

ssize_t.h: ssize_t.stamp
ssize_t.stamp: checktype.sh compile.sh link.sh
	CC=$(CC) sh checktype.sh ssize_t int sys/types.h >ssize_t.t
	@if cmp ssize_t.t ssize_t.h 2>/dev/null >/dev/null ; then : ; else mv ssize_t.t ssize_t.h ; fi
	rm -f ssize_t.t && touch $@

socklen_t.h: socklen_t.stamp
socklen_t.stamp: checktype.sh compile.sh link.sh
	CC=$(CC) sh checktype.sh socklen_t int sys/types.h >socklen_t.t
	@if cmp socklen_t.t socklen_t.h 2>/dev/null >/dev/null ; then : ; else mv socklen_t.t socklen_t.h ; fi
	rm -f socklen_t.t
	@touch $@

.PRECIOUS: Makefile
remake: clean
	rm -f compile.sh link.sh compilew.sh
compile.sh: 
	@echo remaking $@
	@( echo "#! /bin/sh" ; \
	echo '# automatically generated, do not edit' ; \
	echo 'CFLAGS="$${CFLAGS:-$$_CFLAGS_OG}"' ; \
	echo 'CFLAGS="$${CFLAGS:--O2}"' ; \
	echo 'DEFS="$${DEFS:--${DEFS}}"' ; \
	echo 'INCLUDES="$${INCLUDES:--${INCLUDES}}"' ; \
	echo 'CC="$${CC:-gcc}"' ; \
	echo 'exec $$CC $(DEFS) $(INCLUDES) $$CFLAGS $${1+"$$@"}' ; \
	) > compile.t && chmod 755 compile.t
	@mv compile.t $@
compilew.sh: 
	@echo remaking $@
	@( echo "#! /bin/sh" ; \
	echo '# automatically generated, do not edit' ; \
	echo 'CFLAGS="$${CFLAGS:-$$_CFLAGS_OWG}"' ; \
	echo 'CFLAGS="$${CFLAGS:--O2 -Wall}"' ; \
	echo 'DEFS="$${DEFS:--${DEFS}}"' ; \
	echo 'INCLUDES="$${INCLUDES:--${INCLUDES}}"' ; \
	echo 'CC="$${CC:-gcc}"' ; \
	echo 'exec $$CC $(DEFS) $(INCLUDES) $$CFLAGS $${1+"$$@"}' ; \
	) > compilew.t && chmod 755 compilew.t
	@mv compilew.t compilew.sh
link.sh:
	@echo remaking $@
	@( echo "#! /bin/sh" ; \
	echo '# automatically generated, do not edit' ; \
	echo 'LDFLAGS="$${LDFLAGS:-$$_CFLAGS_G}"' ; \
	echo 'LDFLAGS="$${LDFLAGS:--g}"' ; \
	echo 'CC="$${CC:-gcc}"' ; \
	echo 'exec $$CC $$LDFLAGS -o $${1+"$$@"} ' ; \
	) > link.t && chmod 755 link.t
	@mv link.t link.sh

maintainer-clean: distclean
	rm -f Makefile $(MAINTAINERCLEANFILES)
distclean: clean
	rm -f $(DISTCLEANFILES) dummy.tmp # to avoid "no arguments error"
clean spotless: mostlyclean
	rm -f $(LIBRARIES) $(CLEANFILES) link.sh compile.sh compilew.sh
	rm -rf deps
mostlyclean:
	rm -f *.o $(TARGETS) $(CLEANFILES)

NEWS-check:
	@if sed 1q NEWS | grep -e "$(VERSION)" > /dev/null; then :; else \
		echo "NEWS not updated; not releasing" 1>&2; \
		exit 1;                                   \
	fi
	@if grep -e "$(VERSION)" ChangeLog > /dev/null; then :; else \
		echo "ChangeLog not updated; not releasing" 1>&2; \
		exit 1;                                   \
	fi
bk-dist: NEWS-check
	bk tag `echo "Release-$(VERSION)" | sed 's/\./-/g'`
	$(MAKE) dist
cvs-dist: NEWS-check
	cvs tag `echo "Release-$(VERSION)" | sed 's/\./-/g'`
	$(MAKE) dist

distdir=$(PACKAGE)-$(VERSION)
dist: $(DISTFILES)
	rm -rf $(PACKAGE_VERSION)
	mkdir $(PACKAGE_VERSION)
	-chmod 755 $(PACKAGE_VERSION)
	@for file in $(DISTFILES); do \
		d=.; \
		if test -d $$d/$$file; then \
			cp -pr $$/$$file $(distdir)/$$file; \
		else \
			test -f $(distdir)/$$file \
				|| ln $$d/$$file $(distdir)/$$file 2> /dev/null \
			|| cp -p $$d/$$file $(distdir)/$$file || :; \
		fi; \
	done
	$(TAR) chof $(PACKAGE_VERSION).tar $(PACKAGE_VERSION)
	$(GZIP) $(PACKAGE_VERSION).tar
	rm -rf $(PACKAGE_VERSION)
distcheck: NEWS-check dist 
	$(GZIP) -d <$(PACKAGE_VERSION).tar.gz | $(TAR) xf -
	ls -l $(PACKAGE_VERSION) >distcheck.tmp
	cd $(PACKAGE_VERSION) && $(MAKE) && $(MAKE) check && $(MAKE) installcheck \
		&& $(MAKE) NEWS-check && $(MAKE) distclean
	ls -l $(PACKAGE_VERSION) >distcheck.tmp2
	diff -u distcheck.tmp distcheck.tmp2

force:	

install: $(TARGETS) $(EXTRAINSTALL) uoinstall
	prefix="$(prefix)" exec_prefix="$(exec_prefix)" \
	bindir="$(bindir)" sbindir="$(sbindir)" libexecdir="$(libexecdir)" \
	datadir="$(datadir)" sysconfdir="$(sysconfdir)" \
	sharedstatedir="$(sharedstatedir)" localstatedir="$(localstatedir)" \
	libdir="$(libdir)" infodir="$(infodir)" lispdir="$(lispdir)" \
	includedir="$(includedir)" mandir="$(mandir)" man1dir="$(man1dir)" \
	man2dir="$(man2dir)" man3dir="$(man3dir)" man4dir="$(man4dir)" \
	man5dir="$(man5dir)" man6dir="$(man6dir)" man7dir="$(man7dir)" \
	man8dir="$(man8dir)" man9dir="$(man9dir)" \
	srcdir="$(srcdir)" ./uoinstall <$(srcdir)install.list
installcheck: $(TARGETS) $(EXTRAINSTALL) uoinstall
	rm -rf ./testinstall
	mkdir ./testinstall
	nochown=t $(MAKE) prefix=`pwd`/testinstall install
	rm -rf ./testinstall
	
systype:
	@echo
	@echo "PACKAGE: $(PACKAGE) $(VERSION)"
	@echo "REALNAME: $(REALNAME)"
	@echo "uname-a:" `uname -a`
	@echo "uname-s:" `uname -s`
	@echo "uname-r:" `uname -r`
	@echo "uname-v:" `uname -v`
	@echo "uname-m:" `uname -m`

# that's not the "right" dependency, but that way it works even if
# uolib is absent.
LICENSES: Makefile.pre LICENSES.HEAD LICENSES.LOCAL
	( cat LICENSES.HEAD ; \
	for i in *.[ch] ; do \
		grep -h -s "^$$i:" /home/uwe/work/uolib/LICENSES.REPO \
		|| grep -h -s "^$$i:" ./LICENSES.LOCAL \
		|| grep -h -s "^$$i:" ./LICENSES \
		|| echo "$$i: unknown" ; \
	done ; \
	) >>$@.t
	mv $@.t $@

check: 
	env - PATH="`pwd`:$$PATH" sh docheck 2>&1 | cat -v | tee check.out | \
	diff -u check.exp -

%.o: %.c
	./compile.sh -c $<

