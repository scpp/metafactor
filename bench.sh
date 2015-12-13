#!/bin/bash

declare -a qarr=("6" "30" "210" "2310")
declare -a modearr=("1" "2" "3" "4")
declare -a numarr=("65521" "4294967291" "0")
#declare -a numarr=("100")

rm -rf bench_build
mkdir bench_build

if [ -z "$1" ]
then
compiler="g++"
else
compiler="$1"
fi

mv CMakeLists.txt CMakeLists.txt.bak
sed -e "/metaprimes/d" -e "s/g++/${compiler}/g" CMakeLists.txt.bak > CMakeLists.txt

cd bench_build

echo "============ C++11 and variadic templates ================"

for i in "${modearr[@]}"
do
  echo ">>>>>>>>>>>> ${qarr[$i-1]}k+r <<<<<<<<<<<"
  for n in "${numarr[@]}"
  do
    cmake -DCPP="11" -DMODE="${i}" -DNUM="${n}" -B. -H..; time make
  done
#  read -p "Press [Enter] key to continue..."
done

echo "============ Using Loki::Typelist ================"

for i in "${modearr[@]}"
do
  echo ">>>>>>>>>>>> ${qarr[$i-1]}k+r <<<<<<<<<<<"
  for n in "${numarr[@]}"
  do
    cmake -DCPP="98" -DMODE="${i}" -DNUM="${n}" -B. -H..; time make
  done
done

mv CMakeLists.txt.bak CMakeLists.txt
