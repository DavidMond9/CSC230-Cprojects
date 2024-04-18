# Get a fresh build to measure coverage
make clean

# Set coverage-flags to be used inside make.
export CFLAGS="-ftest-coverage -fprofile-arcs"
export LDLIBS="-lgcov"

# Make, using these extra flags.
make

# Remove any of the gcda files (should be done by make clean).
rm -f *.gcda

echo "Running test inputs given with the starter"
for i in 01 02 03 04 05 06 07 08 09 10 11
do
    echo "./driver < input-$i.txtt"
    ./driver < input-$i.txt > output.txt
done

# Run the student-generated test cases.
list=$(echo my-input-*.txt)

if [ "$list" != 'my-input-*.txt' ]; then
    echo
    echo "Running with student-created test inputs"
    for input in my-input-*.txt
    do
	echo "./driver < $input"
	./driver < $input > output.txt
    done
else
    echo "**** No student-created test inputs"
fi

gcov driver map value input
