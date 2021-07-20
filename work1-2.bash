#!/bin/bash
echo insert month, date
declare -i month date sum=0
read month date
monthday=(31 28 31 30 31 30 31 31 30 31 30 31)

for ((i=1;i<month;i++))
do
    sum+=${monthday[i-1]}
done
sum+=$date-1
let num=$sum%7
case $num in
    1) echo "토요일" ;;
    2) echo "일요일" ;;
    3) echo "월요일" ;;
    4) echo "화요일" ;;
    5) echo "수요일" ;;
    6) echo "목요일" ;;
    0) echo "금요일" ;;
esac