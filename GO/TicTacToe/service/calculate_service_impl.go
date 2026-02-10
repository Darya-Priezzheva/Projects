package service

import (
	"errors"
)

type calculateService struct{}

func NewCalculateService() CalculateService {
	return &calculateService{}
}

func (g *calculateService) CalculateComputerMove(game *Game) Coordinates {
	moveRes := Minimax(game.Field, PlayerO)
	return moveRes.Index
}

func (g *calculateService) ValidateField(updatedField [3][3]int, savedField [3][3]int) (Coordinates, error) {
	cnt := 0
	var move Coordinates
	var player int

	for i := 0; i < 3; i++ {
		for j := 0; j < 3; j++ {
			if updatedField[i][j] != savedField[i][j] {
				cnt++
				move = Coordinates{X: i, Y: j}
				player = updatedField[i][j]
			}
		}
	}

	if cnt != 1 {
		return move, errors.New("Too many moves\n")
	}

	if player != PlayerX {
		return move, errors.New("Incorrect move\n")
	}

	return move, nil
}

func (g *calculateService) IsGameOver(game *Game) bool {

	for i := 0; i < 3; i++ {
		if game.Field[i][0]+game.Field[i][1]+game.Field[i][2] == PlayerX*3 {
			game.Winner = PlayerX
			game.GameOver = true
			return true
		}
		if game.Field[i][0]+game.Field[i][1]+game.Field[i][2] == PlayerO*3 {
			game.Winner = PlayerO
			game.GameOver = true
			return true
		}
	}

	for i := 0; i < 3; i++ {
		if game.Field[0][i]+game.Field[1][i]+game.Field[2][i] == PlayerX*3 {
			game.Winner = PlayerX
			game.GameOver = true
			return true
		}
		if game.Field[0][i]+game.Field[1][i]+game.Field[2][i] == PlayerO*3 {
			game.Winner = PlayerO
			game.GameOver = true
			return true
		}
	}

	if game.Field[0][0]+game.Field[1][1]+game.Field[2][2] == PlayerX*3 {
		game.Winner = PlayerX
		game.GameOver = true
		return true
	}
	if game.Field[0][0]+game.Field[1][1]+game.Field[2][2] == PlayerO*3 {
		game.Winner = PlayerO
		game.GameOver = true
		return true
	}

	if game.Field[0][2]+game.Field[1][1]+game.Field[2][0] == PlayerX*3 {
		game.Winner = PlayerX
		game.GameOver = true
		return true
	}
	if game.Field[0][2]+game.Field[1][1]+game.Field[2][0] == PlayerO*3 {
		game.Winner = PlayerO
		game.GameOver = true
		return true
	}

	if FieldFull(game) {
		game.Winner = EmptyCell
		game.GameOver = true
		return true
	}

	return false
}
