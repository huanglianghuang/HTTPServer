
#ifndef TEST2_TW_TIMER_H
#define TEST2_TW_TIMER_H

class client_data ;
class tw_timer {
    public:
        tw_timer(int rot,int ts);

    public:
        int rotation;
        int time_slot;
        void(*cb_func)(client_data*);
        client_data*user_data;
        tw_timer*next;
        tw_timer*prev;
};



#endif //TEST2_TW_TIMER_H


