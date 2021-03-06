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
    m_rotation(rotation_)
  {
    if(!m_instance_count)
      m_model = new Model("models/tank.3ds");
    ++m_instance_count;

	m_velocity = Vector3f(0.0f, 0.0f, 0.0f);

    create_body();
  }

  Crate::Crate(const Crate &rhs)
    : m_source(new Sound_Source(get_Sounds()["collide"])),
    m_corner(rhs.m_corner),
    m_scale(rhs.m_scale),
    m_rotation(rhs.m_rotation)
  {
    ++m_instance_count;

    create_body();
  }

  Crate & Crate::operator=(const Crate &rhs) {
    m_corner = rhs.m_corner;
    m_scale = rhs.m_scale;
    m_rotation = rhs.m_rotation;

    create_body();

    return *this;
  }

  Crate::~Crate() {
    delete m_source;

    if(!--m_instance_count) {
      delete m_model;
      m_model = 0lu;
    }
  }

  void Crate::render() {
    const std::pair<Vector3f, float> rotation = m_rotation.get_rotation();

    m_model->set_translate(m_corner);
    m_model->set_scale(m_scale);
    m_model->set_rotate(rotation.second, rotation.first);

    m_model->render();
  }

  void Crate::applyGravity(float time_step){

	  if (m_corner.z <= 0){
		  m_velocity.z = 0;
		  m_corner.z = 0;
		  return;
	  }


	  //m_velocity.z -= (time_step * 5);
	  m_velocity.z -= ((75 * 9.8 * 1)/ (m_corner.z)) * time_step;
	  m_corner += m_velocity;

	  if (m_corner.z <= 0){
		  m_velocity.z = -0.5f * m_velocity.z;
		  m_corner.z = 0;
		  return;
	  }
	   

  }

  void Crate::rise(){

	  m_corner.z = 550;
	  m_velocity = Vector3f(0.0f, 0.0f, 0.0f);
	  //applyGravity();

  }

  void Crate::launch(){
	  m_corner.z = 100.0f;
	  m_velocity.z = 4.0f;
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

  Model * Crate::m_model = 0;
  unsigned long Crate::m_instance_count = 0lu;

}
