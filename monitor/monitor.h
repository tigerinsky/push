#ifndef MONITOR_H
#define MONITOR_H
#include <memory>
#include <vector>
#include <unordered_map>
#include <limits>  
#include <pthread.h>
#include "common.h"

namespace monitor {

DECLARE_string(monitor_file);
DECLARE_int32(monitor_interval);

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
      TypeName(const TypeName&);	\
      void operator=(const TypeName&)
using std::string;
class StatisticIf;

typedef void (*stat_collector)(std::string* info);

class Monitor {
public:
    static Monitor* Instance() {
        if(0 == _instance.get()) {
            _instance.reset(new Monitor);
        }

        return _instance.get();
    }
    ~Monitor();

public:
    void start();
    void add_statistic(StatisticIf* s);
    void add_stat_collector(stat_collector func);
    void print_stat();//TODO
    void stop();
    bool is_stop();
    void forward();

private:
    Monitor();
    friend class std::unique_ptr<Monitor>;
    static std::unique_ptr<Monitor> _instance;
    std::vector<StatisticIf*> _statistics;
    std::vector<stat_collector> _stat_collectors;
    bool _stop;
private:
    DISALLOW_COPY_AND_ASSIGN(Monitor);
};
#define g_common_monitor monitor::Monitor::Instance()

const uint8_t kFlagCount = 1 << 0;
const uint8_t kFlagSum = 1 << 1;
const uint8_t kFlagMax = 1 << 2;
const uint8_t kFlagMin = 1 << 3;
const uint8_t kFlagBasic = kFlagCount | kFlagSum;
const uint8_t kFlagAll= kFlagCount | kFlagSum | kFlagMax | kFlagMin;

class StatisticIf {
public:
    static const uint32_t kDefaultRange = 10;

public:
    virtual const char* name() = 0;
    virtual uint8_t flag() = 0;
    virtual void get_info(std::string& info) = 0;//TODO
    virtual void forward() = 0;
};
template<typename Type>
struct stat_interval_t {
    uint32_t count;
    Type sum;
    Type min;
    Type max;

    stat_interval_t():
        count(0),
        sum(0), 
        min(std::numeric_limits<Type>::max()), 
        max(std::numeric_limits<Type>::min()) {
    }

    void reset() {
        count = 0;
        sum = 0;
        min = std::numeric_limits<Type>::max();
        max = std::numeric_limits<Type>::min();
    }

};

template<typename Type>
class EternalStatisticImpl : public StatisticIf {
public:
    EternalStatisticImpl(const char* name,
                         uint8_t flag = kFlagAll) {
        _name = name;
        _flag = flag;
        _interval = new stat_interval_t<Type>();
    }

    virtual ~EternalStatisticImpl() {
        delete _interval;
    }

public:
    const char* name() {return _name.c_str(); }
    uint8_t flag() {return _flag;}

    void get_info(std::string& info);
    void forward() {}

public:
    void add_value(Type v);
    void add_count();

private:
    std::string _name;
    uint8_t _flag;
    stat_interval_t<Type>* _interval;

private:
    DISALLOW_COPY_AND_ASSIGN(EternalStatisticImpl);
};

template<typename Type>
void EternalStatisticImpl<Type>::get_info(std::string& info) {
    info.clear();
    if (_flag & kFlagCount) {
        string_appendf(&info, 
                       "%sCount(~): %u\n", 
                       _name.c_str(), 
                       _interval->count);
    }

    if (_flag & kFlagSum) {
        string_appendf(&info, 
                       "%sSum(~): %u\n", 
                       _name.c_str(), 
                       _interval->sum);
    }

    if (_flag & kFlagMax) {
        string_appendf(&info, 
                       "%sMax(~): %u\n", 
                       _name.c_str(), 
                       _interval->max);
    }

    if (_flag & kFlagMin) {
        string_appendf(&info, 
                       "%sMin(~): %u\n", 
                       _name.c_str(), 
                       _interval->min);
    }
}

template<typename Type>
void EternalStatisticImpl<Type>::add_value(Type v) {
    if (_flag & kFlagCount) {
        _interval->count += 1;
    }

    if (_flag & kFlagSum) {
        _interval->sum += v;
    }

    if (_flag & kFlagMax) {
        _interval->max = std::max(v, _interval->max);
    }

    if (_flag & kFlagMin) {
        _interval->min = std::min(v, _interval->min);
    }
}

template<typename Type>
void EternalStatisticImpl<Type>::add_count() {
    if (_flag & kFlagCount) {
        _interval->count += 1;
    }
}

template<typename Type>
class StatisticImpl : public StatisticIf {

public:
    StatisticImpl(const char* name, 
                  uint8_t flag = kFlagAll,
                  uint32_t range = kDefaultRange) { 
        _name = name; 
        _flag = flag;
        _range = range;
    }

