package service

import (
	"TicTacToe/repository"
	"errors"

	"github.com/google/uuid"
)

type gameService struct {
	repository  repository.GameRepository
	calcService CalculateService
}

func NewGameService(repo repository.GameRepository, calcService CalculateService) GameService {
	return &gameService{
		repository:  repo,
		calcService: calcService,
	}
}

func (g *gameService) UpdateGame(updatedGame *Game) error {
	game, err := g.GetGame(updatedGame.ID)
	if err != nil {
		return err
	}

	c, err := g.calcService.ValidateField(updatedGame.Field, game.Field)
	if err != nil {
		return errors.New("Invalid move\n")
	}

	ApplyMove(updatedGame, c, PlayerX)

	gameEntity := ToEntity(*updatedGame)
	err = g.repository.SaveGame(&gameEntity)
	if err != nil {
		return err
	}

	gameOver := g.calcService.IsGameOver(updatedGame)
	if gameOver {
		return nil
	}

	compCord := g.calcService.CalculateComputerMove(updatedGame)
	ApplyMove(updatedGame, compCord, PlayerO)

	gameEntity = ToEntity(*updatedGame)
	err = g.repository.SaveGame(&gameEntity)
	if err != nil {
		return err
	}

	gameOver = g.calcService.IsGameOver(updatedGame)

	if gameOver {
		return nil
	}

	return nil
}

func (g *gameService) GetGame(id string) (*Game, error) {
	gameEntity, err := g.repository.GetGame(id)
	if err != nil {
		return nil, err
	}
	game := FromEntity(*gameEntity)
	return &game, nil
}

func (g *gameService) StartNewGame() Game {
	newID := uuid.New()

	game := Game{
		Field:    [3][3]int{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
		ID:       newID.String(),
		Player:   PlayerX,
		GameOver: false,
		Winner:   EmptyCell,
	}
	gameEntity := ToEntity(game)
	g.repository.SaveGame(&gameEntity)
	return game
}
