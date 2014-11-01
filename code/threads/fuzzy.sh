for i in $(seq 10000); do
    echo $i
    ./nachos -rs $i -q 203 | grep SUCCESS
done;
