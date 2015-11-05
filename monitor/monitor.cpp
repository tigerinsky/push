#include <iostream>
#include <unistd.h>
#include <queue>
#include "monitor.h"

namespace monitor {
Monitor::Monitor() {
    _check_time = 0;//TODO gflags
}

Monitor::~Monitor() {
}

std::unique_ptr<Monitor> Monitor::_instance;

void Monitor::add_statistic(StatisticIf* s) {
    _statistics.push_back(s);
}

void Monitor::start() {
    if (0 == _statistics.size()) {
        return;
    }

    std::queue<uint32_t> sensitivity_list;
    for (uint32_t i = 0; i < _statistics.size(); ++i) {
        if (_statistics[i]->sensitivity()) {
            sensitivity_list.push(_statistics[i]->sensitivity());
        }
    }
    while (1 != sensitivity_list.size()) {
        uint32_t a = sensitivity_list.front(); 
        sensitivity_list.pop();
        uint32_t b = sensitivity_list.front(); 
        sensitivity_list.pop();
        uint32_t t = 0;
        if (a < b) {
            t = a;
            a = b;
            b = t;
        }
        while (t = a % b) {
            a = b;
            b = t;
        }
        sensitivity_list.push(b);
    }
    _check_time = sensitivity_list.front();
    _stop = false;;
}

void Monitor::print_stat() {
    std::vector<StatisticIf*>::iterator ite;
    for (ite = _statistics.begin(); ite != _statistics.end(); ++ite) {
        std::string info;
        (*ite)->get_info(300, info);
        std::cout << info  << std::endl;
    }

}

bool Monitor::is_stop() {
    return _stop;
}

void Monitor::stop() {
    _stop = false;
}

void Monitor::forward_time(uint32_t time) {
    std::vector<StatisticIf*>::iterator ite;
    for (ite = _statistics.begin(); ite != _statistics.end(); ++ite) {
        (*ite) -> forward_sec(time);
    }
}

void* print_stat(void *arg) {
    g_common_monitor->start();
    int check_time = g_common_monitor->get_check_time_interval();
    while(!g_common_monitor->is_stop()) {
        
        sleep(check_time);
        g_common_monitor->print_stat();
        g_common_monitor->forward_time(check_time);

    }
}

pthread_t start_monitor() {
    pthread_t pid;
    pthread_create(&pid, NULL, print_stat, NULL);
    return pid;
}

void end_monitor() {
    g_common_monitor->stop();//TODO 这里是不是得加锁
}

}//namespace monitor
