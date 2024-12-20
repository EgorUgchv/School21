#!/bin/bash
RESULT_COMPARISON=""
TEST_FILE="test.txt"
SUCCESS_TESTS_COUNTER=0
TEST_COUNTER=1
FAILED_TESTS_COUNTER=0
for flag in -b -e -n -s -t -v
do
  echo "FLAG $flag"
  ./s21_cat $flag $TEST_FILE > s21_test_cat.txt
  cat $flag $TEST_FILE > cat_test.txt
  if diff -q s21_test_cat.txt cat_test.txt > /dev/null;
    then
      echo "TEST $TEST_COUNTER SUCCESS"
      ((SUCCESS_TESTS_COUNTER++))
    else
      echo "TEST $TEST_COUNTER FAILED"
      ((FAILED_TESTS_COUNTER++))
  fi
  ((TEST_COUNTER++))
done

rm s21_test_cat.txt
rm cat_test.txt

echo "TESTS PASSED: $SUCCESS_TESTS_COUNTER"
echo "TEST FAILED: $FAILED_TESTS_COUNTER"