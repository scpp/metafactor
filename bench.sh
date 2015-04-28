#!/bin/bash

declare -a qarr=("6" "30" "210" "2310")
declare -a modearr=("1" "2" "3" "4")
#declare -a numarr=("65519" "4294967291" "18446744073709551615")

# for clang
declare -a numarr=("65519" "4294967291" "0")


echo "============ C++11 and variadic templates ================"

for i in "${modearr[@]}"
do
  echo ">>>>>>>>>>>> ${qarr[$i-1]}k+r <<<<<<<<<<<"
  for n in "${numarr[@]}"
  do
    cmake -DCPP="11" -DMODE="${i}" -DNUM="${n}" .; time make
  done
done


echo "============ Using Loki::Typelist ================"

for i in "${modearr[@]}"
do
  echo ">>>>>>>>>>>> ${qarr[$i-1]}k+r <<<<<<<<<<<"
  for n in "${numarr[@]}"
  do
    cmake -DCPP="98" -DMODE="${i}" -DNUM="${n}" .; time make
  done
done

