#!/bin/bash
FAIL=0

# Function to run a test.  Expects the arguments in the variable, args
# (probably a bad way to handle a variable number of arguments)
runTest() {
  TESTNO=$1
  ESTATUS=$2

  rm -f output.txt stderr.txt
  
  echo "Test $TESTNO: ./vinyl ${args[@]} < input-$TESTNO.txt > output.txt 2> stderr.txt"
  ./vinyl ${args[@]} < input-$TESTNO.txt > output.txt 2> stderr.txt
  STATUS=$?

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** FAILED - Expected an exit status of $ESTATUS, but got: $STATUS"
      FAIL=1
      return 1
  fi

  # Make sure output matches expected output.
  if ! diff -q expected-$TESTNO.txt output.txt >/dev/null 2>&1 ; then
      echo "**** FAILED - output to stdout didn't match expected."
      FAIL=1
      return 1
  fi

  # Make sure the output to stderr matches expected.
  if [ -f error-$TESTNO.txt ] &&
	 ! diff -q error-$TESTNO.txt stderr.txt >/dev/null 2>&1 ; then
      echo "**** FAILED - output to stderr didn't match expected"
      FAIL=1
      return 1
  fi

  echo "PASS"
  return 0
}

# Try to get a fresh compile of the project.
make clean
make
if [ $? -ne 0 ]; then
    echo "**** Make didn't run succesfully when trying to build your program."
    FAIL=1
fi

# Run individual tests.
if [ -x vinyl ] ; then
    args=(records-a.txt)
    runTest 01 0
    
    args=(records-b.txt)
    runTest 02 0
    
    args=(records-a.txt records-b.txt)
    runTest 03 0
    
    args=(records-d.txt)
    runTest 04 0
    
    args=(records-b.txt records-c.txt)
    runTest 05 0
    
    args=(records-b.txt records-c.txt)
    runTest 06 0
    
    args=(records-b.txt records-c.txt)
    runTest 07 0
    
    args=(records-b.txt records-c.txt)
    runTest 08 0
    
    args=(records-b.txt records-c.txt)
    runTest 09 0
    
    args=(records-e.txt)
    runTest 10 0
    
    args=(records-e.txt)
    runTest 11 0

    args=(records-e.txt)
    runTest 12 0

    args=(records-a.txt records-b.txt records-c.txt records-d.txt)
    runTest 13 0
 
    args=(records-a.txt records-b.txt records-c.txt records-d.txt)
    runTest 14 0
 
    args=()
    runTest 15 1
 
    args=(records-a.txt file-that-doesnt-exist.txt)
    runTest 16 1
 
    args=(records-f.txt)
    runTest 17 1
 
    args=(records-g.txt)
    runTest 18 1
 
    args=(records-h.txt)
    runTest 19 1
 
    args=(records-i.txt)
    runTest 20 1
 
else
    echo "**** Your program couldn't be tested since it didn't compile successfully."
    FAIL=1
fi

if [ $FAIL -ne 0 ]; then
  echo "**** There were failing tests"
  exit 1
else 
  echo "Tests successful"
  exit 0
fi
