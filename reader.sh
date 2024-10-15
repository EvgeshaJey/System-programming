#!/bin/bash

FIFO="/tmp/my_fifo"

# Чтение сообщений из именованного канала
while true; do
    if read line < $FIFO; then
        echo -e "$line"
    fi
done
