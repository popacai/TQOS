l="100 101 102 103 104 1000 1001 1002 1003 1004"
for run in $l:
do
    echo $run
    ./nachos -rs 1 -q $run | grep "SUCCESS"
done
