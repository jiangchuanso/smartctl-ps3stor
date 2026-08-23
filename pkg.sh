#!/bin/bash
./configure
make 

COMMIT_VALUE=`git rev-parse --short HEAD`
TIME_DATE=`date +"%Y%m%d_%H%M%S"`

pkgname="smartctl_ps3stor_"$COMMIT_VALUE"_"$TIME_DATE".tar.gz"

mkdir bin
tar -czvf ./bin/$pkgname --exclude=./.git --exclude='*.o' --exclude='.deps' --exclude=./bin *





