#include <string>
#include <iostream>
#include "assert.h"
#include "texture.h"

/* Quick utility function for texture creation */
int Texture::power_of_two(int input)
{
  int value = 1;

  while ( value < input ) {
    value <<= 1;
  }
  return value;
}

GLuint Texture::SDL_GL_LoadTexture(SDL_Surface *surface, GLfloat *texcoord)
{
  GLuint texture;
  int w, h;
  SDL_Surface *image;
  SDL_Rect area;
  Uint32 saved_flags;
  Uint8  saved_alpha;

  /* Use the surface width and height expanded to powers of 2 */
  w = power_of_two(surface->w);
  h = power_of_two(surface->h);
  texcoord[0] = 0.0f;         /* Min X */
  texcoord[1] = 0.0f;         /* Min Y */
  texcoord[2] = (GLfloat)surface->w / w;  /* Max X */
  texcoord[3] = (GLfloat)surface->h / h;  /* Max Y */

  image = SDL_CreateRGBSurface(
			       SDL_SWSURFACE,
			       w, h,
			       32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
			       0x000000FF,
			       0x0000FF00,
			       0x00FF0000,
            0xFF000000
#else
			       0xFF000000,
			       0x00FF0000,
			       0x0000FF00,
            0x000000FF
#endif
			       );
  if ( image == NULL ) {
    return 0;
  }

  /* Save the alpha blending attributes */
  saved_flags = surface->flags&(SDL_SRCALPHA|SDL_RLEACCELOK);
  saved_alpha = surface->format->alpha;
  if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
    SDL_SetAlpha(surface, 0, 0);
  }

  /* Copy the surface into the GL texture image */
  area.x = 0;
  area.y = 0;
  area.w = surface->w;
  area.h = surface->h;
  SDL_BlitSurface(surface, &area, image, &area);

  /* Restore the alpha blending attributes */
  if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
    SDL_SetAlpha(surface, saved_flags, saved_alpha);
  }

  /* Create an OpenGL texture for the image */
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D,
	       0,
	       GL_RGBA,
	       w, h,
	       0,
	       GL_RGBA,
	       GL_UNSIGNED_BYTE,
	       image->pixels);
  SDL_FreeSurface(image); /* No longer needed */

  return texture;
}

Texture::Texture(std::string img_name, Uint32 posx, Uint32 posy)
  : cGameObject(posx, posy){
  if( !(glIsEnabled( GL_TEXTURE_2D )) )
     glEnable(GL_TEXTURE_2D);

  //assert(&img_name != 0);
  assert(glIsEnabled(GL_TEXTURE_2D));
  img=NULL;
  //Load our img to an sdl Surface
  img = Engine::load_image(img_name);

  //Bounding box put to power of two
  bbox.w = power_of_two(img->w);
  bbox.h = power_of_two(img->h);
  GLfloat texcoords[4];

  texture_obj = SDL_GL_LoadTexture(img, texcoords);
  
  tex_x = texcoords[0];
  tex_y = texcoords[1];
  tex_w = texcoords[2];
  tex_h = texcoords[3];
  SDL_FreeSurface(img);
}

Texture::~Texture(){
  //glDeleteTextures(1, &texture_obj);
}

void Texture::delete_tex() {
  glDeleteTextures(1, &texture_obj);
}
void Texture::draw() {
  if( !(glIsEnabled( GL_TEXTURE_2D )) )
     glEnable(GL_TEXTURE_2D);

  //Make Alpha Transparent
  if( !(glIsEnabled( GL_BLEND )) )
    glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(1.0,1.0,1.0,1.0);

  assert(glIsEnabled(GL_TEXTURE_2D));
  //Calculate polygon dimensions
  float x,y,w,h,x2,y2;
  //  x = (float)position.x;
  //  y = (float)position.y;
  w = (float)bbox.w;
  h = (float)bbox.h;

  x2 = (w/2);
  y2 = (h/2);
  x = (w/2)*-1;
  y = (h/2)*-1;

  // draw the left polygon
  glPushMatrix();
  // translate the world coordinate system along the z axis, and to
  //0,0 being bottom left system
  glTranslatef((float)position.x + x2, (float)position.y + y2, 0.0);
  glScalef(1, 1, 1 );
  //  glRotatef(0.0, 0.0, 0.0, 1.0);

  // bind the texture  
  glBindTexture(GL_TEXTURE_2D, texture_obj);

  // draw the plane at the world origin
  glBegin(GL_QUADS);
  glTexCoord2f(tex_x, tex_y); glVertex3f(x, y2, 0.0);
  glTexCoord2f(tex_x, tex_h); glVertex3f(x, y, 0.0);
  glTexCoord2f(tex_w, tex_h); glVertex3f(x2, y, 0.0);
  glTexCoord2f(tex_w, tex_y); glVertex3f(x2, y2, 0.0);
  glEnd();

  glPopMatrix();

}
