#include <zenilib.h>

#include "Crate_State.h"
#include <sstream>
#include <iomanip>
using namespace std;
using namespace Zeni;
using namespace Zeni::Collision;

const string vertical_angle_str = "vertical_angle";
const string horizontal_angle_str = "horizontal_angle";
const string power_str = "power";
const string movement_str = "movement";
const string weapon_str = "weapon";
const string damage_str = "Last damage: ";


namespace Crate {
    
	Player m_view(Zeni::Camera(Point3f(0.0f, 0.0f, 50.0f),
                    Quaternion(),
                    1.0f, 10000.0f),
             Vector3f(0.0f, 0.0f, -39.0f),
             11.0f);


    Crate_State::Crate_State()
    : 
    m_forward(false),
    m_backward(false),
    m_rotate_left(false),
    m_rotate_right(false),
	m_fire(false),
	m_switch(false),
	curr_player(0),
	num_players(2),
	cannon_down(false),
	cannon_left(false),
	cannon_right(false),
	cannon_up(false),
	increase_power(false),
	move_release(false),
	switch_rotation(false),
	switch_weapon(false),
	camera_reset(false),
	game_size(Point3f(4096.0f, 4096.0f, 2048.0f)),
    last_damage(0)
//    current_bullet(nullptr)
    {
		
		text_map[power_str] = new Text_Box(Point2f(0, 0), Point2f(200, 20), "system_36_800x600", "Power:", Color());
		text_map[vertical_angle_str] = new Text_Box(Point2f(0, 20), Point2f(200, 40), "system_36_800x600", "V angle: ", Color());
		text_map[horizontal_angle_str] = new Text_Box(Point2f(0, 40), Point2f(200, 60), "system_36_800x600", "H angle: ", Color());
		text_map[movement_str] = new Text_Box(Point2f(0, 60), Point2f(300, 80), "system_36_800x600", "Movement left: ", Color());
		text_map[weapon_str] = new Text_Box(Point2f(0, 80), Point2f(300, 100), "system_36_800x600", "Weapon: Sprinkle", Color());
		text_map[damage_str] = new Text_Box(Point2f(0, 100), Point2f(300, 120), "system_36_800x600", damage_str.c_str(), Color());

		for (auto &pair : text_map){
			pair.second->give_BG_Renderer(new Widget_Renderer_Color(Color(0,0,0,0)));
		}


		//auto colors = get_Colors();
        
		for (int i = 0 ; i < num_players + 1; ++i){
			players.push_back(Crate(Point3f(0.0f + 1000 * i, 0.0f + 1000 * i, 0.0f),
              Vector3f(30.0f, 30.0f, 30.0f)));
		}
        
        players.erase(players.begin());
        
        for (int i = 0; i < num_players; ++i) {
            stringstream text;
            text << "Player " << i << " health: 1000";
            health_map[i] = new Text_Box(Point2f(0, 500 + 30 * i), Point2f(300, 500 + 30 * (i + 1)), "system_36_800x600", text.str().c_str(), Color());
            health_map[i]->give_BG_Renderer(new Widget_Renderer_Color(Color(0,0,0,0)));
        }

        health_map[curr_player]->set_text_color(Color(0xFFDC143C));
    
		// "Sprinkle", "M & M", "Chocolate chip", "Candle"
		weapon_list.push_back("Sprinkle");
		weapon_list.push_back("M & M");
		weapon_list.push_back("Chocolate chip");
		weapon_list.push_back("Candle");

		
		players[curr_player].update_camera();
        set_pausable(true);
    }
    
    void Crate_State::on_push() {
        get_Window().set_mouse_state(Window::MOUSE_RELATIVE);
    }
    
