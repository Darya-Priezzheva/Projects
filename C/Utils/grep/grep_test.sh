#!/bin/bash

INPUT1=text1.txt
INPUT2=text2.txt
PATTERNS=patterns.txt
FLAGS=("-e" "-i" "-v" "-c" "-l" "-n" "-i")
BONUS_FLAGS=("-s" "-h")
COMBINE_FLAGS=("-in" "-cv" "-il" "-hov" "-ch" "-iv" "-cvn")
echo "=======================================" > results.txt
echo "*************Valid cases***************" >> results.txt
for flag in "${FLAGS[@]}"; do
    grep $flag "code" $INPUT1 $INPUT2 > grep_original.txt
    ./s21_grep $flag "code" $INPUT1 $INPUT2 > s21_grep.txt

    if diff -q grep_original.txt s21_grep.txt > /dev/null; then
        echo "Flag $flag: OK" >> results.txt
    else 
        echo "Flag $flag: FAIL" >> results.txt
    fi
done
echo "=======================================" >> results.txt
echo "*************Without flag**************" >> results.txt
grep "code" $INPUT1 > grep_original.txt
./s21_grep "code" $INPUT1 > s21_grep.txt


if diff -q grep_original.txt s21_grep.txt > /dev/null; then
    echo "Without flag: OK" >> results.txt
else 
    echo "Without flag: FAIL" >> results.txt
fi
echo "=======================================" >> results.txt
echo "**************Bonus flags**************" >> results.txt

for flag in "${BONUS_FLAGS[@]}"; do
    grep $flag "code" $INPUT1 $INPUT2> grep_original.txt
    ./s21_grep $flag "code" $INPUT1 $INPUT2> s21_grep.txt

    if diff -q grep_original.txt s21_grep.txt > /dev/null; then
        echo "Bonus flag $flag: OK" >> results.txt
    else 
        echo "Bonus flag $flag: FAIL" >> results.txt
    fi
done
echo "=======================================" >> results.txt
echo "***********Flag -f from file***********" >> results.txt
grep "-f" $PATTERNS $INPUT1 > grep_original.txt
./s21_grep "-f" $PATTERNS $INPUT1 > s21_grep.txt


if diff -q grep_original.txt s21_grep.txt > /dev/null; then
    echo "From file -f: OK" >> results.txt
else 
    echo "From file -f: FAIL" >> results.txt
fi

echo "=======================================" >> results.txt

echo "***************Flag -o*****************" >> results.txt
grep "-o" "co" $INPUT1 > grep_original.txt
./s21_grep "-o" "co" $INPUT1 > s21_grep.txt


if diff -q grep_original.txt s21_grep.txt > /dev/null; then
    echo "From file -o: OK" >> results.txt
else 
    echo "From file -o: FAIL" >> results.txt
fi
echo "=======================================" >> results.txt
echo "*************Combine flags*************" >> results.txt
for flag in "${COMBINE_FLAGS[@]}"; do
    grep $flag "code" $INPUT1 $INPUT2 > grep_original.txt
    ./s21_grep $flag "code" $INPUT1 $INPUT2 > s21_grep.txt

    if diff -q grep_original.txt s21_grep.txt > /dev/null; then
        echo "Combine flags $flag: OK" >> results.txt
    else 
        echo "Combine flags $flag: FAIL" >> results.txt
    fi
done