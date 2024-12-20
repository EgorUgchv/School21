#!bin/bash
PATTERNS_FILE="test_setups.txt"
TEST_COUNTER=1
SUCCESS_TESTS_COUNTER=0
FAILED_TESTS_COUNTER=0
while read -r line; do
    ./s21_grep $line > s21_test_grep.txt
    echo "PATTERN: $line"
    grep $line > grep_test.txt
    if diff -q s21_test_grep.txt grep_test.txt > /dev/null; then
        echo "TEST $TEST_COUNTER SUCCESS"
        ((SUCCESS_TESTS_COUNTER++))
    else
        echo "TEST $TEST_COUNTER FAILED"
        ((FAILED_TESTS_COUNTER++))
    fi
    ((TEST_COUNTER++))
done < $PATTERNS_FILE

rm s21_test_grep.txt
rm grep_test.txt

echo "TESTS PASSED: $SUCCESS_TESTS_COUNTER"
echo "TEST FAILED: $FAILED_TESTS_COUNTER"
