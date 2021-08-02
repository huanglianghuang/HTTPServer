#ifndef TEST2_TIME_WHEEL_H
#define TEST2_TIME_WHEEL_H

class tw_timer;

class time_wheel {
    public:
        time_wheel();

        virtual ~time_wheel();

        tw_timer *add_timer(int timeout);

        void del_timer(tw_timer *timer);

        void tick();
        tw_timer *adjust_timer(tw_timer *timer, int timeout);

    private:
        static const int N= 60;//一圈60块区域
        static const int SI =1;//SI可以想象成时针或者分钟每次转动的幅度
        tw_timer*slots[N];
        int cur_slot;//当前所在区域

};

#endif //TEST2_TIME_WHEEL_H

