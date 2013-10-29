#include <zenilib.h>

#include "Projectile.h"
#include "Crate.h"

using namespace Zeni;
using namespace Zeni::Collision;
using namespace std;
namespace Crate {
    

    
    Projectile::Projectile(Crate * player_,
                           const Point3f &corner_,
                 const Vector3f &scale_,
                 const Quaternion &rotation_,
				 weapon_type weap)
    : m_source(new Sound_Source(get_Sounds()["collide"])),
    m_corner(corner_),
    m_scale(scale_),
    m_rotation(rotation_),
	weapon(weap),
	detonated(false),
	times_doubled(0),
	explosion_radius(0),
    player(player_),
    damage_done(0)
    {
		string model_path = "models/";

		switch(weapon){
			case SPRINKLE:
                damage = 5;
                explosion_radius = 100;
				model_path += "sprinkle.3ds";
			break;

			case MM:
                damage = 15;
                explosion_radius = 70;
				model_path += "m_and_m.3ds";
			break;

			case CHOCOLATE:
                damage = 25;
                explosion_radius = 50;
				model_path += "chocolate_chip.3ds";
			break;

			case CANDLE:
                damage = 50;
                explosion_radius = 20;
				model_path += "candle.3ds";
			break;

			default:
				break;

		}
        //if(!m_instance_count)
            m_model = new Model(model_path.c_str());
        ++m_instance_count;

		m_velocity = Vector3f(0.0f, 0.0f, 0.0f);
		
        
        create_body();
    }
    


    Projectile::Projectile(const Projectile &rhs)
    : m_source(new Sound_Source(get_Sounds()["collide"])),
    m_corner(rhs.m_corner),
    m_scale(rhs.m_scale),
    m_rotation(rhs.m_rotation)
    {
        ++m_instance_count;
        
        create_body();
    }
    
    Projectile & Projectile::operator=(const Projectile &rhs) {
        m_corner = rhs.m_corner;
        m_scale = rhs.m_scale;
        m_rotation = rhs.m_rotation;
        
        create_body();
        
        return *this;
    }
    
    Projectile::~Projectile() {
        delete m_source;
        
        if(!--m_instance_count) {
            delete m_model;
            m_model = 0lu;
        }
    }

	void Projectile::set_velocity(Vector3f vel)
	{
		m_velocity = vel;
	}

	void Projectile::apply_gravity(float time_step)
	{
		
		float grav_strength = 0.5f * (1 - (m_corner.z / 10000));
		Vector3f gravity(0.0f, 0.0f, -1.0f * grav_strength);
		m_velocity += gravity;
		m_corner += m_velocity * time_step;

		//Bounce
		if (m_corner.z < 0)
		{
			m_velocity = 0.5f * m_velocity;
			m_velocity.z = -1.0f * m_velocity.z;
		}
		if (m_corner.z < 0 && abs(m_velocity.z) <= 0.5f )
		{
			m_corner.z = 0;
			m_velocity.z = 0;
		}
		
		if (stopped())
			detonate();
				
	}

	bool Projectile::stopped()
	{
		if (abs(m_velocity.x) + abs(m_velocity.y) + abs(m_velocity.z) <= 0.5f)
		{
			if (!detonated)
				detonate();

			return true;
		}
		return false;

	}
    
    void Projectile::render() {
        const std::pair<Vector3f, float> rotation = m_rotation.get_rotation();
        
        m_model->set_translate(m_corner);
        m_model->set_scale(m_scale);
        m_model->set_rotate(rotation.second, rotation.first);
        
        m_model->render();
    }
    
    void Projectile::collide() {
        if(!m_source->is_playing())
            m_source->play();
    }
    
    void Projectile::create_body() {
        m_body = Parallelepiped(m_corner,
                                m_rotation * m_scale.get_i(),
                                m_rotation * m_scale.get_j(),
                                m_rotation * m_scale.get_k());
        
        m_source->set_position(m_corner + m_rotation * m_scale / 2.0f);
    }

	void Projectile::detonate(){
        
        
//        auto new_position = get_location() - (get_location() - player->get_corner()).normalize() * 50 + Vector3f(0, 0, 50);
//        m_view.set_position(new_position);
        
//        m_view.look_at(get_location());
        
		m_velocity = Vector3f(0,0,0);
		if (times_doubled == 0){
			delete m_model;
			m_model = new Model("models/explosion.3ds");
		}

		times_doubled++;
		m_scale *= 1.02;

		if (times_doubled > explosion_radius){
            player->reset_shot();
			detonated = true;
		}

	}
    
    float Projectile::get_collision_radius(){
        return m_scale.i / 2;
    }
    
    void Projectile::add_damage_done(int damage_){
        damage_done += damage_;
    }
    
    Model * Projectile::m_model = 0;
    unsigned long Projectile::m_instance_count = 0lu;
    
    
    
}
