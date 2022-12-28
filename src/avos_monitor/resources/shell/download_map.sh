#!/bin/bash
cd $1
cp $2 .
tar -zcvf remote_run.tar.gz remote_run.sh
source deploy.sh
deploy -m
rm -rf remote_run.tar.gz
rm -rf map.tar.gz