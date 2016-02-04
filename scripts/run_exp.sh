#! /bin/sh

PROG=/home/yusuke/var/build/pigf/igugen
OPTION="-s -m 4 -n 1000 -l 1000"
DATA_DIR=/home/yusuke/var/sandbox/pigf/data/iptable

DATA_LIST="pat-32 pat-24 pat-23 pat-22 pat-21 pat-20 pat-19 pat-18 pat-17 pat-16"
LX_LIST="MCMC"

for data in ${DATA_LIST}; do
    for lx in ${LX_LIST}; do
	echo "${PROG} ${OPTION} --lx ${lx} ${DATA_DIR}/${data}"
	${PROG} ${OPTION} --lx ${lx} ${DATA_DIR}/${data}
    done
done
