#include <zenilib.h>

#include "Cannon.h"

using namespace Zeni;
using namespace Zeni::Collision;

namespace Crate {
    

	Cannon::Cannon(const Point3f &corner_,
					 const Vector3f &scale_,
					 const Quaternion &rotation_)
		: m_source(new Sound_Source(get_Sounds()["collide"])),
		m_corner(corner_),
		m_scale(scale_),
		m_rotation(rotation_)
		{
			if(!m_instance_count)
				m_model = new Model("models/tank_cannon.3ds");
			++m_instance_count;

			horizontal_angle = 0.0f;
			vertical_angle = 0.0f;

			rotation_angle = 3.14/180.0f;

			weapon = SPRINKLE;
        
			create_body();
		}

	void Cannon::create_body() {
		m_body = Parallelepiped(m_corner,
								m_rotation * m_scale.get_i(),
								m_rotation * m_scale.get_j(),
								m_rotation * m_scale.get_k());
        
		m_source->set_position(m_corner + m_rotation * m_scale / 2.0f);
	}

	void Cannon::render() {
        const std::pair<Vector3f, float> rotation = m_rotation.get_rotation();
        
        m_model->set_translate(m_corner);
        m_model->set_scale(m_scale);
        m_model->set_rotate(rotation.second, rotation.first);
        
        m_model->render();
    }
	    
		void Cannon::rotate_down(){
			m_rotation = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(sin(horizontal_angle), cos(horizontal_angle), 0), -rotation_angle) * m_rotation;
			vertical_angle += rotation_angle;
		}

		void Cannon::rotate_up(){ 
			m_rotation = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(sin(horizontal_angle), cos(horizontal_angle), 0), rotation_angle) * m_rotation;
			vertical_angle -= rotation_angle;
		}
		
		void Cannon::rotate_right(){
			horizontal_angle += rotation_angle;
			m_rotation = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0, 0, 1), -rotation_angle) * m_rotation;
		}

		void Cannon::rotate_left(){ 
			m_rotation = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0, 0, 1), rotation_angle) * m_rotation;
			horizontal_angle -= rotation_angle;
		}

		void Cannon::change_loc(Point3f loc){
			loc.z += 15;
			m_corner = loc;
		}

	Projectile * Cannon::fire(float power){

		
		float projectile_angle = m_rotation.get_rotation().second;
		Vector3f vec = m_rotation.get_rotation().first;
		if (vec.k > 0){
			projectile_angle *= -1;
		}
		Zeni::Point3f cannon_tip = m_corner;
		float length = 25.0f;
		cannon_tip.x += -1.0f * length * cos(vertical_angle) * cos(horizontal_angle);
		cannon_tip.y += length * cos(vertical_angle) * sin(horizontal_angle);
		cannon_tip.z += -length * sin(vertical_angle);

		Projectile *proj = new Projectile(cannon_tip, Vector3f(15.0f, 15.0f, 15.0f), m_rotation, weapon);
		/*
		float z_velocity = -1.0f * power * sin(vertical_angle);
		float x_velocity = -1.0f * z_velocity * cos(horizontal_angle);
		float y_velocity = 1.0f * z_velocity*sin(horizontal_angle);
		*/

		float z_velocity = -1.0f * power * sin(vertical_angle);
		float x_velocity = -1.0f * power * cos(vertical_angle) * cos(horizontal_angle);
		float y_velocity = 1.0f * power * cos(vertical_angle) * sin(horizontal_angle);


		//Vector3f initial_velocity(-1.0f * cannon_power*cos(projectile_angle), cannon_power*sin(projectile_angle), cannon_power*sin(vertical_angle));
		
		Vector3f initial_velocity(x_velocity, y_velocity, z_velocity);

		proj->set_velocity(initial_velocity);
		return proj;
		
	}
	void Cannon::switch_rotation_angle()
	{
		static float alternate_rotation_angle = rotation_angle * 5;
		std::swap(rotation_angle, alternate_rotation_angle);
		
	}

	void Cannon::switch_weapon()
	{
		weapon = (weapon_type)((int(weapon) + 1) % WEAPON_SIZE);

	}

    Model * Cannon::m_model = 0;
    unsigned long Cannon::m_instance_count = 0lu;
}
