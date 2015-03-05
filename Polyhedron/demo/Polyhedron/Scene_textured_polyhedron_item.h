#ifndef SCENE_TEXTURED_POLYHEDRON_ITEM_H
#define SCENE_TEXTURED_POLYHEDRON_ITEM_H
#include <GL/glew.h>
#include "Scene_textured_polyhedron_item_config.h"
#include "Scene_item.h"
#include "Viewer_interface.h"
#include "Textured_polyhedron_type_fwd.h"
#include <iostream>
#include "texture.h"

// This class represents a textured polyhedron in the OpenGL scene
class SCENE_TEXTURED_POLYHEDRON_ITEM_EXPORT Scene_textured_polyhedron_item 
  : public Scene_item {
  Q_OBJECT
public:  
  Scene_textured_polyhedron_item();
//   Scene_textured_polyhedron_item(const Scene_textured_polyhedron_item&);
  Scene_textured_polyhedron_item(const Textured_polyhedron& p);
  Scene_textured_polyhedron_item(Textured_polyhedron* const p);
  ~Scene_textured_polyhedron_item();

  Scene_textured_polyhedron_item* clone() const;
  
  // IO
  bool load(std::istream& in);
  bool save(std::ostream& out) const;

  // Function for displaying meta-data of the item
  virtual QString toolTip() const;

  // Indicate if rendering mode is supported
  virtual bool supportsRenderingMode(RenderingMode m) const { return m != Splatting; }
  // Points/Wireframe/Flat/Gouraud OpenGL drawing in a display list
   void draw() const {}
  virtual void draw(Viewer_interface*) const;
   virtual void draw_edges() const {}
   virtual void draw_edges(Viewer_interface* viewer) const;

  // Get wrapped textured_polyhedron
  Textured_polyhedron*       textured_polyhedron();
  const Textured_polyhedron* textured_polyhedron() const;

  // Get dimensions
  bool isFinite() const { return true; }
  bool isEmpty() const;
  Bbox bbox() const;

  virtual void changed();
  virtual void shading_mode_changed();
  virtual void selection_changed(bool);

private:
  Textured_polyhedron* poly;
  Texture texture;
  std::vector<float> positions_lines;
  std::vector<float> positions_facets;
  std::vector<float> normals;
  std::vector<float> textures_map_facets;
  std::vector<float> textures_map_lines;


  GLuint rendering_program_facets;
  GLuint rendering_program_lines;
  GLuint textureId;
  GLint location[10];
  GLint sampler_location;

  GLuint vao[1];
  GLuint buffer[5];
  bool smooth_shading;
  void initialize_buffers();
  void compile_shaders(void);
  void uniform_attrib(Viewer_interface*, int) const;
  void compute_normals_and_vertices(void);


}; // end class Scene_textured_polyhedron_item

#endif // SCENE_TEXTURED_POLYHEDRON_ITEM_H
