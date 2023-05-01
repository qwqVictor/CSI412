#!/bin/sh
cd "$(dirname "$0")/../"
make
echo "Test program: "
./mysh <<EOF
cat test/city.txt | grep a | wc -l
EOF
echo
echo "System example: "
cat test/city.txt | grep a | wc -l
