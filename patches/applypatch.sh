#!/bin/sh
#
# applypatch.sh
# apply patches
#


dir=`cd $(dirname $0) && pwd`
top=$dir/../../../..

if [ "$IS_RR" = "1" ]; then
    echo "*** This ResurrectionRemix ***"
    for patch in `ls $dir/RR/*.patch` ; do
        echo ""
        echo "==> patch file: ${patch##*/}"
        patch -p1 -N -i $patch -r - -d $top
    done
else
    echo "*** This Cyanogenmod ***"
    for patch in `ls $dir/*.patch` ; do
        echo ""
        echo "==> patch file: ${patch##*/}"
        patch -p1 -N -i $patch -r - -d $top
    done
fi

find . -name "*.orig" -delete
