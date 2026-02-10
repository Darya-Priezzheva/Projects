package main

import (
	"testing"
)

func TestNormal(t *testing.T) {
	result := MostFreqWords("aa bb cc aa cc cc cc aa ab ac bb", 3)
	expected := "cc aa bb"
	if result != expected {
		t.Errorf("Unexpected result: %v vs %v", result, expected)
	}
}

func TestEmptyList(t *testing.T) {
	result := MostFreqWords("", 3)
	expected := ""
	if result != expected {
		t.Errorf("Unexpected result: %q vs %q", result, expected)
	}
}

func TestKMorethenWords(t *testing.T) {
	result := MostFreqWords("aa bb aa nn", 6)
	expected := "aa bb nn"
	if result != expected {
		t.Errorf("Unexpected result: %q vs %q", result, expected)
	}
}
