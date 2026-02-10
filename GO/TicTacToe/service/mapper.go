package service

import "TicTacToe/repository"

func ToEntity(g Game) repository.GameEntity {
	return repository.GameEntity{
		ID:   g.ID,
		Game: matrixToSlice(g.Field),
	}
}

func FromEntity(g repository.GameEntity) Game {
	return Game{
		ID:    g.ID,
		Field: sliceToMatrix(g.Game),
	}
}

func matrixToSlice(matrix [3][3]int) [9]int {
	var ent [9]int
	ind := 0
	for i := 0; i < 3; i++ {
		for j := 0; j < 3; j++ {
			ent[ind] = matrix[i][j]
			ind++
		}
	}
	return ent
}

func sliceToMatrix(sl [9]int) [3][3]int {
	var matrix [3][3]int
	ind := 0
	for i := 0; i < 3; i++ {
		for j := 0; j < 3; j++ {
			matrix[i][j] = sl[ind]
			ind++
		}
	}
	return matrix
}
