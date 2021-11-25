#!bin/bash
make clean
make
echo "**********Sequential**********"
./rainfall_seq 8 50 0.5 4096 measurement_4096x4096.in > my_4096_seq.out
python3 check.py 4096 measurement_4096x4096.out my_4096_seq.out
echo "**********1 Thread**********"
./rainfall_pt 1 50 0.5 4096 measurement_4096x4096.in > my_4096_1thread.out
python3 check.py 4096 measurement_4096x4096.out my_4096_1thread.out
echo "**********2 Thread**********"
./rainfall_pt 2 50 0.5 4096 measurement_4096x4096.in > my_4096_2thread.out
python3 check.py 4096 measurement_4096x4096.out my_4096_2thread.out
echo "**********4 Thread**********"
./rainfall_pt 4 50 0.5 4096 measurement_4096x4096.in > my_4096_4thread.out
python3 check.py 4096 measurement_4096x4096.out my_4096_4thread.out
echo "*********8 Thread**********"
./rainfall_pt 8 50 0.5 4096 measurement_4096x4096.in > my_4096_8thread.out
python3 check.py 4096 measurement_4096x4096.out my_4096_8thread.out

