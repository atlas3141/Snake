#include "SDL.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;


class Sprite{
  
 protected:
  SDL_Surface* image;
  SDL_Rect rect;
  int origin_x, origin_y;
  
 public:
  Sprite (Uint32 color, int x, int y, int w = 48, int h = 64){
    image = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);

    SDL_FillRect(image, NULL, color);
    rect = image->clip_rect;

    origin_y = 0;
    origin_x = 0;

    rect.x = x - origin_x;
    rect.y = y - origin_y;
  }
  void update(){

  }
  void draw(SDL_Surface* destination){
    SDL_BlitSurface(image,NULL,destination,&rect);
  }
  SDL_Surface* get_image() const {
    return image;
  }
  bool operator==(const Sprite &other) const{
    return(image == other.get_image());
  }
};

class SpriteGroup{
  
private:
  vector <Sprite*> sprites;
  int sprites_size;
  
  

 public:

  vector <Sprite*> getSprites(){
    return sprites;
  }
  SpriteGroup copy(){
    SpriteGroup new_group;
    for (int i = 0; i < sprites_size; i++){
      new_group.add(sprites[i]);
    }
    return new_group;
  }
  void add(Sprite* sprite){
    sprites.push_back(sprite);
    sprites_size = sprites.size();
  }
  void remove(Sprite sprite_object){
    for(int i = 0; i < sprites_size; i++){
      if(*sprites[i] == sprite_object){
	sprites.erase(sprites.begin() + i);
      }
    }
    sprites_size = sprites.size();
  }
  bool has(Sprite sprite_object){
    for(int i = 0; i < sprites_size; i++){
      if(*sprites[i] == sprite_object){
	return true;
      }
    }
  }
  void update(){
    if (!sprites.empty()){
      for(int i = 0; i < sprites_size; i++){
	sprites[i]->update();
      }
    }
  }
  void draw(SDL_Surface* destination){
    if (!sprites.empty()){
      for(int i = 0; i < sprites_size; i++){
        sprites[i]->draw(destination);
      }
    }
  }
  void empty(){
    sprites.clear();
    sprites_size = sprites.size();
  }
  int size(){
    return sprites_size;
  }
};

class Segment {
private:
  Sprite* sprite;
  int age;
  int x, y;
public:
  Segment(int nx, int ny, SpriteGroup* group, SDL_Surface* screen, int board[50][50] ){
    x = nx;
    y = ny;
    Uint32 red = SDL_MapRGB(screen->format,255,0,0);
    sprite = new Sprite (red, x, y, 10, 10);
    group->add(sprite);
    board[x/10][y/10] = 1;
    age = 0;
  }
  bool decay(int lifespan, SpriteGroup* group, vector<Segment*>* list, int index, int (&board)[50][50]){
    age++;
    if (age > lifespan){
      group->remove(*sprite);
      list->erase(list->begin()+index);
     
      board[x/10][y/10] = 0;
  
      return true;
    }
    else return false;
  }
};