    virtual ~StatisticImpl() {
        delete[] _interval;  
    }

public:
    const char* name() { return _name.c_str(); }
    uint8_t flag() { return _flag; }

    void  init () {
        _data_interval_num 
            = (_range + FLAGS_monitor_interval - 1) / FLAGS_monitor_interval;
        _data_range = _data_interval_num * FLAGS_monitor_interval;
        _interval_num = _data_interval_num + 2;
        _interval = new stat_interval_t<Type>[_interval_num];
        _cur_interval = _interval;
        _head_interval = _interval;
        _tail_interval = _interval + _interval_num - 1;
    }

    void forward() {
        stat_interval_t<Type>* new_open_interval = _post_interval(_cur_interval); 
        new_open_interval->reset();
        _cur_interval = new_open_interval;
    }

    void get_info(std::string& info) {
        info.clear();
        stat_interval_t<Type>* interval = _cur_interval;
        uint32_t count = 0;
        if (_flag & kFlagCount) {
            _calc_count_statistics(interval, count);
            string_appendf(&info, "%sCount(%u): %u\n", _name.c_str(), _data_range, count);
            string_appendf(&info, 
                           "%sNumPerSec(%u): %lf\n", 
                           _name.c_str(), 
                           _data_range, 
                           count * 1.0 /_data_range);
        }
        if (_flag & kFlagMax) {
            Type max;
            _calc_max_statistics(interval, max);
            string_appendf(&info, 
                           "%sMax(%u): %s\n", 
                           _name.c_str(), 
                           _data_range, 
                           std::to_string(max).c_str());
        }
        if (_flag & kFlagMin) {
            Type min;
            _calc_min_statistics(interval, min);
            string_appendf(&info, 
                           "%sMin(%u): %s\n", 
                           _name.c_str(), 
                           _data_range, 
                           std::to_string(min).c_str());
        }
        if (_flag & kFlagSum) {
            Type sum;
            _calc_sum_statistics(interval, sum);
            string_appendf(&info,
                           "%sSum(%u): %s\n",
                           _name.c_str(),
                           _data_range,
                           std::to_string(sum).c_str());
            string_appendf(&info,
                           "%sAmountPerSec(%u): %lf\n",
                           _name.c_str(),
                           _data_range,
                           sum * 1.0 / _data_range);
            if (_flag & kFlagCount) {
                double avg = 0 == count ? 0 : sum * 1.0 / count;
                string_appendf(&info,
                               "%sAmountAvg(%u): %lf\n",
                               _name.c_str(),
                               _data_range,
                               avg); 
            }
        }
    }

public:
    /**
     * 收集带value的统计量
     * */
    void add_value(Type v);
    /**
     * 收集只统计count的统计量，也可以用add_value(v, 1)实现,
     * 不过这样专门拿出来性能会好些,因为有很多统计量是只用统计count的
     * */
    void add_count();

private:
    stat_interval_t<Type>* _pre_interval(stat_interval_t<Type>* si) {
        if (_head_interval == si) {
            return _tail_interval; 
        } else {
            return si - 1; 
        }
    }

    stat_interval_t<Type>* _post_interval(stat_interval_t<Type>* si) {
        if (_tail_interval == si) {
            return _head_interval; 
        } else {
            return si + 1; 
        } 
    }
    
    void _calc_count_statistics(stat_interval_t<Type>* si,
                                uint32_t& count);

    void _calc_sum_statistics(stat_interval_t<Type>* si,
                              Type& sum);

    void _calc_max_statistics(stat_interval_t<Type>* si,
                              Type& max);

