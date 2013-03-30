#!/bin/sh

<<<<<<< HEAD
echo "int foo(void) { char X[200]; return 3; }" | $* -S -xc -c -O0 -fstack-protector - -o - 2> /dev/null | grep -q "%gs"
=======
echo "int foo(void) { char X[200]; return 3; }" | $* -S -x c -c -O0 -fstack-protector - -o - 2> /dev/null | grep -q "%gs"
>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y
if [ "$?" -eq "0" ] ; then
	echo y
else
	echo n
fi
