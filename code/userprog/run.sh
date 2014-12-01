
for i in $(seq 1000); do
  echo $i
  ./nachos -rs $i -x ../test/test_join
done

