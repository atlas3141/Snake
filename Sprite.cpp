#include "Sprite.h"

using namespace std;

Sprite::Sprite (Uint32 color, int x, int y, int w, int h){
  image = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);

  SDL_FillRect(image, NULL, color);
  rect = image->clip_rect;

  origin_y = 0;
  origin_x = 0;

  rect.x = x - origin_x;
  rect.y = y - origin_y;
}
void Sprite::update(){

}
void Sprite::draw(SDL_Surface* destination){
  SDL_BlitSurface(image,NULL,destination,&rect);
}
SDL_Surface* Sprite::get_image() const{
  return image;
}
bool Sprite::operator==(const Sprite &other) const{
  return(image == other.get_image());
}



//SpriteGroup

vector<Sprite*> SpriteGroup::getSprites(){
  return sprites;
}
SpriteGroup SpriteGroup::copy(){
  SpriteGroup new_group;
  for (int i = 0; i < sprites_size; i++){
    new_group.add(sprites[i]);
  }
  return new_group;
}
void SpriteGroup::add(Sprite* sprite){
  sprites.push_back(sprite);
  sprites_size = sprites.size();
}
void SpriteGroup::remove(Sprite sprite_object){
  for(int i = 0; i < sprites_size; i++){
    if(*sprites[i] == sprite_object){
      sprites.erase(sprites.begin() + i);
    }
  }
  sprites_size = sprites.size();
}
bool SpriteGroup::has(Sprite sprite_object){
  for(int i = 0; i < sprites_size; i++){
    if(*sprites[i] == sprite_object){
      return true;
    }
  }
}
void SpriteGroup::update(){
  if (!sprites.empty()){
    for(int i = 0; i < sprites_size; i++){
      sprites[i]->update();
    }
  }
}
void SpriteGroup::draw(SDL_Surface* destination){
  if (!sprites.empty()){
    for(int i = 0; i < sprites_size; i++){
      sprites[i]->draw(destination);
    }
  }
}
void SpriteGroup::empty(){
  sprites.clear();
  sprites_size = sprites.size();
}
int SpriteGroup::size(){
  return sprites_size;
}
