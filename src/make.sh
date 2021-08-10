#!/bin/bash

make clean && \
make doc && \
make && \
sudo make install && \
sudo /etc/init.d/apache2 stop && \
sudo /etc/init.d/apache2 start
