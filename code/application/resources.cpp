#include "resources.hpp"
#include <systems/model_manager/model_manager.hpp>
#include <systems/texture_manager/texture_manager.hpp>
#include <systems/entity/generic_id.hpp>
#include <data/world_data/world_data.hpp>
#include <utilities/directory.hpp>
#include <string>


namespace Resource {


void
load_default_resources(World_data::World *world)
{
  const std::string asset_path = util::get_resource_path() + "assets/";

  // Load models
  {
    auto model_pool = world->model_pool;
    
    const ::Model::Load_model model[Resource::Model::size] {
      ::Model::Load_model{Resource::Model::unit_cube,     std::string(asset_path + "models/unit_cube.obj")},
      ::Model::Load_model{Resource::Model::unit_plane,    std::string(asset_path + "models/unit_plane.obj")},
      ::Model::Load_model{Resource::Model::unit_tube,    std::string(asset_path + "models/unit_tube.obj")},
      ::Model::Load_model{Resource::Model::test_aabb,     std::string(asset_path + "models/aabb_offset_test.obj")},
    };
    
    Generic_id::ids_init_sequentially(model_pool->id, model_pool->size);
    ::Model::model_load(model, Resource::Model::size, model_pool->vbo, model_pool->aabb, model_pool->size);
  }
  
  // Load textures
  {
    auto texture_pool = world->texture_pool;
    
    const ::Texture::Load_texture tex[Resource::Texture::size] {
      ::Texture::Load_texture{Resource::Texture::dev_green,             std::string(asset_path + "textures/dev_grid_green_512.png").c_str()},
      ::Texture::Load_texture{Resource::Texture::dev_red,               std::string(asset_path + "textures/dev_grid_red_512.png").c_str()},
      ::Texture::Load_texture{Resource::Texture::dev_blue,              std::string(asset_path + "textures/dev_grid_blue_512.png").c_str()},
      ::Texture::Load_texture{Resource::Texture::dev_orange,            std::string(asset_path + "textures/dev_grid_orange_512.png").c_str()},
      ::Texture::Load_texture{Resource::Texture::dev_colored_squares,   std::string(asset_path + "textures/dev_colored_squares_512.png").c_str()},
      ::Texture::Load_texture{Resource::Texture::dev_squares,           std::string(asset_path + "textures/dev_squares_512.png").c_str()},
    };
    
    Generic_id::ids_init_sequentially(texture_pool->id, texture_pool->size);
    ::Texture::texture_load(tex, Resource::Texture::size, texture_pool->texture, texture_pool->size);
  }
  
  // Load audio
  {
    // TODO Need to hook up ids some how.
  
    auto audio_pool = world->audio_pool;
    
    const uint32_t num_of_files = 1;
    
    const char *audio_files[num_of_files] {
      "/Users/PhilCK/Developer/wired/assets/audio/temp_shot.wav",
    };
    
    Audio::load_samples(audio_files, num_of_files, audio_pool->samples, audio_pool->size);
  }
}


} // ns