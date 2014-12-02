for i in $(seq 1000); do
    ./nachos -rs $i -x ../test/test_userexception
done
