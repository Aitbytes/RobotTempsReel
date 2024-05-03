#!/bin/bash

IP="10.105.0.134"
command_list="cd /home/xenomai/RobotTempsReel && \
 git pull && \
 cd /home/xenomai/RobotTempsReel/software/raspberry/superviseur-robot && \
make CONF=Debug__RPI_ -j4  -Wall && \
 cd /home/xenomai/RobotTempsReel/software/raspberry/superviseur-robot/dist/Debug__RPI_/GNU-Linux && \
 sudo ./superviseur-robot"

ssh xenomai@"$IP" "$command_list" | tee output.txt