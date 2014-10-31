for i in $(seq 100); do
    ./nachos -rs $i -q 203 | grep "SUCCESS"
    echo $i
done;