    void Crate_State::on_key(const SDL_KeyboardEvent &event) {
        switch(event.keysym.sym) {
            case SDLK_UP:
                m_forward = event.type == SDL_KEYDOWN;
				move_release = event.type == SDL_KEYUP;
                break;
            case SDLK_DOWN:
                m_backward = event.type == SDL_KEYDOWN;
				move_release = event.type == SDL_KEYUP;
                break;
                
            case SDLK_LEFT:
                m_rotate_left = event.type == SDL_KEYDOWN;
                break;
                
            case SDLK_RIGHT:
                m_rotate_right = event.type == SDL_KEYDOWN;
                break;
                
			case SDLK_RETURN:
				break;
                
			case SDLK_t:
				m_switch = event.type == SDL_KEYDOWN;
				break;

			case SDLK_i:
				cannon_up = event.type == SDL_KEYDOWN;
				break;

			case SDLK_k:
				cannon_down = event.type == SDL_KEYDOWN;
				break;
				
            case SDLK_j:
				cannon_left = event.type == SDL_KEYDOWN;
				break;

			case SDLK_l:
				cannon_right = event.type == SDL_KEYDOWN;
				break;
            case SDLK_w:
//                m_controls.forward = event.type == SDL_KEYDOWN;
                break;
                
            case SDLK_a:
//                m_controls.left = event.type == SDL_KEYDOWN;
                break;
                
            case SDLK_s:
//                m_controls.back = event.type == SDL_KEYDOWN;
                break;
                
            case SDLK_d:
//                m_controls.right = event.type == SDL_KEYDOWN;
                break;
                
			case SDLK_o:
				switch_weapon = event.type == SDL_KEYDOWN;
				break;

			case SDLK_r:
				camera_reset = event.type == SDL_KEYDOWN;
				break;

            case SDLK_SPACE:
				increase_power = event.type == SDL_KEYDOWN;
				m_fire = event.type == SDL_KEYUP;
                break;

            case SDLK_LSHIFT:
				switch_rotation = (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP);
                break;    

			case SDLK_RSHIFT:
				switch_rotation = (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP);
                break;    

            default:
                Gamestate_Base::on_key(event);
                break;
        }
    }
    
    void Crate_State::on_mouse_motion(const SDL_MouseMotionEvent &event) {
        m_view.adjust_pitch(event.yrel / 500.0f);
        m_view.turn_left_xy(-event.xrel / 500.0f);
    }
    
