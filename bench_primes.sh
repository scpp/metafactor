#!/bin/bash

declare -a modearr=("1" "2" "3")
#declare -a numarr=("100" "200" "300" "400" "500" "600" "700" "800" "900" "1000" "2000"                    #"3000" "4000" "5000" "6000" "7000" "8000" "9000" "10000" "20000" "30000" "40000" "50000" #"65536")
#declare -a numarr=("100" "200" "300" "400" "500" "600" "700" "800" "900" "1000" "2000"                    "3000" "4000" "5000" "6000" "7000" "8000" "9000" "10000")
declare -a numarr=("1000")

rm -rf bench_build
mkdir bench_build

if [ -z "$1" ]
then
compiler="g++"
else
compiler="$1"
fi

mv CMakeLists.txt CMakeLists.txt.bak
sed -e "/metafactor/d" -e "s/g++/${compiler}/g" CMakeLists.txt.bak > CMakeLists.txt

cd bench_build

for i in "${modearr[@]}"
do
  for n in "${numarr[@]}"
  do
  echo ">>>>>>>>>>>> ${n}"
    cmake -DCPP="98" -DMODE="${i}" -DLIMIT="${n}" -B. -H.. > /dev/null; time -p make > /dev/null
  done
done

cd ..
mv CMakeLists.txt.bak CMakeLists.txt
