#ifndef CANNON_H
#define CANNON_H

#include <Zeni/Collision.h>
#include <Zeni/Model.h>
#include <Zeni/Quaternion.h>
#include <Zeni/Sound.h>
#include <Zeni/Vector3f.h>
#include <utility>
#include "Projectile.h"


namespace Crate {
    
	class Cannon {
		public:
			Cannon(const Zeni::Point3f &corner_ = Zeni::Point3f(0.0f, 0.0f, 0.0f),
				  const Zeni::Vector3f &scale_ = Zeni::Vector3f(1.0f, 1.0f, 1.0f),
				  const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f, 0.0f, 1.0f), 0.0f));
   
        
			void render();
        
			void collide();
        
			const Zeni::Collision::Parallelepiped & get_body() const {return m_body;}

			void rotate_up();
			void rotate_down();
			void rotate_left();
			void rotate_right();

			void change_loc(Zeni::Point3f);

			Projectile * fire(float, Crate *);

			float get_vertical_angle(){
				return vertical_angle;
			}

			float get_horizontal_angle(){
				return horizontal_angle;
			}

			void switch_rotation_angle();

			void switch_weapon();

			weapon_type get_weapon(){
				return weapon;
			}
        
            Zeni::Point3f get_position(){
                return m_corner;
            }
        
        Zeni::Point3f get_cannon_tip(float length = 25.0f);

		private:
			void create_body();
        
			// Level 1
			static Zeni::Model * m_model;
			static unsigned long m_instance_count;

			float horizontal_angle;
			float vertical_angle;
			        
			Zeni::Sound_Source * m_source;
        
			// Level 2
			Zeni::Point3f m_corner;
			Zeni::Vector3f m_scale;
			Zeni::Quaternion m_rotation;

			float rotation_angle;
			bool alive;

			weapon_type weapon;
        
			// Level 3
			Zeni::Collision::Parallelepiped m_body; // not motion so much as collision
        
			// Level 4
			// A stationary Crate has no controls
		};
}

#endif