    void Crate_State::perform_logic() {
        const Time_HQ current_time = get_Timer_HQ().get_time();
        float processing_time = float(current_time.get_seconds_since(time_passed));
        time_passed = current_time;

		const float total_movement = 5;
		

		if (switch_weapon){
			players[curr_player].cannon.switch_weapon();
			stringstream weapon_text;
			weapon_text << "Weapon: " << weapon_list[players[curr_player].cannon.get_weapon()];
			text_map[weapon_str]->set_text(weapon_text.str().c_str());
			switch_weapon = false;
		}

		if (camera_reset){
			players[curr_player].update_camera();
			camera_reset = false;
		}
		       
		if (switch_rotation){
			players[curr_player].switch_rotation_angle();
			switch_rotation = false;
		}

		if (increase_power){
  			shot_power.start();
			increase_power = false;
		}
		
		float pow = shot_power.seconds() * 70;
		if (pow != 0.0f)
		{
//            if (pow > 500){
//                pow = 500;
//            }
            pow = std::min(pow, 500.0f);
			stringstream display_text;
            display_text << std::setprecision(3);
			display_text << "Power: ";
			display_text << (pow/5) << endl;
			text_map[power_str]->set_text(display_text.str().c_str());
		}
		if (m_fire)
		{
			
			shot_power.stop();
			shot_power.reset();
//			bullets.push_back(players[curr_player].fire(pow));
//            if (current_bullet)
//                delete current_bullet;
            bullets.push_back(players[curr_player].fire(pow, &players[curr_player]));
			m_fire = false;
		}

		stringstream vertical_angle_text;
		vertical_angle_text << std::setprecision(5);
		vertical_angle_text << "V angle: " << -1 * int(players[curr_player].cannon.get_vertical_angle() / 3.14 * 180) % 360;
		text_map[vertical_angle_str]->set_text(vertical_angle_text.str().c_str());

		stringstream horizontal_angle_text;
		horizontal_angle_text << std::setprecision(5);
		horizontal_angle_text << "V angle: " << int(players[curr_player].cannon.get_horizontal_angle() / 3.14 * 180) % 360 ;
		text_map[horizontal_angle_str]->set_text(horizontal_angle_text.str().c_str());

		stringstream movement_text;
		movement_text << std::setprecision(5);
		movement_text << "Movement left: ";
		if (total_movement - movement_timer.seconds() < 0)
			movement_text << 0;
		else
			movement_text << (total_movement - movement_timer.seconds());
		text_map[movement_str]->set_text(movement_text.str().c_str());

		if (move_release || movement_timer.seconds() >= total_movement){
			
			movement_timer.stop();
			move_release = false;
		}

		Point3f previous_location = players[curr_player].get_corner();

		if (m_switch){
            health_map[curr_player]->set_text_color(Color(0xFFFFFFFF));
            
			curr_player++;
			curr_player %= num_players;
			while (!players[curr_player].is_alive()){
				curr_player++;
				curr_player %= num_players;
			}
            health_map[curr_player]->set_text_color(Color(0xFFDC143C));
            
            players[curr_player].reset_shot();
			players[curr_player].update_camera();
			movement_timer.reset();
			m_switch = false;
			
		}
        
        if (m_forward && movement_timer.seconds() <= total_movement){
			if (!movement_timer.is_running())
				movement_timer.start();
            players[curr_player].move_forward();
            m_forward = false;
        }
        
        
        if (m_backward && movement_timer.seconds() <= total_movement){
			if (!movement_timer.is_running())
				movement_timer.start();
            players[curr_player].move_back();
            m_backward = false;
        }

        if (m_rotate_left){
            players[curr_player].turn_left();
            m_rotate_left = false;
        }
        
        
        if (m_rotate_right){
            players[curr_player].turn_right();
            m_rotate_right = false;
        }

		if (cannon_down){
			players[curr_player].cannon.rotate_down();
			cannon_down = false;
		}
		if (cannon_up){
			players[curr_player].cannon.rotate_up();
			cannon_up = false;
		}
		if (cannon_left){
			players[curr_player].cannon.rotate_left();
			cannon_left = false;
		}
		if (cannon_right){
			players[curr_player].cannon.rotate_right();
			cannon_right = false;
		}
		/*
		if (m_fire)
		{
			bullets.push_back(players[curr_player].fire());
			
			m_fire = false;
		}
		*/
		/*Collision Detection*/
		vector<pair<Sphere, Crate *> > collision_spheres;
		for (auto &player : players){
			collision_spheres.push_back(make_pair(Sphere(player.get_corner(), 18.5), &player));
		}
		vector<pair<Sphere, Projectile *> > projectile_spheres;
		for (auto bullet : bullets){
			projectile_spheres.push_back(make_pair(Sphere(bullet->get_corner(), bullet->get_collision_radius()), bullet));
		}

		//check projectile collisions
		for (auto &bullet : projectile_spheres){
			for (auto iter = collision_spheres.begin(); iter != collision_spheres.end(); ++iter){
				if (iter->first.intersects(bullet.first)){
//					sphere.second->die();
                    bullet.second->set_velocity(Vector3f(0, 0, 0));
                    iter->second->decrease_health(bullet.second->get_damage());
                    bullet.second->add_damage_done(bullet.second->get_damage());
					bullet.second->detonate();
                    stringstream last_damage_text;
                    last_damage_text << damage_str << bullet.second->get_damage_done();
                    text_map[damage_str]->set_text(last_damage_text.str().c_str());
                    
                    int player_ind = int(iter - collision_spheres.begin());
                    stringstream health_text;
                    health_text << "Player " << player_ind << " health: " << iter->second->get_health();
                    health_map[player_ind]->set_text(health_text.str().c_str());
				}
			}
		}

		//check tank collisions
		for (unsigned int i = 0; i < players.size(); ++i){
			if (i != curr_player){
				if (collision_spheres[i].first.intersects(collision_spheres[curr_player].first)){
					players[curr_player].set_corner(previous_location);
					break;
				}
			}
		}

		/*End Collision Detection*/
        
        /** Get forward and left vectors in the XY-plane **/
        const Vector3f forward = m_view.get_camera().get_forward().get_ij().normalized();
        const Vector3f left = m_view.get_camera().get_left().get_ij().normalized();
        
        /** Get velocity vector split into a number of axes **/
       
        const Vector3f velocity = (m_controls.forward - m_controls.back) * 50.0f * forward
        + (m_controls.left - m_controls.right) * 50.0f * left;

        const Vector3f x_vel = velocity.get_i();
        const Vector3f y_vel = velocity.get_j();
        Vector3f z_vel = m_view.get_velocity().get_k();
        
        /** Bookkeeping for sound effects **/
        if(velocity.magnitude() != 0.0f)
            m_moved = true;
        
        /** Keep delays under control (if the program hangs for some time, we don't want to lose responsiveness) **/
        if(processing_time > 0.1f)
            processing_time = 0.1f;
        
        /** Physics processing loop**/
        for(float time_step = 0.05f;
            processing_time > 0.0f;
            processing_time -= time_step)
        {
            if(time_step > processing_time)
                time_step = processing_time;
            
            /** Gravity has its effect **/
            z_vel -= Vector3f(0.0f, 0.0f, 50.0f * time_step);
            

			/** Projectile Flying **/
			for(std::list<Projectile *>::const_iterator it = bullets.begin(); it != bullets.end(); ){
				(*it)->apply_gravity(time_step);

				//Remove stopped bullets
				if ((*it)->finished_detonating())
				{
                    m_switch = true;
					it = bullets.erase(it);
				}else
				{
					++it;
				}
			}

            /** Try to move on each axis **/
            partial_step(time_step, x_vel);
            partial_step(time_step, y_vel);
            partial_step(time_step, z_vel);
            
            /** Keep player above ground; Bookkeeping for jumping controls **/
//            const Point3f &position = m_view.get_camera().position;
//            if(position.z < 50.0f) {
//                m_view.set_position(Point3f(position.x, position.y, 50.0f));
//                m_view.set_on_ground(true);
//            }
        }
        if (!players[curr_player].has_shot())
            players[curr_player].update_camera();
        else
            update_bullet_camera();

    }
    
