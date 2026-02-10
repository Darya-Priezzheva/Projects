package di

import (
	"TicTacToe/handler"
	"context"
	"net/http"

	"go.uber.org/fx"
)

func RegisterHTTP(
	lc fx.Lifecycle,
	h *handler.GameHandler,
) {
	lc.Append(fx.Hook{
		OnStart: func(ctx context.Context) error {

			http.Handle(
				"/game/start",
				CorsMiddleware(http.HandlerFunc(h.CreateGameHandler)),
			)

			http.Handle(
				"/game/move",
				CorsMiddleware(http.HandlerFunc(h.UpdateGameHandler)),
			)

			http.Handle("/", http.FileServer(http.Dir("./web")))

			go http.ListenAndServe(":8080", nil)
			return nil
		},
	})
}
