#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
   vec3 color, t, view, reflection, ambient, diffuse, specular;
   Light *cur_light;
   Ray shadow_ray;
   Hit intersect = {0,0,0};

   ambient = color_ambient * world.ambient_intensity * world.ambient_color; 
  
   for(unsigned int i = 0; i < world.lights.size(); i++){
   	cur_light = world.lights.at(i);
   	t = cur_light->position - intersection_point;

	shadow_ray.endpoint = intersection_point;
	shadow_ray.direction = t.normalized(); 

	intersect = world.Closest_Intersection(shadow_ray);
        reflection = (t - 2 * dot(t, normal.normalized()) * normal).normalized();

	if(!world.enable_shadows || (world.enable_shadows && !intersect.object || intersect.dist >= t.magnitude() ) ){
		specular = specular + color_specular * cur_light->Emitted_Light(-t) * pow(std::max(dot(ray.direction, reflection), 0.0), specular_power);
        	diffuse = diffuse + color_diffuse * cur_light->Emitted_Light(-t) * std::max(dot(normal.normalized(),shadow_ray.direction) , 0.0);
	}
   } 
color = ambient + diffuse + specular;
   return color;
}
