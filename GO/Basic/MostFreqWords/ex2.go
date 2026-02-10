package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
)

type WordStr struct {
	Word  string
	Count int
}

func main() {

	reader := bufio.NewReader((os.Stdin))
	line, err := reader.ReadString('\n')
	if err != nil {
		fmt.Printf("Invalid input\n")
		return
	}
	line = strings.TrimSpace(line)

	c, err := reader.ReadString('\n')
	c = strings.TrimSpace(c)
	if err != nil {
		fmt.Printf("Invalid input\n")
		return
	}
	k, err := strconv.Atoi(c)
	if err != nil || k <= 0 {
		fmt.Printf("Invalid input\n")
		return
	}

	result := MostFreqWords(line, k)

	fmt.Println(result)
}

func MostFreqWords(line string, k int) string {
	if len(line) == 0 {
		return line
	}
	words := strings.Split(line, " ")

	wordCount := make(map[string]int)
	for _, word := range words {
		wordCount[word]++
	}

	wordList := make([]WordStr, len(wordCount))
	i := 0
	for word, count := range wordCount {
		wordList[i] = WordStr{Word: word, Count: count}
		i++
	}
	count := min(k, len(wordList))
	sort.Slice(wordList, func(i, j int) bool {
		if wordList[i].Count == wordList[j].Count {
			return wordList[i].Word < wordList[j].Word
		}
		return wordList[i].Count > wordList[j].Count
	})

	topWords := make([]string, count)
	for i := 0; i < count; i++ {
		topWords[i] = wordList[i].Word
	}
	result := strings.Join(topWords, " ")
	return result
}
