package lrucache_test

import (
	"lrucache"
	"testing"
)

func TestWithOverflow(t *testing.T) {
	c := lrucache.Constructor[int, string](3)
	c.Set(1, "Hello")
	c.Set(2, "world")
	c.Set(3, "school")

	c.Set(4, "Hello")

	gc, ex := c.Get(1)
	if ex {
		t.Errorf("ELement is not deleted: %T\n", gc)
	}
	c.Clear()
}
