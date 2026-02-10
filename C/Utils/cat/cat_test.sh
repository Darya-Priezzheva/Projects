#!/bin/bash
INPUT=text.txt

FLAGS=("-n" "-b" "-e" "-s" "-t" "--number-nonblank" "-E" "--number" "--squeeze-blank" "-T")
echo "******Valid cases******" > results.txt
for flag in "${FLAGS[@]}"; do
    cat $flag $INPUT > cat_original.txt
    ./s21_cat $flag $INPUT > s21_cat.txt

    if diff -q cat_original.txt s21_cat.txt > /dev/null; then
        echo "Flag $flag: OK" >> results.txt
    else 
        echo "Flag $flag: FAIL" >> results.txt
    fi
done


echo "******Without flag******" >> results.txt
cat  $INPUT > cat_original.txt
./s21_cat $INPUT > s21_cat.txt

if diff -q cat_original.txt s21_cat.txt > /dev/null; then
    echo "Without flag: OK" >> results.txt
else 
    echo "Without flag: FAIL" >> results.txt
fi

