package service

type CalculateService interface {
	CalculateComputerMove(game *Game) Coordinates
	ValidateField(updatedField [3][3]int, savedField [3][3]int) (Coordinates, error)
	IsGameOver(game *Game) bool
}
