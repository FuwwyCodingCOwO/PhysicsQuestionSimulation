#include <cstdio>
#include <iostream>
#include <thread>


struct vector3{
    double x, y, z;
};


struct ball_parameters{
    struct vector3 pos, veloc;
    double mass, density, gravity, area, tick;
};


class ball{
    private:
        struct vector3 pos, veloc, acc;
        double mass, density, gravity, area, tick;
        bool run = false;
        int tick_count = 0;

        void update_position(void){
            this->pos.x += this->tick * this->veloc.x;
            this->pos.y += this->tick * this->veloc.y;
            this->pos.z += this->tick * this->veloc.z;
        }

        void update_velocity(void){
            this->veloc.x += this->tick * this->acc.x;
            this->veloc.y += this->tick * this->acc.y;
            this->veloc.z += this->tick * this->acc.z;
        }

        void update_accelleration(void){
            double drag_x, drag_y, drag_z;
            drag_x = this->density * ((this->veloc.x * this->veloc.x) / 2) * this->area;
            drag_y = this->density * ((this->veloc.y * this->veloc.y) / 2) * this->area;
            drag_z = this->density * ((this->veloc.z * this->veloc.z) / 2) * this->area;
            this->acc.x = drag_x / this->mass;
            this->acc.y = (drag_y / this->mass) + gravity;
            this->acc.z = drag_z / this->mass;
        }

        void simulation_instance(void){
            while(run){
                this->update_accelleration();
                this->update_velocity();
                this->update_position();
                this->tick_count += 1;
            }
        }

    public:
        ball(struct ball_parameters bp){
            this->pos = bp.pos,
            this->veloc = bp.veloc,
            this->acc = {0, bp.gravity*-1, 0},
            this->mass = bp.mass,
            this->density = bp.density,
            this->gravity = bp.gravity * -1.0,
            this->area = bp.area,
            this->tick = bp.tick;
        }

        ball(struct vector3 pos, struct vector3 veloc, double mass, double density, double gravity, double area, double tick){
            if(density >= 0 && gravity >= 0 && tick > 0 && mass > 0){
                this->pos = pos,
                this->veloc = veloc,
                this->acc = {0, gravity*-1, 0},
                this->mass = mass,
                this->density = density,
                this->gravity = gravity * -1.0,
                this->area = area, this->tick = tick;
            }
            else{
                throw std::runtime_error("fatal values were provided, aborting execution");
            }
        }

        vector3 get_pos(void){
            return this->pos;
        }

        vector3 get_veloc(void){
            return this->veloc;
        }

        double get_tick_count(void){
            return this->tick_count;
        }

        double get_tick(void){
            return this->tick;
        }

        void start_simulation(){
            this->run = true;
            std::thread t([this] { this->simulation_instance(); });
            t.detach();
        }

        void end_simulation(){
            this->run = false;
        };
};


void trial(struct ball_parameters &BP, class ball &B){
    double NA_F_T;
    std::printf(
        "starting position = (%.2lf m, %.2lf m, %.2lf m), starting velocity = (%.2lf m/s, %.2lf m/s, %.2lf m/s), mass of object = %.2lf kg, density of atmosphere = %.2lf kg/m^3, gravity = %.2lf ms/2^s, cross section = %.2lf m^2, tick = %lf s\n",
        BP.pos.x,
        BP.pos.y,
        BP.pos.z,
        BP.veloc.x,
        BP.veloc.y,
        BP.veloc.z,
        BP.mass,
        BP.density,
        BP.gravity,
        BP.area,
        BP.tick
    );
    B.start_simulation();
    while(true){
        if (B.get_pos().y < 0){
            B.end_simulation();
            break;
        }
    }
    NA_F_T = B.get_tick_count() * B.get_tick();
    std::printf(
        "object took, %lf, seconds to fall, %.2lf, meters\n\n",
        NA_F_T,
        BP.pos.y
    );
}


int main(void){
    //non-class declarations
    struct vector3 starting_position, fixed, moving;
    struct ball_parameters NoAtmos_Fixed, NoAtmos_Moving, YesAtmos_Fixed, YesAtmos_Moving;
    double mass, area, gravity, yes_atmos, no_atmos, tick;

    //value assignment
    mass = 1;
    area = 1;
    gravity = 9.8;
    yes_atmos = 30;
    no_atmos = 0;
    tick = .000001;
    starting_position = {
        0,
        50,
        0
    };
    fixed = {
        0,
        0,
        0
    };
    moving = {
        1000,
        0,
        0
    };
    NoAtmos_Fixed = {
        starting_position,
        fixed,
        mass,
        no_atmos,
        gravity,
        area,
        tick
    };
    NoAtmos_Moving = {
        starting_position,
        moving,
        mass,
        no_atmos,
        gravity,
        area,
        tick
    };
    YesAtmos_Fixed = {
        starting_position,
        fixed,
        mass,
        yes_atmos,
        gravity,
        area,
        tick
    };
    YesAtmos_Moving = {
        starting_position,
        moving,
        mass,
        yes_atmos,
        gravity,
        area,
        tick
    };

    //delcartion of ball object
    ball NA_F(NoAtmos_Fixed), NA_M(NoAtmos_Moving), YA_F(YesAtmos_Fixed), YA_M(YesAtmos_Moving);

    //run trials
    trial(NoAtmos_Fixed, NA_F); //trial one
    trial(NoAtmos_Moving, NA_M); //trial two
    trial(YesAtmos_Fixed, YA_F); //trial three
    trial(YesAtmos_Moving, YA_M); //trial four
}