int main(){
  srand (time(NULL));
  int window_height = 500;
  int window_width = 500;
  SDL_Init(SDL_INIT_EVERYTHING);
  
  SDL_Window *window = NULL;
  window = SDL_CreateWindow ("Window", 
			     SDL_WINDOWPOS_UNDEFINED, 
			     SDL_WINDOWPOS_UNDEFINED, 
			     500, 500, 
			     SDL_WINDOW_RESIZABLE);
  if (window == NULL){
    cout << "Something Happened" << endl;
  }
  
  SDL_Surface *screen = SDL_GetWindowSurface(window);
  
  Uint32 white = SDL_MapRGB(screen->format,255,255,255);
  Uint32 red = SDL_MapRGB(screen->format,255,0,0);
  Uint32 blue = SDL_MapRGB(screen->format,0,0,255);

  SDL_FillRect(screen, NULL, white);
  
  SpriteGroup active_sprites;

  int board[50][50];
  for (int i = 0; i < 50; i++){
    for (int j =  0; j < 50; j++){
      board[i][j] = 0;
    }
  }
  Segment segment (30,30, &active_sprites, screen, board);

  Sprite* object = new Sprite(blue,250,250,10,10);
  active_sprites.add(object);
  board[25][25] = 2;

  Uint32 startingTick;
  SDL_Event event;
  bool running = true;
  int speed = 100;
  Uint32 lastMove = SDL_GetTicks();
  int headPos_x  = 0; 
  int headPos_y = 0;
  int lifespan = 1;  
  vector<Segment*> segments;
  int direction = 0;
  bool pressed;
  segments.push_back(&segment);



  while (running == true){
    startingTick = SDL_GetTicks();
    SDL_FillRect(screen, NULL, white);
    active_sprites.draw(screen);
    SDL_UpdateWindowSurface(window);

    while (SDL_PollEvent(&event)){
      
      if (event.type == SDL_QUIT){
	running = false;
	cout << "Goodbye" << endl;
	break;
      }
      if (event.type == SDL_KEYDOWN){
	if (event.key.keysym.sym == SDLK_UP && direction != 3 && !pressed){
	  direction = 1;
	  pressed = true;
	}
	else if (event.key.keysym.sym == SDLK_RIGHT && direction != 4 && !pressed){
	  direction = 2;
	  pressed = true;
	}
	else if (event.key.keysym.sym == SDLK_DOWN && direction != 1 && !pressed){
	  direction = 3;
	  pressed = true;
	}
	else if (event.key.keysym.sym == SDLK_LEFT && direction != 2 && !pressed){
	  direction = 4;
	  pressed = true;
	}
	if(event.key.keysym.sym == SDLK_a){
	  for (int i = 0; i < 50; i++){
	    for (int j =  0; j < 50; j++){
	      cout<< board[i][j];
	    }
	    cout << endl;
	  }
	}
      }
    }

    if(SDL_GetTicks() - lastMove > speed){
      pressed = false;
      lastMove = SDL_GetTicks();
      for(int i = 0; i < segments.size(); i++){
	segments[i]->decay(lifespan,&active_sprites,&segments,i,board);
      }
      
      if(direction == 1)
	headPos_x -=10;
      
      else if(direction == 2)
	headPos_y +=10;
      else if(direction == 3)
	headPos_x +=10;
      else if(direction == 4)
        headPos_y -=10;
      if(board[headPos_y/10][headPos_x/10] == 2){
	lifespan += 5;
	active_sprites.remove(*object);
	board[headPos_y/10][headPos_x/10] = 0;
	int newX,newY;
	do{
	   newX = rand() %51;
	   newY = rand() %51;
	}while(board[newX][newY] != 0);
	board[newX][newY] = 2;
	delete object;
	object = new Sprite(blue,newX*10,newY*10,10,10);
	active_sprites.add(object);
    }
   
      if( board[headPos_y/10][headPos_x/10] == 1 ||
	  headPos_y < 0 || headPos_y > 500 ||
	  headPos_x < 0 || headPos_x > 500){
	segments.clear();
	delete object;
	active_sprites.empty();
	object = new Sprite(blue, 250,250,10,10);
	active_sprites.add(object);
	lifespan = 1;
	headPos_y = 0;
	headPos_x = 0;
	direction = 0;
	for (int i = 0; i < 50; i++){
	  for (int j =  0; j < 50; j++){
	    board[i][j] = 0;
	  }
	}
	board[25][25] = 2;
	segments.push_back(new Segment(headPos_y,headPos_x, &active_sprites, screen, board));
      }
      segments.push_back(new Segment(headPos_y,headPos_x, &active_sprites, screen, board));
    }
    
    if((1000/60)>SDL_GetTicks() - startingTick){
      SDL_Delay(1000/60 - (SDL_GetTicks()-startingTick));
      
    } 
  }
  
  
  SDL_DestroyWindow(window);
  
  SDL_Quit();
  
  return 0;
}

