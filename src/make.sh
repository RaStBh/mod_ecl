#!/bin/bash

make clean && \
make && \
sudo make install && \
sudo /etc/init.d/apache2 stop && \
sudo /etc/init.d/apache2 start && \
sudo lynx 'https://user:password@mod_ecl.lh.ld/index.ecl?key=value'
