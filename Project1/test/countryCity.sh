cat country.txt city.txt  | egrep 'g' | sort | more > countryCitygSorted.txt
cat country.txt city.txt  | egrep 'g' | sort | wc -l > countryCitygCount.txt
