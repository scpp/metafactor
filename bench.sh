#!/bin/bash

declare -a qarr=("6" "30" "210" "2310")
declare -a modearr=("1" "2" "3" "4")
declare -a numarr=("65521" "4294967291" "0")
#declare -a numarr=("100")


echo "============ C++11 and variadic templates ================"

for i in "${modearr[@]}"
do
  echo ">>>>>>>>>>>> ${qarr[$i-1]}k+r <<<<<<<<<<<"
  for n in "${numarr[@]}"
  do
    cmake -DCPP="11" -DMODE="${i}" -DNUM="${n}" .; time make
  done
#  read -p "Press [Enter] key to continue..."
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

