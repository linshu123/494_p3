#include <zenilib.h>

#include "Crate.h"

using namespace Zeni;
using namespace Zeni::Collision;

namespace Crate {
    

    
    Crate::Crate(const Point3f &corner_,
                 const Vector3f &scale_,
                 const Quaternion &rotation_)
    : m_source(new Sound_Source(get_Sounds()["collide"])),
    m_corner(corner_),
    m_scale(scale_),
    m_rotation(rotation_),
	alive(true),
    shot(false),
    health(1000)
    {
        if(!m_instance_count)
            m_model = new Model("models/tank_body.3ds");
        ++m_instance_count;

		Point3f cannon_location = m_corner;
		cannon_location.z += 15.0f;
		m_scale *= 0.9f;
		cannon = Cannon(cannon_location, m_scale, m_rotation);

		cannon_rotation = m_rotation;
		cannon_power = 100.0f;
		rotation_angle = 3.14/180.0f;
		horizontal_angle = 0.0f;
		vertical_angle = 0;
        
        create_body();
    }
 
    void Crate::render() {
        const std::pair<Vector3f, float> rotation = m_rotation.get_rotation();
        
        m_model->set_translate(m_corner);
        m_model->set_scale(m_scale);
        m_model->set_rotate(rotation.second, rotation.first);
        
        m_model->render();
    }
    
    void Crate::collide() {
        if(!m_source->is_playing())
            m_source->play();
    }
    
    void Crate::create_body() {
        m_body = Parallelepiped(m_corner,
                                m_rotation * m_scale.get_i(),
                                m_rotation * m_scale.get_j(),
                                m_rotation * m_scale.get_k());
        
        m_source->set_position(m_corner + m_rotation * m_scale / 2.0f);
    }
    
    void Crate::move_forward(){
		
		float angle = m_rotation.get_rotation().second;

		Vector3f vec = cannon_rotation.get_rotation().first;
		if (vec.k > 0){
			angle *= -1;
		}

		float dist_x = 3.0f * cos(angle);
		float dist_y = 3.0f * sin(angle);
		        
		m_corner.x -= dist_x;
		m_corner.y += dist_y;
		
		cannon.change_loc(m_corner);

		//update_camera();
		
    }

	void Crate::update_camera(){
//		Point3f cam_position(m_corner);
//		cam_position.x -= 100.0f;
		//cam_position.y -= 10.0f;
//		cam_position.z = 30.0f;

//		m_view.adjust_pitch(-15.0f);

//		m_view.set_position(cam_position);
        Point3f cam_position = cannon.get_cannon_tip(15.0f);
        cam_position.z += 5.0f;
        m_view.set_position(cam_position);
        m_view.look_at(cannon.get_cannon_tip());

	}

    void Crate::move_back(){

		float angle = m_rotation.get_rotation().second;

		Vector3f vec = cannon_rotation.get_rotation().first;
		if (vec.k > 0){
			angle *= -1;
		}

		float dist_x = 3.0f * cos(angle);
		float dist_y = 3.0f * sin(angle);
		        
		m_corner.x += dist_x;
		m_corner.y -= dist_y;

		
		cannon.change_loc(m_corner);
		
		//update_camera();
    }
    
    void Crate::turn_left(){
        m_rotation = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0, 0, 1), rotation_angle) * m_rotation;
        cannon_rotation = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0, 0, 1), rotation_angle) * cannon_rotation;

		horizontal_angle += rotation_angle;

		cannon.rotate_left();

		//m_view.adjust_pitch
        
    }
    
    void Crate::turn_right(){
        m_rotation = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0, 0, 1), -rotation_angle) * m_rotation;
        cannon_rotation = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0, 0, 1), -rotation_angle) * cannon_rotation;

		horizontal_angle -= rotation_angle;
		cannon.rotate_right();
    }

	Projectile * Crate::fire(float power, Crate *player){
        shot = true;
		return cannon.fire(power, player);
	}
	
	void Crate::switch_rotation_angle()
	{
		static float alternate_rotation_angle = rotation_angle * 5;
		std::swap(rotation_angle, alternate_rotation_angle);
		cannon.switch_rotation_angle();
	}

    void Crate::reset_shot(){
        shot = false;
    }
    
    void Crate::decrease_health(int damage){
        health -= damage;
        if (health < 0){
            die();
            health = 0;
        }

    }
    
    Model * Crate::m_model = 0;
    unsigned long Crate::m_instance_count = 0lu;
}
