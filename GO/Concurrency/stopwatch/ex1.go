package main

import (
	"flag"
	"fmt"
	"math/rand"
	"sort"
	"sync"
	"time"
)

type result struct {
	goroutineNum int
	sleepTime    time.Duration
}

func main() {
	var n, m int
	flag.IntVar(&n, "n", 0, "Count goroutines")
	flag.IntVar(&m, "m", 0, "MS")
	flag.Parse()

	results := make([]result, n)
	wg := sync.WaitGroup{}
	var mu sync.Mutex
	wg.Add(n)
	for i := 0; i < n; i++ {
		go func(id int) {

			mu.Lock()
			randTime := time.Duration(rand.Intn(m))
			timeSleep := randTime * time.Millisecond
			results[i].goroutineNum = i
			results[i].sleepTime = timeSleep
			time.Sleep(timeSleep)
			mu.Unlock()
			wg.Done()
		}(i)
	}
	wg.Wait()

	sort.SliceStable(results, func(i, j int) bool { return results[i].sleepTime > results[j].sleepTime })

	for i := 0; i < n; i++ {
		fmt.Printf("[%d %v]\n", results[i].goroutineNum, results[i].sleepTime)
	}

}
