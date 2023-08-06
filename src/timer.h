#ifndef TIMER_H
#define TIMER_H



class Timer
{
    public:
        Timer(int _max_ticks);
        ~Timer();

        void start();
        void reset();
        void set_to_new_value(int _new_max_ticks);
        void update();
        bool check();


    private:
        int ticks;
        int max_ticks;
        bool running;
};


#endif
