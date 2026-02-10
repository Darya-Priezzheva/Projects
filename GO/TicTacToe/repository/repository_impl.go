package repository

import (
	"errors"
	"sync"
)

type gameRepository struct {
	mu sync.RWMutex
	db map[string][9]int
}

func NewGameRepository() GameRepository {
	return &gameRepository{db: make(map[string][9]int)}
}

func (r *gameRepository) SaveGame(g *GameEntity) error {
	r.mu.Lock()
	defer r.mu.Unlock()
	r.db[g.ID] = g.Game
	return nil
}

func (r *gameRepository) GetGame(id string) (*GameEntity, error) {
	r.mu.RLock()
	defer r.mu.RUnlock()
	if _, ok := r.db[id]; !ok {
		return nil, errors.New("Game not found\n")
	}
	g := GameEntity{
		ID:   id,
		Game: r.db[id],
	}
	return &g, nil
}
