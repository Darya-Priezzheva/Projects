package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func readLeftOperand() float64 {
	reader := bufio.NewReader(os.Stdin)
	var op float64
	for {
		ip, err := reader.ReadString('\n')
		if err != nil {
			fmt.Println("Invalid input. Please, try again:")
			continue
		}
		ip = strings.TrimSpace(ip)
		value, err := strconv.ParseFloat(ip, 64)
		if err != nil {
			fmt.Println("Invalid input. Please, try again:")
			continue
		}
		op = value
		break
	}
	return op
}

func readRightOperand() float64 {
	reader := bufio.NewReader(os.Stdin)
	var op float64
	for {
		ip, err := reader.ReadString('\n')
		if err != nil {
			fmt.Println("Invalid input. Please, try again:")
			continue
		}
		ip = strings.TrimSpace(ip)
		value, err := strconv.ParseFloat(ip, 64)
		if err != nil {
			fmt.Println("Invalid input. Please, try again:")
			continue
		}
		if value == 0 {
			fmt.Println("Division by 0")
		}
		op = value
		break
	}
	return op
}

func readOperation() string {
	reader := bufio.NewReader(os.Stdin)
	var op string
	for {
		ip, err := reader.ReadString('\n')
		if err != nil {
			fmt.Println("Invalid input. Please, try again:")
			continue
		}
		ip = strings.TrimSpace(ip)
		if ip != "+" && ip != "-" && ip != "*" && ip != "/" {
			fmt.Println("Invalid operation, please use '+, -, *, /'")
			continue
		}
		op = ip
		break
	}
	return op
}

func main() {
	var a float64
	var b float64
	var op string

	fmt.Println("Input left operand")
	a = readLeftOperand()

	fmt.Println("Input operation, please use '+, -, *, /'")
	op = readOperation()

	fmt.Println("Input right operand")
	b = readRightOperand()
	fmt.Println("Result:")
	var res float64
	switch op {
	case "+":
		res = a + b
	case "-":
		res = a - b
	case "*":
		res = a * b
	case "/":
		res = a / b
	}
	fmt.Printf("%.3f\n", res)

}
