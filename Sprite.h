#include "SDL.h"
#include <vector>

using namespace std;

class Sprite{
  
 protected:
  SDL_Surface* image;
  SDL_Rect rect;
  int origin_x, origin_y;
  
 public:
  
  Sprite(Uint32 color, int x, int y, int w = 48, int h = 64);
  void update();
  void draw(SDL_Surface* destination);
  SDL_Surface* get_image() const;
  bool operator==(const Sprite &other) const;
};

class SpriteGroup{

 private:
  vector <Sprite*> sprites;
  int sprites_size;
  
 public:

  vector <Sprite*> getSprites();
  SpriteGroup copy();
  void draw(SDL_Surface* destination);
  void add(Sprite* sprite);
  void remove(Sprite sprite_object);
  bool has(Sprite sprite_object);
  void update();
  void empty();
  int size();
};
