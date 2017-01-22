#ifndef INC_TEXTURE_H
#define INC_TEXTURE_H

/* #ifndef _WINDOWS */
/* #include "SDL/SDL_OpenGL.h" */
/* #endif  /\* _WINDOWS *\/ */

#include <string>
#include <GL/gl.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "engine.h"

class Texture : public cGameObject
{
 private:
  SDL_Surface *img;
  Uint32 texture_obj;
  GLfloat tex_x, tex_y, tex_w, tex_h;

 protected:

 public:
  Texture(std::string img_name, Uint32 posx = 0, Uint32 posy = 0);
  ~Texture();
  void draw();
  int power_of_two(int input);
  GLuint SDL_GL_LoadTexture(SDL_Surface *surface, GLfloat *texcoord);  
  void delete_tex();
 };

#endif  /* INC_TEXTURE_H */
