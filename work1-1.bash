#!/bin/bash
echo insert number 1, number2
declare -i num1 num2 sum=0
read num1 num2
for ((i=2;i<=num2;i++))
do
    arr[i]=$i
done

for ((i=2;i<=num2;i++))
do
    if [ ${arr[i]} -eq 0 ]
    then
        continue
    fi
     for ((j=i+i;j<=num2;j+=i))
    do
        arr[j]=0
    done
   
done

for ((i=num1+1;i<num2;i++))
do
    if [ ${arr[i]} -ne 0 ]
    then
        sum+=${arr[i]}
    fi
done

echo $sum


