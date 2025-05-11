#include <SDL.h>
#include <iostream>
#include <vector>

bool gameOver;
int x, y, fruitX, fruitY, score;
int tailX[1000];
int tailY[1000];
int nTail;

class MySDL_Setup {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	int window_height = 400;
	int window_width = 400;

	int box_length = 20;
	int box_width = 20;
	
	int no_of_boxes_x_axis = window_width / box_width;
	int no_of_boxes_y_axis = window_height / box_length;
	
	const int total_no_of_boxes = no_of_boxes_x_axis * no_of_boxes_y_axis;
	
public:
	MySDL_Setup() {
		window = NULL;
		renderer = NULL;	
	}

	void MySDL_Initialize() {
		SDL_Init(SDL_INIT_EVERYTHING);

		window = SDL_CreateWindow("Snake Game in C++", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_height, window_width, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, 0);
	}

	void MySDL_End() {
		SDL_RenderPresent(renderer);
	}

	void MySDL_Draw() {
		SDL_Delay(100);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		
		/*
		SDL_Rect v[2500];
		int k = 0;
		for (int i = 0 ; i < no_of_boxes_y_axis ; i++) {
			for (int j = 0 ; j < no_of_boxes_x_axis ; j++) {
				v[k].w = box_width; 
				v[k].h = box_length;
				v[k].x = j * 20;
				v[k].y = i * 20;
				k++;
			}
		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRects(renderer, v, 2500 );
		*/

		std::vector<SDL_Rect> render_boxes;
		std::vector<SDL_Rect> render_snake;
		std::vector<SDL_Rect> render_fruit;
		for (int i = 0; i < no_of_boxes_y_axis; i++) {
			for (int j = 0; j < no_of_boxes_x_axis; j++) {
				if (j == 0 || j == no_of_boxes_x_axis-1 || i==0 || i == no_of_boxes_y_axis-1) {
					SDL_Rect rect;
					rect.w = box_width;
					rect.h = box_length;
					rect.x = j * box_width; 
					rect.y = i * box_length;
					render_boxes.emplace_back(rect);
				}
				if (i==y && j==x) {
					SDL_Rect rect;
					rect.w = box_width;
					rect.h = box_length;
					rect.x = j * box_width;
					rect.y = i * box_length;
					render_snake.emplace_back(rect);

					for (int i = 0; i < nTail ; i++) {
						SDL_Rect rect;
						rect.w = box_width;
						rect.h = box_length;
						rect.x = tailX[i] * box_width;
						rect.y = tailY[i] * box_length;
						render_snake.emplace_back(rect);
					}
				}
				else if (i == fruitY && j == fruitX) {
					SDL_Rect rect;
					rect.w = box_width;
					rect.h = box_length; 
					rect.x = j * box_width;
					rect.y = i * box_length;
					render_fruit.emplace_back(rect);
				}
			}
		}
		SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);
		SDL_RenderFillRects( renderer, render_boxes.data(), int(render_boxes.size()) );
		SDL_SetRenderDrawColor(renderer, 255,  255, 55, 255);
		SDL_RenderFillRects(renderer, render_snake.data(), int(render_snake.size()));
		SDL_SetRenderDrawColor(renderer, 255, 55, 55, 255);
		SDL_RenderFillRects(renderer, render_fruit.data(), int(render_fruit.size()));
	}

	int getWindowHeight() {return window_height;}
	void setWindowHeight(int window_height) {window_height = window_height;}
	int getWindowWidth() {return window_width;}
	void setWindowWidth(int window_width) {window_width = window_width;}
	int getNoOfBoxesXAxis() { return no_of_boxes_x_axis; }
	void setNoOfBoxesXAxis(int no_of_boxes_x_axis) { no_of_boxes_x_axis = no_of_boxes_x_axis; }
	int getNoOfBoxesYAxis() { return no_of_boxes_y_axis; }
	void setNoOfBoxesYAxis(int no_of_boxes_y_axis) { no_of_boxes_y_axis = no_of_boxes_y_axis; }

}MySDL_setup;

enum eDirection {
	STOP = 0, 
	LEFT, 
	RIGHT, 
	UP, 
	DOWN
} direction;

void Spawn_Fruit_Randomly() {
	srand((unsigned int)time(NULL));
	fruitX = (rand() % (MySDL_setup.getNoOfBoxesXAxis() - 2)) + 1;
	srand((unsigned int)time(NULL));
	fruitY = (rand() % (MySDL_setup.getNoOfBoxesYAxis() - 2)) + 1;
}

void Spawn_Snake_At_Center() {
	x = MySDL_setup.getNoOfBoxesXAxis() / 2;
	y = MySDL_setup.getNoOfBoxesYAxis() / 2;
}

void Game_Initialize(){
	gameOver = false;
	direction = STOP;
	Spawn_Snake_At_Center();
	Spawn_Fruit_Randomly();
	score = 0;
}

void Check_Input() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {

		}
		else if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
			case SDLK_RIGHT:
				std::cout << "right key was pressed" << std::endl;
				direction = RIGHT;
				//if (rect.x % window_length == 0) rect.x = 0;
				break;
			case SDLK_LEFT:
				std::cout << "left key was pressed" << std::endl;
				direction = LEFT;
				break;
			case SDLK_UP:
				std::cout << "up key was pressed" << std::endl;
				direction = UP;
				//if (rect.x < 0) rect.x = 0;
				break;
			case SDLK_DOWN:
				std::cout << "down key was pressed" << std::endl;
				direction = DOWN;
				//if (rect.x < 0) rect.x = 0;
				break;
			}
		}
		else if (e.type == SDL_MOUSEMOTION) {
			//SDL_GetMouseState(&rect.x, &rect.y);
		}
	}
}

void Logic() { 

	int prevX = x;
	int prevY = y;

	switch (direction) {
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
		break;
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	default:
		break;
	}

	//check if snake is touching boundary
	if (x == MySDL_setup.getNoOfBoxesXAxis() - 1 || x == 0 || y == MySDL_setup.getNoOfBoxesYAxis() - 1 || y == 0) {
		gameOver = true;
	}

	//check if snake is touching itself
	for (int i = nTail; i > 0; i--) {
		if (x == tailX[i] && y == tailY[i]) {
			gameOver = true;
		}
	}

	if (x == MySDL_setup.getNoOfBoxesXAxis() - 1 || x == 0 || y == MySDL_setup.getNoOfBoxesYAxis() - 1 || y == 0) {
		gameOver = true;
	}

	std::cout << "x : " << x << std::endl;
	std::cout << "y : " << y << std::endl;

	if (nTail > 0) {
		if (nTail == 1) {
			tailX[0] = prevX;
			tailY[0] = prevY;
		}
		else {
			for (int i = nTail ; i > 0 ; i--) {
				tailX[i] = tailX[i - 1];
				tailY[i] = tailY[i - 1];
			}
			tailX[0] = prevX;
			tailY[0] = prevY;
		}

	}

	//check if snake is touching fruit
	if (x == fruitX && y == fruitY) {
		std::cout << "hello i ate the fruit" << std::endl;
		score += 10; //increase score
		nTail++; //increase tail 
		Spawn_Fruit_Randomly(); 
	}

}

int main(int argc, char* argv[]) {

	Game_Initialize();
	MySDL_setup.MySDL_Initialize();
	
	while (!gameOver) {

		MySDL_setup.MySDL_Draw();

		Check_Input();
		Logic();

		MySDL_setup.MySDL_End();
	}

	return 0;
}//main