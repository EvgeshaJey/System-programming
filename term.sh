#!/bin/bash

read -p "Введите количество терминалов для запуска: " num

for ((i=1; i<=num; i++))
do
  read -p "Введите команду для терминала $i: " command

  gnome-terminal -- bash -c "$command; exec bash"
done

