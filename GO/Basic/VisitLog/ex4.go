package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"time"
)

type VisitInfo struct {
	Spec string
	Date string
}

type PatientNotFoundError struct{}

var patientVisits = make(map[string][]VisitInfo)

func main() {

	reader := bufio.NewReader(os.Stdin)

	for {
		fmt.Println("Введите команду:\n  -Save\n  -GetHistory\n  -GetLastVisit")
		comand, _ := reader.ReadString('\n')
		comand = strings.TrimSpace(comand)

		switch comand {
		case "Save":
			saveVisit(reader)
		case "GetHistory":
			err := getHistory(reader)
			if err != nil {
				fmt.Printf("%s\n\n", err.Error())
			}
		case "GetLastVisit":
			err := getLastVisit(reader)
			if err != nil {
				fmt.Printf("%s\n\n", err.Error())
			}
		default:
			fmt.Printf("Неизвестная команда\n\n")
		}

	}
}

func saveVisit(reader *bufio.Reader) {
	fmt.Println("Введите ФИО пациента")
	fio, _ := reader.ReadString('\n')
	fio = strings.TrimSpace(fio)

	fmt.Println("Введите специализацию врача")
	spec, _ := reader.ReadString('\n')
	spec = strings.TrimSpace(spec)

	fmt.Println("Введите дату визита в формате YYYY-MM-DD")
	for {
		date, _ := reader.ReadString('\n')
		date = strings.TrimSpace(date)
		_, err := time.Parse("2006-01-02", date)
		if err != nil {
			fmt.Println("Неверный формат даты")
			continue
		}

		visit := VisitInfo{Spec: spec, Date: date}
		patientVisits[fio] = append(patientVisits[fio], visit)
		break
	}

	fmt.Printf("Визит сохранен\n\n")
}

func getHistory(reader *bufio.Reader) error {
	fmt.Println("Введите ФИО пациента")
	fio, _ := reader.ReadString('\n')
	fio = strings.TrimSpace(fio)

	visits, exist := patientVisits[fio]
	if !exist {
		return &PatientNotFoundError{}
	}

	for _, vst := range visits {
		fmt.Printf("%s %s\n\n", vst.Spec, vst.Date)
	}
	return nil
}

func getLastVisit(reader *bufio.Reader) error {
	fmt.Println("Введите ФИО пациента")
	fio, _ := reader.ReadString('\n')
	fio = strings.TrimSpace(fio)

	visits, exist := patientVisits[fio]
	if !exist {
		return &PatientNotFoundError{}
	}
	fmt.Println("Введите специализацию врача")
	spec, _ := reader.ReadString('\n')
	spec = strings.TrimSpace(spec)

	var lastDate time.Time
	found := false

	for _, v := range visits {
		if v.Spec == spec {
			t, _ := time.Parse("2006-01-02", v.Date)

			if !found {
				lastDate = t
				found = true
			}
			if t.After(lastDate) {
				lastDate = t
			}
		}
	}
	if found {
		fmt.Printf("%s\n\n", lastDate.Format("2006-01-02"))
	} else {
		fmt.Printf("Нет посещений\n\n")
	}
	return nil
}

func (err *PatientNotFoundError) Error() string {
	return "patient not found"
}
