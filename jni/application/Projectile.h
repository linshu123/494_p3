#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <Zeni/Collision.h>
#include <Zeni/Model.h>
#include <Zeni/Quaternion.h>
#include <Zeni/Sound.h>
#include <Zeni/Vector3f.h>
#include <utility>
#include <string>

namespace Crate {
    
	enum weapon_type {SPRINKLE, MM, CHOCOLATE, CANDLE, WEAPON_SIZE};

    class Crate;
    
    
	class Projectile {
    public:
        Projectile(Crate *player_,
                   const Zeni::Point3f &corner_ = Zeni::Point3f(0.0f, 0.0f, 0.0f),
              const Zeni::Vector3f &scale_ = Zeni::Vector3f(1.0f, 1.0f, 1.0f),
              const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f, 0.0f, 1.0f), 0.0f), 
			  weapon_type weap = SPRINKLE);
        Projectile(const Projectile &rhs);
        Projectile & operator=(const Projectile &rhs);
        ~Projectile();
        
        void render();
        
        void collide();
        
        const Zeni::Collision::Parallelepiped & get_body() const {return m_body;}

		void set_velocity(Zeni::Vector3f);
        
		void apply_gravity(float time_step);

		bool stopped();

		Zeni::Point3f get_corner(){
			return m_corner;
		}

		
		void detonate();

		bool finished_detonating(){
			return detonated;
		}
        
        Zeni::Point3f get_location(){
            return m_corner;
        }
        
        Zeni::Vector3f get_velocity(){
            return m_velocity;
        }
        
        int get_damage(){
            return damage;
        }
        
        float get_collision_radius();
		
        int get_damage_done(){
            return damage_done;
        }
        
        void add_damage_done(int damage_);
        
private:
        void create_body();
        
        // Level 1
        static Zeni::Model * m_model;
        static unsigned long m_instance_count;
        
        Zeni::Sound_Source * m_source;
        
        // Level 2
        Zeni::Point3f m_corner;
        Zeni::Vector3f m_scale;
        Zeni::Quaternion m_rotation;
		Zeni::Vector3f m_velocity;

		bool detonated;
		int times_doubled;

		int explosion_radius;
		weapon_type weapon;
        
        // Level 3
        Zeni::Collision::Parallelepiped m_body; // not motion so much as collision
        
        // Level 4
        // A stationary Projectile has no controls
        
        
        Crate *player;
        
        int damage;
        int damage_done;
        
    };
    
}

#endif
