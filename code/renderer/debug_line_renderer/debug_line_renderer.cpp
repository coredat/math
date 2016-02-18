#include "debug_line_renderer.hpp"
#include "../graphics_api/ogl/ogl_common.hpp"
#include "../graphics_api/ogl/ogl_texture.hpp"
#include "../graphics_api/ogl/ogl_shader.hpp"
#include "../graphics_api/ogl/ogl_shader_uniform.hpp"
#include "../graphics_api/ogl/ogl_pixel_format.hpp"
#include "../graphics_api/pixel_format.hpp"
#include "../graphics_api/ogl/ogl_texture_filtering.hpp"
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <vector>
#include <cstddef>


namespace
{
  Ogl::Shader debug_line_shader;
  
  GLint uni_wvp(0);
  GLint uni_data(0);
  
  constexpr size_t number_of_lines = 32;
  constexpr size_t number_of_components = 3;
  constexpr size_t line_uniform_max = number_of_lines * number_of_components; // 64 lines, 3 components per line (start, end, color)
  GLint uni_line[line_uniform_max];
  
  constexpr size_t size_of_data_buffer = (1 << 20) * number_of_components; // Some big size * number of components (start, end, color)
  float data[size_of_data_buffer];
  
  size_t data_ptr = 0;
}


namespace Debug_line_renderer {


void
initialize()
{
  Ogl::error_clear();

  const char* vertex_shader = R"(
    #version 150 core

    out int gs_in_vert_id;
  
    void main()
    {
      gs_in_vert_id = gl_VertexID;
    }
  )";
  
  const char* geometry_shader = R"(
    #version 330

    layout (points) in;
    layout (line_strip, max_vertices = 2) out;
  
    in int              gs_in_vert_id[];
  
    uniform mat4        uni_wvp_mat;
    uniform sampler2D 	uni_data_lookup;

    #define NUM_LINES 32
    #define COMPONENTS_PER_LINE 3

    uniform vec3        uni_line[NUM_LINES * COMPONENTS_PER_LINE]; // In groups of three, start, end, color.
  
    out vec3            ps_in_color;

    void main()
    {
      int id = gs_in_vert_id[0] * COMPONENTS_PER_LINE;
      
      int start = id + 0;
      int end   = id + 1;
      int color = id + 2;
      
      ps_in_color = uni_line[color];
      
      // Generate the primitive //

      gl_Position = uni_wvp_mat * vec4(uni_line[start], 1);
      EmitVertex();
      
      gl_Position = uni_wvp_mat * vec4(uni_line[end], 1);
      EmitVertex();
      
      EndPrimitive();
    }

  )";
  
  const char* fragment_shader = R"(
    #version 150

    in vec3 ps_in_color;
  
    out vec4 ps_out_color;

    void main()
    {
      ps_out_color = vec4(ps_in_color, 1.0);
    }
  )";
  
  Ogl::shader_create(&debug_line_shader, vertex_shader, geometry_shader, fragment_shader, &std::cout);
  Ogl::error_check("Building debug line shader.", &std::cout);
  
  Ogl::Shader_uniforms uniforms;
  Ogl::shader_uniforms_retrive(&uniforms, &debug_line_shader);
  Ogl::error_check("Getting uniforms from debug shader", &std::cout);
  
  Ogl::shader_uniforms_get_uniform_index(&uni_wvp, &uniforms, "uni_wvp_mat");
  Ogl::shader_uniforms_get_uniform_index(&uni_data, &uniforms, "uni_data_lookup");
  
  for(size_t i = 0; i < line_uniform_max; ++i)
  {
    const std::string uni_name = "uni_line[" + std::to_string(i) + "]";
    uni_line[i] = glGetUniformLocation(debug_line_shader.program_id, uni_name.c_str());
  }
  
  Ogl::error_check("Debug Renderer Setup.", &std::cout);
}


void
add_lines(const Line_node nodes[], const std::size_t number_of_lines)
{
  // Shit - can do two memcpy's instead
  for(int32_t i = 0; i < number_of_lines; ++i)
  {
    if(data_ptr >= size_of_data_buffer)
    {
      assert(false); // trying to render more lines than we have capacity for.
      return;
    }
    
    data[data_ptr++] = nodes[i].position_from[0];
    data[data_ptr++] = nodes[i].position_from[1];
    data[data_ptr++] = nodes[i].position_from[2];
    
    data[data_ptr++] = nodes[i].position_to[0];
    data[data_ptr++] = nodes[i].position_to[1];
    data[data_ptr++] = nodes[i].position_to[2];
    
    data[data_ptr++] = nodes[i].color[0];
    data[data_ptr++] = nodes[i].color[1];
    data[data_ptr++] = nodes[i].color[2];
  }
}


void
render(const float wvp_mat[16])
{
  const size_t number_to_batch = (data_ptr / number_of_lines) + 1;
  
  // Render
  Ogl::default_state();
  
  size_t data_get = 0; // counter to how many we have already draw from the buffer.
  
  //glUseProgram(debug_line_shader.program_id);
  Ogl::shader_bind(&debug_line_shader);
  Ogl::error_check("Use program", &std::cout);
  
  glUniformMatrix4fv(uni_wvp, 1, GL_FALSE, wvp_mat);
  Ogl::error_check("set wvp.", &std::cout);
  
  for(size_t b = 0; b < number_to_batch; ++b)
  {
    for(size_t l = 0; l < std::min<size_t>(number_of_lines, data_ptr); ++l)
    {
      const size_t uni = l * number_of_components;
      
      // TODO: If I stored the info in a 3x3 matrix would uploading be quicker?
      glUniform3f(uni_line[uni + 0], data[data_get+ 0], data[data_get+ 1], data[data_get+2]);
      glUniform3f(uni_line[uni + 1], data[data_get+ 3], data[data_get+ 4], data[data_get+5]);
      glUniform3f(uni_line[uni + 2], data[data_get+ 6], data[data_get+ 7], data[data_get+8]);
      
      data_get += number_of_components * 3; // * 3 because we are doing 3 at a time.
      data_ptr -= (number_of_components * 3);
    }
    
    Ogl::error_check("Uploading debug line uniforms", &std::cout);
    
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(line_uniform_max));
  }
  
  assert(data_ptr == 0); // We need to have drawn all the lines, if not somethings out of sync.

  Ogl::error_check("Debug line renderer.", &std::cout);
}


} // ns