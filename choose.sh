#! /bin/sh

# ./choose.sh [clr] basename resultfile-if-ok resultfile-if-not

result="$4"

case "$1" in
  *c*) ./compile.sh -c $2.c >/dev/null 2>&1 || result="$3"
  ;;
esac

case "$1" in
  *l*) ./link.sh $2 >/dev/null 2>&1 || result="$3"
  ;;
esac

case "$1" in
  *r*) ./$2 >/dev/null 2>&1 || result="$3"
  ;;
esac

rm -f $2.o $2

exec cat "$result"
