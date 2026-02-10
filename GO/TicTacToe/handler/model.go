package handler

type UpdateGameRequest struct {
	Field [3][3]int `json:"field"`
	ID    string    `json:"id"`
}

type GetGameRequest struct {
	ID string `json:"id"`
}
type Coordinates struct {
	X int `json:"x"`
	Y int `json:"y"`
}

type GameResponse struct {
	Field    [3][3]int `json:"field"`
	ID       string    `json:"id"`
	GameOver bool      `json:"game_over"`
	Winner   string    `json:"winner"`
}
