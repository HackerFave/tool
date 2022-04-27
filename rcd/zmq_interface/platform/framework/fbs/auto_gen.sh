#!/bin/bash

if [ ! $# == 1 ]; then
    echo "input project root path firstly"
    exit 1
fi

proj_dir=$1
if [ ! -d ${proj_dir} ]; then
    echo "project root path is not found"
    exit 1
fi

fbs_dir=${proj_dir}/src
if [ ! -d ${fbs_dir} ]; then
    echo "*.proto file path is not found"
    exit 1
fi

out_dir=${proj_dir}/out
rm -rf ${out_dir}
mkdir -p ${out_dir}

cmd=flatc
if [ -z `which ${cmd}` ]; then
    echo "need install flatc firstly"
    exit 1
fi

proto_src=`find ${fbs_dir} -name "*.fbs"`
${cmd} --cpp --gen-mutable --gen-object-api --reflect-names --no-includes --gen-compare -o ${out_dir}  ${proto_src}

exit 0
