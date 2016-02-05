        #ifndef SIMULATION_H
        #define SIMULATION_H

        #include <rfftw.h>              //the numerical simulation FFTW library


        class Simulation {


        public:
            //Default Constructor
            Simulation();

            //Overload Constructor (to call Simulation with different default values)
            Simulation(double,float,int);

            //Destructor (destroy out of memory after being done)
            ~Simulation();

            //Accessor Functions (to get and set member variables)
            int get_frozen() const;
            double get_dt() const;				//simulation time step
            float get_visc() const;				//fluid viscosity
            fftw_real* get_rho() const;
            fftw_real* get_rho0() const;
            fftw_real* get_vx() const;
            fftw_real* get_vy() const;
            fftw_real* get_vx0() const;
            fftw_real* get_vy0() const;
            fftw_real* get_fy() const;
            fftw_real* get_fx() const;

            //Mutator functions
            void set_frozen(int);
            void set_visc(float);
            void set_dt(double);
            void set_vx(fftw_real*);
            void set_vy(fftw_real*);
            void set_vx0(fftw_real*);
            void set_vy0(fftw_real*);
            void set_fx(fftw_real*);
            void set_fy(fftw_real*);
            void set_rho(fftw_real*);
            void set_rho0(fftw_real*);
            void set_plan_rc(rfftwnd_plan);
            void set_plan_cr(rfftwnd_plan new_plan_cr);

            static const int DIM = 100;	//size of simulation grid (const, so already initialized) <- should probbly not be in Simulation class


            //init_simulation: Initialize simulation data structures as a function of the grid size 'n'.
            //                 Although the simulation takes place on a 2D grid, we allocate all data structures as 1D arrays,
            //                 for compatibility with the FFTW numerical library.
            // Note to self: in header only give the type of the variable, not the name
            void init_simulation(int);

            //set_forces: copy user-controlled forces to the force vectors that are sent to the solver.
            //            Also dampen forces and matter density to get a stable simulation.
            void set_forces(void);

            // diffuse_matter: This function diffuses matter that has been placed in the velocity field. It's almost identical to the
            // velocity diffusion step in the function above. The input matter densities are in rho0 and the result is written into rho.
            void diffuse_matter(int, fftw_real*, fftw_real*, fftw_real*, fftw_real*, fftw_real);

            //solve: Solve (compute) one step of the fluid flow simulation
            void solve(int, fftw_real*, fftw_real*, fftw_real*, fftw_real*, fftw_real,fftw_real);

            int clamp(float);

        private:
            // member variables
            double dt;				//simulation time step
            float visc;				//fluid viscosity
            fftw_real *vx, *vy;             //(vx,vy)   = velocity field at the current moment
            fftw_real *vx0, *vy0;           //(vx0,vy0) = velocity field at the previous moment
            fftw_real *fx, *fy;	            //(fx,fy)   = user-controlled simulation forces, steered with the mouse
            fftw_real *rho, *rho0;			//smoke density at the current (rho) and previous (rho0) moment
            rfftwnd_plan plan_rc, plan_cr;  //simulation domain discretization
            int frozen;               //toggles on/off the animation

            //FFT: Execute the Fast Fourier Transform on the dataset 'vx'.
            //     'dirfection' indicates if we do the direct (1) or inverse (-1) Fourier Transform
            void FFT(int,void*);

        };

        #endif // SIMULATION_H