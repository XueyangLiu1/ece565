#!/bin/bash
make clean
make
rm result.out
###### Normal
echo "*****Sequential*******"
./histo uiuc.pgm > result.out
diff result.out validation.out
rm result.out
echo "*****Lock*******"
./histo_locks uiuc.pgm > result.out
diff result.out validation.out
rm result.out
echo "*****Atomic*******"
./histo_atomic uiuc.pgm > result.out
diff result.out validation.out
rm result.out
echo "*****Creative*******"
./histo_creative uiuc.pgm > result.out
diff result.out validation.out
rm result.out
####### Large
# echo "*****Sequential*******"
# ./histo uiuc-large.pgm > result.out
# diff result.out validation-large.out
# rm result.out
# echo "*****Lock*******"
# ./histo_locks uiuc-large.pgm > result.out
# diff result.out validation-large.out
# rm result.out
# echo "*****Atomic*******"
# ./histo_atomic uiuc-large.pgm > result.out
# diff result.out validation-large.out
# rm result.out
# echo "*****Creative*******"
# ./histo_creative uiuc-large.pgm > result.out
# diff result.out validation-large.out
# rm result.out