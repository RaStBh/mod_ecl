#!/bin/bash

make clean && \
make doc && \
make && \
sudo make install && \
sudo make stop && \
sleep 3 && \
sudo make start && \
sleep 3 && \
lynx https://mod_ecl.lh.ld/index.ecl
