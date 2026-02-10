package main

import (
	"TicTacToe/di"
	"TicTacToe/handler"
	"TicTacToe/repository"
	"TicTacToe/service"

	"go.uber.org/fx"
)

func main() {
	fx.New(
		fx.Provide(
			repository.NewGameRepository,
			service.NewCalculateService,
			service.NewGameService,
			handler.NewGameHandler,
		),
		fx.Invoke(
			di.RegisterHTTP,
		),
	).Run()
}
