for i in $(seq 10000); do
  echo $i
  ../userprog/nachos -rs 1 -x ../test/test_read_return_value < ../userprog/input_10.dat > temp.dat
  python remove_machine_info.py temp.dat _temp.dat
  diff _temp.dat output_10.dat
done

