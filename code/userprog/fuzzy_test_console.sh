for i in $(seq 10000); do
# echo $i
  ./nachos -rs $i -x ../test/console < input.dat > temp.dat
  python remove_machine_info.py temp.dat _temp.dat
  diff _temp.dat output.dat
done

