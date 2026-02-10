package handler

import (
	"TicTacToe/service"
)

func RequestToService(req UpdateGameRequest) *service.Game {
	return &service.Game{
		Field:    req.Field,
		ID:       req.ID,
		Player:   1,
		Winner:   service.EmptyCell,
		GameOver: false,
	}
}

func ServiceToResponse(g *service.Game) GameResponse {
	var win string
	if g.Winner == service.PlayerX {
		win = "X"
	}
	if g.Winner == service.PlayerO {
		win = "О"
	}
	return GameResponse{
		Field:    g.Field,
		ID:       g.ID,
		GameOver: g.GameOver,
		Winner:   win,
	}
}
