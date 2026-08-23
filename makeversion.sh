#!/bin/bash


if [ ! -d "./.git" ]; then
    echo  Version no change 
    exit 0
fi

#1.参数获取检查
num=0
SHELL_PATH=$(cd "$(dirname "$0")"; pwd)
VERSION_FILE="$SHELL_PATH/inner.h"
GIT_REPOSITORY=$SHELL_PATH/.git 

#头文件中定义的宏定义
COMMIT_FIELD=PS3_SMART_\COMMIT_ID
COMMIT_NEW_FIELD=PS3_SMART_\BRANCH
ARM_X86_FLAG=PS3_SMART_\ARCH
BUILD_TIME=PS3_SMART_\BUILD_TIME
VERSION_INFO=PS3_SMART_\VERSION

PS3_VERSION=V1.1.0
CURRENT_TIME=`date +"%b %d %Y %H:%M:%S"`

commit=`git --git-dir=$GIT_REPOSITORY log --pretty=oneline -n 100`
commit_id=(${commit///})

COMMIT_NEW_VALUE=`git --git-dir=$GIT_REPOSITORY show $commit_id --pretty=format:%h |sed -n 1p|cut -c 1-7`;
COMMIT_NEW_VALUE_GIT=`git --git-dir=$GIT_REPOSITORY branch -vv --no-color 2>/dev/null |\
    sed -e '/^[^*]/d' -e 's/* \(.*\)/\1/'|\
    awk '{print $3}'|sed -e 's/\[origin\///g' -e 's/\]//g'`;

#新生成的结构
COMMIT_NEW_LINE="#define $COMMIT_FIELD              \"${COMMIT_NEW_VALUE}\""
COMMIT_NEW_NEW="#define $COMMIT_NEW_FIELD                 \"${COMMIT_NEW_VALUE_GIT}\""
ARM_X86_FLAG_NEW1="#define $ARM_X86_FLAG                   \"${ARCH_FLAG_NEW}\""
BUILD_TIME_NEW="#define $BUILD_TIME             \"${CURRENT_TIME}\""
VERSION_INFO_NEW="#define $VERSION_INFO                \"${PS3_VERSION}\""

#Build Time: 代码中写成如下格式即可
#define PS3_BUILD_DATE          __DATE__
#define PS3_BUILD_TIME          __TIME__
#product : 产品通过宏定义区分写即可
#Version : 各个产品定义自己的版本


sed -i "/${COMMIT_FIELD}/c ${COMMIT_NEW_LINE}" $VERSION_FILE
sed -i "/${COMMIT_NEW_FIELD}/c ${COMMIT_NEW_NEW}" $VERSION_FILE
sed -i "/${ARM_X86_FLAG}/c ${ARM_X86_FLAG_NEW1}" $VERSION_FILE
sed -i "/${BUILD_TIME}/c ${BUILD_TIME_NEW}" $VERSION_FILE
sed -i "/${VERSION_INFO}/c ${VERSION_INFO_NEW}" $VERSION_FILE


