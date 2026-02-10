package lrucache

import (
	"container/list"
	"sync"
)

type Cache[K comparable, V any] struct {
	capacity int
	items    map[K]*elem[K, V]
	list     *list.List
	mu       sync.Mutex
}

type elem[K comparable, V any] struct {
	key  K
	val  V
	elem *list.Element
}

func Constructor[K comparable, V any](cap int) *Cache[K, V] {
	return &Cache[K, V]{
		capacity: cap,
		items:    make(map[K]*elem[K, V]),
		list:     list.New()}
}

func (c *Cache[K, V]) Set(key K, value V) {
	c.mu.Lock()
	defer c.mu.Unlock()

	elemPtr, exist := c.items[key]

	if exist {
		elemPtr.val = value
		c.list.MoveToFront(elemPtr.elem)
	} else {
		// создаем элем с новыми данными
		newElem := &elem[K, V]{key: key, val: value}

		// в лист вносим новое значение в начало
		newElem.elem = c.list.PushFront(newElem)

		// в кэш по ключу вносим новую структуру
		c.items[key] = newElem
	}

	// если лимит элементов превышен, то последний элемент удаляется
	if len(c.items) > c.capacity {
		// сохраняем последний элемент кэша
		lastItem := c.list.Back()

		// сохраняем указатель на структуру
		lastElem := lastItem.Value.(*elem[K, V])

		// удаляем мапу по ключу
		delete(c.items, lastElem.key)

		// удаляем из кэша по списку
		c.list.Remove(lastItem)
	}
}

func (c *Cache[K, V]) Get(key K) (map[K]V, bool) {
	c.mu.Lock()
	defer c.mu.Unlock()
	res := make(map[K]V)

	v, exist := c.items[key]

	if exist {
		res[v.key] = v.val
		return res, exist
	} else {
		return res, exist
	}
}

func (c *Cache[K, V]) Clear() {
	c.mu.Lock()
	defer c.mu.Unlock()
	c.items = make(map[K]*elem[K, V])
	c.list.Init()
}

//func main() {
//	c := Constructor[int, string](3)
//
//	c.Set(1, "Hello")
//	gc, ex := c.Get(1)
//	fmt.Println(gc, ex)
//
//	c.Set(2, "world")
//	gc, ex = c.Get(2)
//	fmt.Println(gc, ex)
//
//	c.Set(3, "school")
//	gc, ex = c.Get(3)
//	fmt.Println(gc, ex)
//
//	c.Set(4, "Hello")
//	gc, ex = c.Get(4)
//	fmt.Println(gc, ex)
//
//	// c.Set(1, "Hello")
//	gc, ex = c.Get(1)
//	fmt.Println("Afetr overflow", gc, ex)
//
//	c.Clear()
//
//	gc, ex = c.Get(1)
//	fmt.Println(gc, ex)
//}
