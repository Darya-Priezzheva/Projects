package service

func ApplyMove(game *Game, c Coordinates, player int) {
	game.Field[c.X][c.Y] = player
}

func FieldFull(game *Game) bool {
	for i := 0; i < 3; i++ {
		for j := 0; j < 3; j++ {
			if game.Field[i][j] == EmptyCell {
				return false
			}
		}
	}
	return true
}

func Minimax(field [3][3]int, player int) MoveResult {
	if checkWin(field, PlayerO) {
		return MoveResult{Score: 10}
	}
	if checkWin(field, PlayerX) {
		return MoveResult{Score: -10}
	}

	empty := emptyTiles(field)
	if len(empty) == 0 {
		return MoveResult{Score: 0}
	}

	var moves []MoveResult

	for _, tile := range empty {
		newField := field
		newField[tile.X][tile.Y] = player

		var result MoveResult
		if player == PlayerO {
			result = Minimax(newField, PlayerX)
		} else {
			result = Minimax(newField, PlayerO)
		}

		moves = append(moves, MoveResult{
			Index: tile,
			Score: result.Score,
		})
	}
	bestMove := moves[0]

	if player == PlayerO {
		for _, m := range moves {
			if m.Score > bestMove.Score {
				bestMove = m
			}
		}
	} else {
		for _, m := range moves {
			if m.Score < bestMove.Score {
				bestMove = m
			}
		}
	}

	return bestMove
}

func emptyTiles(field [3][3]int) []Coordinates {
	var cords []Coordinates
	for i := 0; i < 3; i++ {
		for j := 0; j < 3; j++ {
			if field[i][j] == 0 {
				cords = append(cords, Coordinates{X: i, Y: j})
			}
		}
	}
	return cords
}

func checkWin(f [3][3]int, player int) bool {

	for i := 0; i < 3; i++ {
		if f[i][0]+f[i][1]+f[i][2] == player*3 {
			return true
		}
	}

	for i := 0; i < 3; i++ {
		if f[0][i]+f[1][i]+f[2][i] == player*3 {
			return true
		}
	}

	if f[0][0]+f[1][1]+f[2][2] == player*3 {
		return true
	}

	if f[0][2]+f[1][1]+f[2][0] == player*3 {
		return true
	}

	return false
}
