#! /bin/sh
#
# try to find socket
#
cat >conftest.c <<EOF
int main() {
	if (socket()) return gethostbyname();
	else return 0;
}
EOF
./compile.sh -c conftest.c  2>/dev/null >/dev/null
for i in "" -lsocket "-lsocket -lnsl" ; do
	./link.sh conftest conftest.o $i 2>/dev/null >/dev/null
	if test $? -eq 0 ; then
		if test "x$i" = x ; then 
			:
		else
			echo "$i"
		fi
		rm -f conftest.c conftest.o conftest
		exit 0;
	fi
done
rm -f conftest.c conftest.o conftest
