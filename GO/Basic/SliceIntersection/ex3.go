package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	str1, err := readAndSplitString()
	if err != nil {
		fmt.Printf("Invalid input\n")
		return
	}
	nums1, err := stringToNums(str1)
	if err != nil {
		fmt.Printf("Invalid input\n")
		return
	}

	str2, err := readAndSplitString()
	if err != nil {
		fmt.Printf("Invalid input\n")
		return
	}
	nums2, err := stringToNums(str2)
	if err != nil {
		fmt.Printf("Invalid input\n")
		return
	}

	resultInts := findIntersection(nums1, nums2)
	if len(resultInts) == 0 {
		fmt.Printf("Empty intersecrion\n")
		return
	}
	resultString := intToString(resultInts)
	fmt.Println(resultString)

}

func readAndSplitString() ([]string, error) {
	var stringOfNumbers []string
	reader := bufio.NewReader(os.Stdin)
	str, err := reader.ReadString('\n')
	str = strings.TrimSpace(str)
	if err != nil {
		return stringOfNumbers, err
	}

	stringOfNumbers = strings.Split(str, " ")
	return stringOfNumbers, nil
}

func stringToNums(str []string) ([]int, error) {
	var arrOfInt []int
	for _, n := range str {
		nm, err := strconv.Atoi(n)
		if err != nil {
			return arrOfInt, err
		}
		arrOfInt = append(arrOfInt, nm)
	}
	return arrOfInt, nil
}

func findIntersection(nums1 []int, nums2 []int) []int {
	var result []int

	pattern := make(map[int]bool)
	exist := make(map[int]bool)

	for _, n := range nums2 {
		pattern[n] = true
	}

	for i := range nums1 {
		if pattern[nums1[i]] && !exist[nums1[i]] {
			exist[nums1[i]] = true
			result = append(result, nums1[i])
		}
	}
	return result
}

func intToString(resultInt []int) string {
	resultString := make([]string, len(resultInt))
	for i, n := range resultInt {
		resultString[i] = strconv.Itoa(n)
	}
	return strings.Join(resultString, " ")
}
