#ifndef CRATE_STATE_H
#define CRATE_STATE_H

#include "Crate.h"
#include "Player.h"
#include "Projectile.h"
#include "Cannon.h"
#include <Zeni/Gamestate.h>
#include <Zeni/Timer.h>
#include <zenilib.h>
#include <string>
#include <list>

namespace Crate {
    
    class Crate_State : public Zeni::Gamestate_Base {
        struct Controls {
            Controls() : forward(false), left(false), back(false), right(false) {}
            
            bool forward;
            bool left;
            bool back;
            bool right;
        } m_controls;
        
    public:
        Crate_State();
        
        void on_push();
        
        void on_key(const SDL_KeyboardEvent &event);
        void on_mouse_motion(const SDL_MouseMotionEvent &event);
        
        void perform_logic();
        
        void render();
        
        void update_bullet_camera();
        
    private:
        void partial_step(const float &time_step, const Zeni::Vector3f &velocity);
        
        Zeni::Time_HQ time_passed;
        
//        Crate m_tank;
		std::vector<Crate> players;
		std::list<Projectile*> bullets;
		//std::list<int> tree;


        
        bool m_moved;
        bool m_forward;
        bool m_backward;
        bool m_rotate_left;
        bool m_rotate_right;
		bool m_fire;
		bool m_switch;
		int curr_player;
		int num_players;
		bool cannon_up;
		bool cannon_down;
		bool cannon_left;
		bool cannon_right;
		bool increase_power;
		bool move_release;
		bool switch_rotation;
		bool switch_weapon;
		bool camera_reset;
		Zeni::Point3f game_size;
		Zeni::Chronometer<Zeni::Time> shot_power;
		Zeni::Chronometer<Zeni::Time> movement_timer;
		std::vector<std::string> weapon_list;
		//Zeni::Text_Box display_power;
		std::map<std::string, Zeni::Text_Box *> text_map;
        std::map<int, Zeni::Text_Box *> health_map;
        int last_damage;
    };
    
}

#endif