    void Crate_State::render() {
        Video &vr = get_Video();
        
        vr.set_3d(m_view.get_camera());
        
		for (auto &player : players){
			if (player.is_alive()){
				player.render();
				player.cannon.render();
			}
		}
        // players[curr_player].render();

		for(std::list<Projectile *>::const_iterator it = bullets.begin(); it != bullets.end(); ++it){
			(*it)->render();
		}


		float x = -2048.0f;
		float y = -2048.0f;
		float h = 4096.0f;
		float w = 4096.0f;

		Vertex2f_Texture p0(Point2f(x,y), Point2f(0.0f, 0.0f));  //Bottom Left
		Vertex2f_Texture p1(Point2f(x, y+h), Point2f(0.0f, 1.0f));  //Top Left
		Vertex2f_Texture p2(Point2f(x+w, y+h), Point2f(1.0f, 1.0f)); //top Right
		Vertex2f_Texture p3(Point2f(x+w, y), Point2f(1.0f, 0.0f)); //Bottom Right
		Material material("crate");

		Quadrilateral<Vertex2f_Texture> quad(p0, p1, p2, p3);
		quad.fax_Material(&material);


		material = Material("sky_blue");
		//Sky Ahead
		Point3f low_left(game_size.x/2, -game_size.y/2, 0.0f);
		Point3f low_right(game_size.x/2, game_size.y/2, 0.0f);
		Point3f top_left(game_size.x/2, -game_size.y/2, game_size.z);
		Point3f top_right(game_size.x/2, game_size.y/2, game_size.z);

		Vertex3f_Texture p5(low_left, Point2f(0.0f, 0.0f));  //Bottom Left
		Vertex3f_Texture p6(top_left, Point2f(0.0f, 1.0f));  //Top Left
		Vertex3f_Texture p7(top_right, Point2f(1.0f, 1.0f)); //top Right
		Vertex3f_Texture p8(low_right, Point2f(1.0f, 0.0f)); //Bottom Right
		

		Quadrilateral<Vertex3f_Texture> sky_pos_x(p5, p6, p7, p8);
		sky_pos_x.fax_Material(&material);

		//Sky Behind
		low_left = Point3f(-game_size.x/2, -game_size.y/2, 0.0f);
		low_right = Point3f(-game_size.x/2, game_size.y/2, 0.0f);
		top_left = Point3f(-game_size.x/2, -game_size.y/2, game_size.z);
		top_right = Point3f(-game_size.x/2, game_size.y/2, game_size.z);

		p5 = Vertex3f_Texture(low_left, Point2f(0.0f, 0.0f));  //Bottom Left
		p6 = Vertex3f_Texture(top_left, Point2f(0.0f, 1.0f));  //Top Left
		p7 = Vertex3f_Texture(top_right, Point2f(1.0f, 1.0f)); //top Right
		p8 = Vertex3f_Texture(low_right, Point2f(1.0f, 0.0f)); //Bottom Right
		

		Quadrilateral<Vertex3f_Texture> sky_neg_x(p5, p6, p7, p8);
		sky_neg_x.fax_Material(&material);

		//Sky Above
		material = Material("sky_blue");

		low_left = Point3f(-game_size.x/2, -game_size.y/2, game_size.z);
		low_right = Point3f(-game_size.x/2, game_size.y/2, game_size.z);
		top_left = Point3f(game_size.x/2, -game_size.y/2, game_size.z);
		top_right = Point3f(game_size.x/2, game_size.y/2, game_size.z);

		p5 = Vertex3f_Texture(low_left, Point2f(0.0f, 0.0f));  //Bottom Left
		p6 = Vertex3f_Texture(top_left, Point2f(0.0f, 1.0f));  //Top Left
		p7 = Vertex3f_Texture(top_right, Point2f(1.0f, 1.0f)); //top Right
		p8 = Vertex3f_Texture(low_right, Point2f(1.0f, 0.0f)); //Bottom Right
		

		Quadrilateral<Vertex3f_Texture> sky_above(p5, p6, p7, p8);
		sky_above.fax_Material(&material);

		//Sky Right
		material = Material("sky_blue");
		low_left = Point3f(-game_size.x/2, -game_size.y/2, 0);
		low_right = Point3f(game_size.x/2, -game_size.y/2, 0);
		top_left = Point3f(-game_size.x/2, -game_size.y/2, game_size.z);
		top_right = Point3f(game_size.x/2, -game_size.y/2, game_size.z);

		p5 = Vertex3f_Texture(low_left, Point2f(0.0f, 0.0f));  //Bottom Left
		p6 = Vertex3f_Texture(top_left, Point2f(0.0f, 1.0f));  //Top Left
		p7 = Vertex3f_Texture(top_right, Point2f(1.0f, 1.0f)); //top Right
		p8 = Vertex3f_Texture(low_right, Point2f(1.0f, 0.0f)); //Bottom Right
		

		Quadrilateral<Vertex3f_Texture> sky_neg_y(p5, p6, p7, p8);
		sky_neg_y.fax_Material(&material);

		//Sky Left
		low_left = Point3f(-game_size.x/2, game_size.y/2, 0);
		low_right = Point3f(game_size.x/2, game_size.y/2, 0);
		top_left = Point3f(-game_size.x/2, game_size.y/2, game_size.z);
		top_right = Point3f(game_size.x/2, game_size.y/2, game_size.z);

		p5 = Vertex3f_Texture(low_left, Point2f(0.0f, 0.0f));  //Bottom Left
		p6 = Vertex3f_Texture(top_left, Point2f(0.0f, 1.0f));  //Top Left
		p7 = Vertex3f_Texture(top_right, Point2f(1.0f, 1.0f)); //top Right
		p8 = Vertex3f_Texture(low_right, Point2f(1.0f, 0.0f)); //Bottom Right
		

		Quadrilateral<Vertex3f_Texture> sky_pos_y(p5, p6, p7, p8);
		sky_pos_y.fax_Material(&material);

		vr.render(sky_pos_y);
		vr.render(sky_neg_y);
		vr.render(sky_above);
		vr.render(sky_pos_x);
		vr.render(sky_neg_x);
		vr.render(quad);
			
		vr.set_2d();
		text_map[power_str]->render();
		for (auto &pair : text_map){
			pair.second->render();
		}
		for (auto &pair : health_map){
			pair.second->render();
		}
        
    }
    
