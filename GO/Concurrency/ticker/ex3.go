package main

import (
	"flag"
	"fmt"
	"os"
	"os/signal"
	"syscall"
	"time"
)

func main() {
	var k int
	ch := make(chan os.Signal, 2)
	flag.IntVar(&k, "k", 0, "step")
	flag.Parse()
	i := 0
	var timeSleep time.Duration
	go func() {
		for {
			time.Sleep(time.Duration(k) * time.Second)
			timeSleep += time.Duration(k) * time.Second
			fmt.Printf("Tick %d since %v\n", i+1, timeSleep)
			i++
		}

	}()

	signal.Notify(ch, syscall.SIGTERM, syscall.SIGINT)
	<-ch
	fmt.Println("\nTermitnation")
}
