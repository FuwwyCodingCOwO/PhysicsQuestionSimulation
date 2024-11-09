#include <cstdio>
#include <iostream>
#include <thread>

struct vector3{
   double x, y, z;
};

struct ball_parameters{
    struct vector3 pos, veloc; double mass, density, gravity, area, tick;
};

class ball{
    public:
        struct vector3 pos, veloc, acc; double mass, density, gravity, area, tick; bool run = false; int tick_count = 0;
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
            double drag_x=this->density*((this->veloc.x*this->veloc.x)/2)*this->area;
            double drag_y=this->density*((this->veloc.y*this->veloc.y)/2)*this->area;
            double drag_z=this->density*((this->veloc.z*this->veloc.z)/2)*this->area;
            this->acc.x = drag_x / this->mass;
            this->acc.y = (drag_y / this->mass) + gravity;
            this->acc.z = drag_z / this->mass;
        }
        void simulation_instance(void){
            while(run){
                this->update_accelleration();
                this->update_velocity();
                this->update_position();
                ++this->tick_count;
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
                this->area = area,
                this->tick = tick;
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
        void start_simulation(){ this->run = true; std::thread t([this] { this->simulation_instance(); }); t.detach(); }
        void end_simulation(){ this->run = false; };
};

int main(void){
    struct vector3 starting_position = {0, 50, 0};
    struct vector3 fixed = {0, 0, 0}, moving = {1000, 0, 0};
    double mass = 1, area = 1, gravity = 9.8, yes_atmos = 30, no_atmos = 0, tick = .000001;

    struct ball_parameters NoAtmos_Fixed = {starting_position, fixed, mass, no_atmos, gravity, area, tick};
    struct ball_parameters NoAtmos_Moving = {starting_position, moving, mass, no_atmos, gravity, area, tick};
    struct ball_parameters YesAtmos_Fixed = {starting_position, fixed, mass, yes_atmos, gravity, area, tick};
    struct ball_parameters YesAtmos_Moving = {starting_position, moving, mass, yes_atmos, gravity, area, tick };

    ball NA_F(NoAtmos_Fixed);
    ball NA_M(NoAtmos_Moving);
    ball YA_F(YesAtmos_Fixed);
    ball YA_M(YesAtmos_Moving);

    //experiment one
    std::printf("starting position = (%.2lf m, %.2lf m, %.2lf m), starting velocity = (%.2lf m/s, %.2lf m/s, %.2lf m/s), mass of object = %.2lf kg, density of atmosphere = %.2lf kg/m^3, gravity = %.2lf ms/2^s, cross section = %.2lf m^2, tick = %lf s\n", NoAtmos_Fixed.pos.x, NoAtmos_Fixed.pos.y, NoAtmos_Fixed.pos.z, NoAtmos_Fixed.veloc.x, NoAtmos_Fixed.veloc.y, NoAtmos_Fixed.veloc.z, NoAtmos_Fixed.mass, NoAtmos_Fixed.density, NoAtmos_Fixed.gravity, NoAtmos_Fixed.area, NoAtmos_Fixed.tick);
    NA_F.start_simulation();
    while(true){
        if (NA_F.get_pos().y < 0){
            NA_F.end_simulation();
            break;
        }
    }
    double NA_F_T = NA_F.tick_count * NA_F.tick;
    std::printf("object took, %lf, seconds to fall, %.2lf, meters\n\n", NA_F_T, NoAtmos_Fixed.pos.y);

    //experiment two
    std::printf("starting position = (%.2lf m, %.2lf m, %.2lf m), starting velocity = (%.2lf m/s, %.2lf m/s, %.2lf m/s), mass of object = %.2lf kg, density of atmosphere = %.2lf kg/m^3, gravity = %.2lf ms/2^s, cross section = %.2lf m^2, tick = %lf s\n", NoAtmos_Moving.pos.x, NoAtmos_Moving.pos.y, NoAtmos_Moving.pos.z, NoAtmos_Moving.veloc.x, NoAtmos_Moving.veloc.y, NoAtmos_Moving.veloc.z, NoAtmos_Moving.mass, NoAtmos_Moving.density, NoAtmos_Moving.gravity, NoAtmos_Moving.area, NoAtmos_Moving.tick);
    NA_M.start_simulation();
    while(true){
        if (NA_M.get_pos().y < 0){
            NA_M.end_simulation();
            break;
        }
    }
    double NA_M_T = NA_M.tick_count * NA_M.tick;
    std::printf("object took, %lf, seconds to fall, %.2lf, meters\n\n", NA_M_T, NoAtmos_Moving.pos.y);

    //experiment three
    std::printf("starting position = (%.2lf m, %.2lf m, %.2lf m), starting velocity = (%.2lf m/s, %.2lf m/s, %.2lf m/s), mass of object = %.2lf kg, density of atmosphere = %.2lf kg/m^3, gravity = %.2lf ms/2^s, cross section = %.2lf m^2, tick = %lf s\n", YesAtmos_Fixed.pos.x, YesAtmos_Fixed.pos.y, YesAtmos_Fixed.pos.z, YesAtmos_Fixed.veloc.x, YesAtmos_Fixed.veloc.y, YesAtmos_Fixed.veloc.z, YesAtmos_Fixed.mass, YesAtmos_Fixed.density, YesAtmos_Fixed.gravity, YesAtmos_Fixed.area, YesAtmos_Fixed.tick);
    YA_F.start_simulation();
    while(true){
        if (YA_F.get_pos().y < 0){
            YA_F.end_simulation();
            break;
        }
    }
    double YA_F_T = YA_F.tick_count * YA_F.tick;
    std::printf("object took, %lf, seconds to fall, %.2lf, meters\n\n", YA_F_T, YesAtmos_Fixed.pos.y);

    //experiment four
    std::printf("starting position = (%.2lf m, %.2lf m, %.2lf m), starting velocity = (%.2lf m/s, %.2lf m/s, %.2lf m/s), mass of object = %.2lf kg, density of atmosphere = %.2lf kg/m^3, gravity = %.2lf ms/2^s, cross section = %.2lf m^2, tick = %lf s\n", YesAtmos_Moving.pos.x, YesAtmos_Moving.pos.y, YesAtmos_Moving.pos.z, YesAtmos_Moving.veloc.x, YesAtmos_Moving.veloc.y, YesAtmos_Moving.veloc.z, YesAtmos_Moving.mass, YesAtmos_Moving.density, YesAtmos_Moving.gravity, YesAtmos_Moving.area, YesAtmos_Moving.tick);
    YA_M.start_simulation();
    while(true){
        if (YA_M.get_pos().y < 0){
            YA_M.end_simulation();
            break;
        }
    }
    double YA_M_T = YA_M.tick_count * YA_M.tick;
    std::printf("object took, %lf, seconds to fall, %.2lf, meters\n\n", YA_M_T, YesAtmos_Moving.pos.y);

    return 0;
}