    void Crate_State::update_bullet_camera(){
        auto camera_loc = bullets.back()->get_location();
        auto velocity = bullets.back()->get_velocity();
        
        camera_loc -= velocity.normalize() * 10;
//        static Vector3f previous_vel = velocity;
//        if (bullets.back()->stopped())
//            return;
        
        
//        m_view.set_position(camera_loc);
//        m_view.look_at(bullets.back()->get_location());
        
        auto new_position = bullets.back()->get_location() - (bullets.back()->get_location() - players[curr_player].get_corner()).normalize() * 50 + Vector3f(0, 0, 50);
        m_view.set_position(new_position);
        m_view.look_at(bullets.back()->get_location());        
        
//        m_view.adjust_pitch(previous_vel.angle_between(velocity));
//        previous_vel = velocity;
        
    }
    
    void Crate_State::partial_step(const float &time_step, const Vector3f &velocity) {
//        m_view.set_velocity(velocity);
//        const Point3f backup_position = m_view.get_camera().position;
//        
//        m_view.step(time_step);
//        
//        /** If collision with the crate has occurred, roll things back **/
//        if(players[curr_player].get_body().intersects(m_view.get_body())) {
//            if(m_moved)
//            {
//                /** Play a sound if possible **/
//                players[curr_player].collide();
//                m_moved = false;
//            }
//            
//            m_view.set_position(backup_position);
//            
//            /** Bookkeeping for jumping controls **/
//            if(velocity.k < 0.0f)
//                m_view.set_on_ground(true);
//        }
    }
    
}
