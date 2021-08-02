#include <cstdio>
#include "time_wheel.h"
#include "tw_timer.h"

time_wheel::~time_wheel() {
    for (int i = 0; i < N; ++i) {
        tw_timer *tmp = slots[i];
        while (tmp) {
            slots[i] = tmp->next;
            delete tmp;
            tmp = slots[i];
        }
    }
}

time_wheel::time_wheel() : cur_slot(0) {
    for (int i = 0; i < N; ++i) {
        slots[i] = nullptr;
    }
}

tw_timer *time_wheel::add_timer(int timeout) {
    if (timeout < 0) {
        return nullptr;
    }
    int ticks = 0;
    if (timeout < SI) {
        //这个水表一次只能跳5小格，那当然定时3小格至少要时针跳一次
        ticks = 1;
    } else {
        ticks = timeout / SI;//跳的次数
    }
    int rotation = ticks / N;//跳的圈数
    int ts = (cur_slot + (ticks % N)) % N;//跳的最终位置（时间）
    //这里它将timer在timer_wheel内部动态分配了，这也就导致了使用接口和时间升序链表有所区别，见后文
    tw_timer *timer = new tw_timer(rotation, ts);
    if (!slots[ts]) {
//        printf("ticks =%d ,timeout=%d\n",ticks,timeout);
//        printf("add timer,rotation is %d,ts is %d,cur_slot is %d\n", rotation, ts, cur_slot);
        slots[ts] = timer;
    } else {
        timer->next = slots[ts];
        slots[ts]->prev = timer;
        slots[ts] = timer;
    }

    return timer;
}
//这个adjust_timer函数其实书上没有，因为和时间升序链表的接口比较相似，我就把节点摘下来，再重新放到时间轮上=del_timer+add_timer，也就是用于客户在发送消息之后我们重新设置超时的需求
tw_timer *time_wheel::adjust_timer(tw_timer *timer, int timeout) {
    if (timeout < 0) {
        return nullptr;
    }
    int ots = timer->time_slot;

    if (timer == slots[ots]) {
        slots[ots] = slots[ots]->next;
        if (slots[ots]) {
            slots[ots]->prev = nullptr;
        }
    } else {
        timer->prev->next = timer->next;
        if (timer->next) {
            timer->next->prev = timer->prev;
        }
    }

    int ticks = 0;
    if (timeout < SI) {
        ticks = 1;
    } else {
        ticks = timeout / SI;
    }
    int rotation = ticks / N;
    int ts = (cur_slot + (ticks % N)) % N;
    timer->time_slot = ts;
    timer->rotation = rotation;

    if (!slots[ts]) {
//        printf("add timer,rotation is %d,ts is %d,cur_slot is %d\n", rotation, ts, cur_slot);
        slots[ts] = timer;
    } else {
        timer->next = slots[ts];
        slots[ts]->prev = timer;
        slots[ts] = timer;
    }
    return timer;
}

void time_wheel::del_timer(tw_timer *timer) {
    if (!timer) {
        return;
    }
    int ts = timer->time_slot;
    if (timer == slots[ts]) {
        slots[ts] = slots[ts]->next;
        if (slots[ts]) {
            slots[ts]->prev = nullptr;
        }
        delete timer;
    } else {
        timer->prev->next = timer->next;
        if (timer->next) {
            timer->next->prev = timer->prev;
        }
        delete timer;
    }
}

void time_wheel::tick() {
    tw_timer *tmp = slots[cur_slot];
//    printf("current slot is %d\n", cur_slot);
    while (tmp) {
        printf("tick the timer once\n");
        //因为时针转过一圈才会回到相同的cur_slot,所以每调用tick且轮到相同的cur_slot时，将rotation--直到0为止
        if (tmp->rotation > 0) {
            tmp->rotation--;
            tmp = tmp->next;//换到这个区域内链表的下一个节点，查看他它是否超时
        } else {
            //超时则调用函数，删除节点
            tmp->cb_func(tmp->user_data);
            if (tmp == slots[cur_slot]) {
//                printf("delete header in cur_slot\n");
                slots[cur_slot] = tmp->next;
                delete tmp;
                if (slots[cur_slot]) {
                    slots[cur_slot]->prev = nullptr;
                }
                tmp = slots[cur_slot];
            } else {
                tmp->prev->next = tmp->next;
                if (tmp->next) {
                    tmp->next->prev = tmp->prev;
                }
                tw_timer *tmp2 = tmp->next;
                delete tmp;
                tmp = tmp2;
            }
        }
    }
    //时针转1格
    cur_slot = ++cur_slot % N;
}