    void _calc_min_statistics(stat_interval_t<Type>* si,
                              Type& min);
private:
    std::string _name;
    uint8_t _flag;
    uint32_t _range;
    uint32_t _data_range;
    uint32_t _data_interval_num;
    uint32_t _interval_num;
    stat_interval_t<Type>* _interval;
    stat_interval_t<Type>* _cur_interval;
    stat_interval_t<Type>* _head_interval;
    stat_interval_t<Type>* _tail_interval;

private:
    DISALLOW_COPY_AND_ASSIGN(StatisticImpl);
};

template<typename Type>
void StatisticImpl<Type>::add_value(Type v) {
    stat_interval_t<Type>* interval = _cur_interval;
    if (_flag & kFlagCount) {
        interval->count += 1; 
    }
    if (_flag & kFlagSum) {
        interval->sum += v;
    }
    if (_flag & kFlagMax) {
        interval->max = std::max(v, interval->max);
    }
    if (_flag & kFlagMin) {
        interval->min = std::max(v, interval->min);
    }
}

template<typename Type>
void StatisticImpl<Type>::add_count() {
    stat_interval_t<Type>* interval = _cur_interval;
    if (_flag & kFlagCount) {
        interval->count += 1; 
    }
}

template<typename Type>
void StatisticImpl<Type>::_calc_count_statistics(stat_interval_t<Type>* si,
                                                 uint32_t& result) {
    stat_interval_t<Type>* interval = si;
    uint32_t count = 0;
    for (uint32_t i = 0; i < _data_interval_num; ++i) {
        interval = _pre_interval(interval); 
        count += interval->count;
    }
    result = count;
}

template<typename Type>
void StatisticImpl<Type>::_calc_sum_statistics(stat_interval_t<Type>* si,
                                               Type& sum) {
    stat_interval_t<Type>* interval = si;
    Type total = 0;
    for (uint32_t i = 0; i < _data_interval_num; ++i) {
        interval = _pre_interval(interval); 
        total += interval->sum;
    }
    sum = total;
}

template<typename Type>
void StatisticImpl<Type>::_calc_max_statistics(stat_interval_t<Type>* si,
                                               Type& max) {
    stat_interval_t<Type>* interval = _pre_interval(si);
    max = interval->max;
    for (uint32_t i = 1; i < _data_interval_num; ++i) {
        interval = _pre_interval(interval); 
        max = std::max(interval->max, max);
    }
    if (max == std::numeric_limits<Type>::min()) {
        max = 0;
    }
}

template<typename Type>
void StatisticImpl<Type>::_calc_min_statistics(stat_interval_t<Type>* si,
                                               Type& min) {
    stat_interval_t<Type>* interval = _pre_interval(si);
    min = interval->min;
    for (uint32_t i = 1; i < _data_interval_num; ++i) {
        interval = _pre_interval(interval); 
        min = std::min(interval->min, min);
    }
    if (min == std::numeric_limits<Type>::max()) {
        min = 0;
    }
}

class StatisticRegister {
public:
    StatisticRegister(StatisticIf* s) {
        g_common_monitor->add_statistic(s);
    }

};

pthread_t start_monitor();

void end_monitor();

void* print_stat(void *arg);

/********************  declarers  ***********/
#define DECLARE_STATISTIC(name, type) \
        namespace monitor { \
                    extern StatisticImpl<type> g_##name; \
                } \
    using monitor::g_##name;

#define DECLARE_STATISTIC_INT32(name) \
        DECLARE_STATISTIC(name, int)

#define DECLARE_STATISTIC_INT64(name) \
        DECLARE_STATISTIC(name, int64_t)

#define DECLARE_STATISTIC_DOUBLE(name) \
        DECLARE_STATISTIC(name, double)

#define DECLARE_ETERNAL_STATISTIC(name, type) \
    namespace monitor { \
        extern  EternalStatisticImpl<type> g_##name; \
    } \
    using monitor::g_##name;

#define DECLARE_ETERNAL_STATISTIC_INT32(name) \
    DECLARE_ETERNAL_STATISTIC(name, int)

#define DECLARE_ETERNAL_STATISTIC_INT64(name) \
    DECLARE_ETERNAL_STATISTIC(name, int64_t)

#define DECLARE_ETERNAL_STATISTIC_DOUBLE(name) \
    DECLARE_ETERNAL_STATISTIC(name, double)

/*****************   definers   ***********/
#define DEFINE_STATISTIC(name, type, arg...) \
    namespace monitor {\
        StatisticImpl<type> g_##name(#name, ##arg); \
        static StatisticRegister \
        register_##name(&g_##name); \
    } \
    using monitor::g_##name;

#define DEFINE_STATISTIC_INT32(name, arg...) \
        DEFINE_STATISTIC(name, int, ##arg)

#define DEFINE_STATISTIC_INT64(name, arg...) \
        DEFINE_STATISTIC(name, int64_t, ##arg)

#define DEFINE_STATISTIC_DOUBLE(name, arg...) \
        DEFINE_STATISTIC(name, double, ##arg)
#define DEFINE_ETERNAL_STATISTIC(name, type, flag) \
    namespace monitor{\
        EternalStatisticImpl<type> g_##name(#name, flag); \
        static StatisticRegister \
        register_##name(&g_##name); \
    } \
    using monitor::g_##name;

#define DEFINE_ETERNAL_STATISTIC_INT32(name, flag) \
        DEFINE_ETERNAL_STATISTIC(name, int, flag)

#define DEFINE_ETERNAL_STATISTIC_INT64(name, flag) \
        DEFINE_ETERNAL_STATISTIC(name, int64_t, flag)

#define DEFINE_ETERNAL_STATISTIC_DOUBLE(name, flag) \
        DEFINE_ETERNAL_STATISTIC(name, double, flag)

#define ADD_STAT_COLLECTOR(func) \
    do { \
        g_common_monitor->add_stat_collector(func); \
    } while (0);

/*********   methods   *********/
#define STAT_COLLECT(name, value) \
    do { \
        g_##name.add_value(value); \
    } while (0);

#define STAT_COLLECT_COUNT(name) \
    do { \
        g_##name.add_count(); \
    } while (0);

}//namespace monitor
#endif
