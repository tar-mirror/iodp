#! /bin/sh
#usage: gettype TYPE ERSATZ headers
type="$1"
ersatz="$2"
shift
shift
for i in "" "$*" ; do
if test "x$i" = x ; then
	line="";
else
	line="#include <$i>"
fi
cat >conftest.c <<EOF
$line
int main()
{
	$type t; 
	return sizeof(t);
}
EOF
	rm -f conftest.o
	./compile.sh -c conftest.c >/dev/null  2>/dev/null
	if test -f conftest.o ; then
	  echo "$line"
	  rm -f conftest.c conftest.o conftest
	  exit 0
	fi

done
echo "#define $type $ersatz"
rm -f conftest.c conftest.o conftest
exit 0
