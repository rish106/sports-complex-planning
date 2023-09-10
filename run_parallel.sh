#!/usr/bin/env sh

if [ $# != 2 ]; then
    echo "Need 2 arguments"
    exit 1
fi

if [ ! -e "./main.out" ]; then
    echo "main binary does not exist, execute \"make\" first"
    exit 1
fi

input_file="$1"
output_file="$2"

max_processes=3

echo "Started executing"

for ((i = 0; i < $max_processes; i++)); do
    sleep $i && ./main.out "$input_file" "${output_file}_${i}" &
done

wait
echo "Finished"
