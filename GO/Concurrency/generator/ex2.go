package main

import (
	"flag"
	"fmt"
)

func main() {
	var k, n int

	flag.IntVar(&k, "k", 0, "start")
	flag.IntVar(&n, "n", 0, "range")

	flag.Parse()

	ch1 := generator(k, n)
	ch2 := squere(ch1)

	for num := range ch2 {
		fmt.Println(num)
	}
}

func generator(k, n int) <-chan int {
	ch := make(chan int)
	go func() {
		for i := k; i <= n; i++ {
			ch <- i
		}
		close(ch)
	}()
	return ch
}

func squere(ch1 <-chan int) <-chan int {
	ch2 := make(chan int)
	go func() {
		for i := range ch1 {
			ch2 <- i * i
		}
		close(ch2)
	}()

	return ch2
}
