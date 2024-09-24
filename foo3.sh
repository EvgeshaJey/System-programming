#!/bin/bash
# sys_info_page: программа для отображения информации о системе с использованием графического меню Zenity

PROGNAME=$(basename $0)
TITLE="System Information Report For $HOSTNAME"
CURRENT_TIME=$(date +"%x %r %Z")
TIME_STAMP="Generated $CURRENT_TIME, by $USER"
FILENAME="sys_info_report.html"

# Функция для отчета о времени работы системы
report_uptime () {
cat <<- _EOF_
    <H2>System Uptime</H2>
    <PRE>$(uptime)</PRE>
_EOF_
}

# Функция для отчета об использовании дискового пространства
report_disk_space () {
cat <<- _EOF_
    <H2>Disk Space Utilization</H2>
    <PRE>$(df -h)</PRE>
_EOF_
}

# Функция для отчета об использовании пространства домашней директории
report_home_space () {
    local format="%8s%10s%10s\n"
    local i dir_list total_files total_dirs total_size user_name
    if [[ $(id -u) -eq 0 ]]; then
        dir_list=/home/*
        user_name="All Users"
    else
        dir_list=$HOME
        user_name=$USER
    fi
    echo "<H2>Home Space Utilization ($user_name)</H2>"
    for i in $dir_list; do
        total_files=$(find "$i" -type f | wc -l)
        total_dirs=$(find "$i" -type d | wc -l)
        total_size=$(du -sh "$i" | cut -f 1)
        echo "<H3>$i</H3>"
        echo "<PRE>"
        printf "$format" "Dirs" "Files" "Size"
        printf "$format" "----" "-----" "----"
        printf "$format" "$total_dirs" "$total_files" "$total_size"
        echo "</PRE>"
    done
}

# Функция для генерации HTML-отчета
write_html_page () {
    cat <<- _EOF_ > $FILENAME
    <HTML>
        <HEAD>
            <TITLE>$TITLE</TITLE>
        </HEAD>
        <BODY>
            <H1>$TITLE</H1>
            <P>$TIME_STAMP</P>
            $(report_uptime)
            $(report_disk_space)
            $(report_home_space)
        </BODY>
    </HTML>
_EOF_
}

# Функция для выбора действия через Zenity
show_menu() {
    ACTION=$(zenity --list --title="System Information Menu" --column="Option" --text="Select an option:" \
        "Display System Uptime" \
        "Display Disk Space" \
        "Display Home Space Utilization" \
        "Generate Full HTML Report" \
        "Exit")

    case "$ACTION" in
        "Display System Uptime")
            zenity --info --text="$(uptime)" --title="System Uptime";;
        "Display Disk Space")
            zenity --info --text="$(df -h)" --title="Disk Space Utilization";;
        "Display Home Space Utilization")
            if [[ $(id -u) -eq 0 ]]; then
                zenity --info --text="$(du -sh /home/*)" --title="Home Space Utilization (All Users)"
            else
                zenity --info --text="$(du -sh $HOME)" --title="Home Space Utilization ($USER)"
            fi;;
        "Generate Full HTML Report")
            write_html_page
            zenity --info --text="Report generated at $FILENAME"
            xdg-open "$FILENAME" ;;
        "Exit")
            exit 0;;
        *)
            zenity --error --text="Invalid selection.";;
    esac
}

# Запускаем меню
show_menu

