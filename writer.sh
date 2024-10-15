#!/bin/bash

# Создание именованного канала
FIFO="/tmp/my_fifo"
if [[ ! -p $FIFO ]]; then
    mkfifo $FIFO
fi

# Функция для получения системного времени
get_system_time() {
    date +"%Y-%m-%d %H:%M:%S"
}

# Основной цикл по неделям семестра
for week in {1..16}; do
    case $week in
        1|2|3|4)
            echo "1-4 неделя: $(get_system_time)" > $FIFO
            ;;
        5|6|7)
            echo "5-7 неделя: Календарь:\n$(cal)\n$(get_system_time)" > $FIFO
            ;;
        8|9|10)
            echo "8-10 неделя: Ядро: $(uname -s), ОС: $(uname -o), $(get_system_time)" > $FIFO
            ;;
        11|12|13|14|15|16)
            echo -e "11-16 неделя: ОС: $(uname -o), Версия ядра: $(uname -r), Реализация ядра: $(uname -v), $(get_system_time)\nКалендарь:\n$(cal)" > $FIFO
            ;;
    esac
    sleep 1 # Задержка для удобства
done
