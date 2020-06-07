#!/bin/bash

output_file="test/benchmark_plot/time_result.txt"

test -e "${output_file}" && rm -f "${output_file}"

function ran_num_gen(){
    clang -g test/numgen.c test/utils/test_num.c -o test/numgen
    ./test/numgen -s "${1}" -r "${2}" "test/test_num.txt"
}

read -rp "Please enter the size of the test numbers: " size
read -rp "Please enter the range of the test numbers: " range
read -rp "How many times you want to repeat? " repeat

ran_num_gen "${size}" "${range}"

clang -g test/benchmark.c test/utils/test_num.c -o test/benchmark_plot/benchmark

function run_benchmark(){
    for((i=0;i<"${repeat}";i++))
    do
	    sudo nice -n -20 /usr/bin/time -f "%e,%U,%S" -a -o "${output_file}" ./test/benchmark_plot/benchmark

      # remove the page cache
	    sync; sudo sh -c "echo 3 > /proc/sys/vm/drop_caches"
    done

    # plot the graph
    python3 test/benchmark_plot/plot.py "${repeat}"
}

echo "Benchmark starts..."
run_benchmark
echo "Benchmark ends!"
