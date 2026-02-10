package repository

type GameRepository interface {
	SaveGame(g *GameEntity) error
	GetGame(id string) (*GameEntity, error)
}
