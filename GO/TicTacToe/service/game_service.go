package service

type GameService interface {
	UpdateGame(updatedGame *Game) error
	GetGame(id string) (*Game, error)
	StartNewGame() Game
}
