#!/bin/bash
# This is a shell script to check your program(s) on test cases.
# It usese the same language you normally use to type commands in
# a terminal window.  You can write whole programs in shell.

# Assume we've succeeded until we see otherwise.
FAIL=0

# Print an error message and set the fail flag.
fail() {
    echo "**** $1"
    FAIL=1
}

# Check the exit status.  The actual exit status (ASTATUS) should match
# the expected status (ESTATUS)
checkStatus() {
  ESTATUS="$1"
  ASTATUS="$2"

  # Make sure the program exited with the right exit status.
  if [ "$ASTATUS" -ne "$ESTATUS" ]; then
      fail "FAILED - incorrect program exit status. (expected $ESTATUS,  Got: $ASTATUS)"
      return 1
  fi

  return 0
}

# Check the contents of an a file.  If the expected file (EFILE)
# exists, then the actual file (AFILE) should exist and it should match.
checkFile() {
  NAME="$1"
  EFILE="$2"
  AFILE="$3"

  # Make sure we're really expecting this file.
  if [ ! -f "$EFILE" ]; then
      return 0
  fi

  # Make sure the output matches the expected output.
  echo "   diff $EFILE $AFILE"
  diff -q "$EFILE" "$AFILE" >/dev/null 2>&1
  if [ $? -ne 0 ]; then
      fail "FAILED - $NAME ($AFILE) doesn't match expected ($EFILE)"
      return 1
  fi

  return 0
}

# Same as checkFile, but if the expected file (EFILE) doesn't exist, the
# actual file (AFILE) should be empty.
checkFileOrEmpty() {
  NAME="$1"
  EFILE="$2"
  AFILE="$3"
  
  # if the expected output file doesn't exist, the actual file should be
  # empty.
  if [ ! -f "$EFILE" ]; then
      if [ -s "$AFILE" ]; then
	  fail "FAILED - $NAME ($AFILE) should be empty."
	  return 1
      fi
      return 0
  fi

  # Make sure the output matches the expected output.
  echo "   diff $EFILE $AFILE"
  diff -q "$EFILE" "$AFILE" >/dev/null 2>&1
  if [ $? -ne 0 ]; then
      fail "FAILED - $NAME ($AFILE) doesn't match expected ($EFILE)"
      return 1
  fi

  return 0
}

# Same as checkFile, but if the expected file (EFILE) doesn't exist, the
# actual file (AFILE) should not exist.
checkFileOrDNE() {
  NAME="$1"
  EFILE="$2"
  AFILE="$3"
  
  # if the expected output file doesn't exist, the actual file should not exist
  if [ ! -f "$EFILE" ]; then
      if [ -f "$AFILE" ]; then
	  fail "FAILED - $NAME ($AFILE) should not exist."
	  return 1
      fi
      return 0
  fi

  # Make sure the output matches the expected output.
  echo "   diff $EFILE $AFILE"
  diff -q "$EFILE" "$AFILE" >/dev/null 2>&1
  if [ $? -ne 0 ]; then
      fail "FAILED - $NAME ($AFILE) doesn't match expected ($EFILE)"
      return 1
  fi

  return 0
}

# The given file should exist but should be empty.
checkEmpty() {
  NAME="$1"
  AFILE="$2"
  
  if [ -s "$AFILE" ]; then
      fail "FAILED - $NAME ($AFILE) should be empty."
      return 1
  fi

  return 0
}

# Test the ident program.
runTest() {
    TESTNO=$1
    ESTATUS=$2

    echo "Test $TESTNO"
    rm -f output.txt

    # Test 7 doesn't redirect stdout.
    if [ $TESTNO == "07" ]; then
	echo "   ./vcalc input-$TESTNO.txt output.txt > stdout.txt 2> stderr.txt"
	./vcalc input-$TESTNO.txt output.txt > stdout.txt 2> stderr.txt
	ASTATUS=$?
	if ! checkEmpty "Stdout output" stdout.txt; then
	    return 1
	fi
    else
	echo "   ./vcalc input-$TESTNO.txt > output.txt 2> stderr.txt"
	./vcalc input-$TESTNO.txt > output.txt 2> stderr.txt
	ASTATUS=$?
    fi

    if ! checkStatus "$ESTATUS" "$ASTATUS"
       ! checkFile "Program output" "expected-$TESTNO.txt" "output.txt" ||
       ! checkFileOrEmpty "Stderr output" "error-$TESTNO.txt" "stderr.txt"
    then
	return 1
    fi

    echo "Test $TESTNO PASS"
    return 0
}

# make a fresh copy of the target programs
make clean
make

if [ $? -ne 0 ]; then
  fail "Make exited unsuccessfully"
fi

# Run test cases
if [ -x vcalc ]; then
    runTest 01 0
    runTest 02 0
    runTest 03 0
    runTest 04 0
    runTest 05 0
    runTest 06 0
    runTest 07 0
    runTest 08 0
    runTest 09 0
    runTest 10 0
    runTest 11 0
    runTest 12 0
    runTest 13 0
    runTest 14 0
    runTest 15 0
    runTest 16 0
    runTest 17 0
    runTest 18 0
    runTest 19 0
    runTest 20 1
else
    fail "Since your program didn't compile, we couldn't test it"
fi

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  echo "TESTS SUCCESSFUL"
  exit 0
fi
