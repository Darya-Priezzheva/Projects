package service

type Game struct {
	Field    [3][3]int
	ID       string
	Player   int
	GameOver bool
	Winner   int
}

const (
	EmptyCell = 0
	PlayerX   = 1
	PlayerO   = -1
)

type Coordinates struct {
	X int
	Y int
}

type MoveResult struct {
	Index Coordinates
	Score int
}
