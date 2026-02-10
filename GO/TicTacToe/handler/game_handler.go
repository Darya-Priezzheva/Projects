package handler

import (
	"TicTacToe/service"
	"encoding/json"
	"net/http"
)

type GameHandler struct {
	service service.GameService
}

func NewGameHandler(service service.GameService) *GameHandler {
	return &GameHandler{service: service}
}

func (handler *GameHandler) UpdateGameHandler(w http.ResponseWriter, r *http.Request) {
	var gameRequest UpdateGameRequest
	if err := json.NewDecoder(r.Body).Decode(&gameRequest); err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
		return
	}

	updatedGame := RequestToService(gameRequest)
	err := handler.service.UpdateGame(updatedGame)
	if err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
	}

	resp := ServiceToResponse(updatedGame)
	json.NewEncoder(w).Encode(resp)
}

func (handler *GameHandler) CreateGameHandler(w http.ResponseWriter, r *http.Request) {
	game := handler.service.StartNewGame()
	resp := ServiceToResponse(&game)
	json.NewEncoder(w).Encode(resp)
}
