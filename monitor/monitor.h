#ifndef MONITOR_H
#define MONITOR_H
#include <memory>
#include <vector>
#include <unordered_map>
#include <limits>  

namespace monitor {
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
      TypeName(const TypeName&);	\
      void operator=(const TypeName&)
class StatisticIf;
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
    uint32_t get_check_time_interval() {
        return _check_time;
    }

    void start();
    void add_statistic(StatisticIf* s);
    void print_stat();//TODO
    void stop();
    bool is_stop();
    void forward_time(uint32_t time);

private:
    Monitor();
    friend class std::unique_ptr<Monitor>;
    static std::unique_ptr<Monitor> _instance;
    std::vector<StatisticIf*> _statistics;
    uint32_t _check_time;
    bool _stop;
private:
    DISALLOW_COPY_AND_ASSIGN(Monitor);
};
#define g_common_monitor monitor::Monitor::Instance()

#define S_SUM               "COUNT"
#define S_TOTAL             "TOTAL"
#define S_AVG               "AVG"
#define S_AMOUNT_PER_SEC    "AMOUNT_PER_SEC"
#define S_MAX               "MAX"
#define S_MIN               "MIN"

const uint8_t kFlagCount = 1 << 0;
const uint8_t kFlagSum = 1 << 1;
const uint8_t kFlagMax = 1 << 2;
const uint8_t kFlagMin = 1 << 3;
const uint8_t kFlagBasic = kFlagCount | kFlagSum;
const uint8_t kFlagAll= kFlagCount | kFlagSum | kFlagMax | kFlagMin;

class StatisticIf {
public:
    static const uint32_t kDefaultRange = 300;
    static const uint32_t kDefaultSensitivity = 5;

public:
    virtual const char* name() = 0;
    virtual uint8_t flag() = 0;
    virtual uint32_t sensitivity() = 0;
    virtual void get_info(uint32_t range, std::string& info) = 0;//TODO
    virtual void forward_sec(uint32_t sec) = 0;
};
template<typename Type>
struct stat_interval_t {
    uint32_t count;
    Type sum;
    Type min;
    Type max;

    stat_interval_t():sum(), min(), max() {
        count = 0;
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
    uint32_t sensitivity() {return 0; }

    void get_info(uint32_t range, std::string& info);
    void forward_sec(uint32_t sec) {}

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
void EternalStatisticImpl<Type>::get_info(uint32_t range, std::string& info) {
    std::string s = _name;
    if (_flag & kFlagCount) {
        s += "\t";
        s += std::to_string(_interval->count);
    }

    if (_flag & kFlagSum) {
        s += "\t";
        s += std::to_string(_interval->sum);
    }

    if (_flag & kFlagMax) {
        s += "\t";
        s += std::to_string(_interval->max);
    }

    if (_flag & kFlagMin) {
        s += "\t";
        s += std::to_string(_interval->min);
    }
    info.assign(s.c_str());
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
        if (v > _interval->max)
            _interval->max = v;
    }

    if (_flag & kFlagMin) {
        if (v < _interval->min)
            _interval->min = v;
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
                  uint32_t range = kDefaultRange,
                  uint32_t sensitivity = kDefaultSensitivity) { 
        _name = name; 
        _flag = flag;
        _range = range;
        _sensitivity = sensitivity; 
        _interval_num = (_range + _sensitivity - 1) / _sensitivity + 2;
        _interval = new stat_interval_t<Type>[_interval_num];
        _cur_interval = _interval;
        _head_interval = _interval;
        _tail_interval = _interval + _interval_num - 1;
        _sec_count = 0;
    }

    virtual ~StatisticImpl() {
        delete[] _interval;  
    }

public:
    const char* name() { return _name.c_str(); }
    uint8_t flag() { return _flag; }
    uint32_t sensitivity() { return _sensitivity; }

    void forward_sec(uint32_t sec) {
        _sec_count += sec; 
        if (0 == _sec_count % _sensitivity) {
            stat_interval_t<Type>* new_open_interval = _post_interval(_cur_interval); 
            new_open_interval->reset();
            _cur_interval = new_open_interval;
        }
    }

    void get_info(uint32_t range, std::string& info) {
        std::string s = _name;
        uint32_t range_sec = range < _range ? range : _range;
        uint32_t range_interval_num = (range_sec + _sensitivity - 1) / _sensitivity;
        stat_interval_t<Type>* interval = _cur_interval;
        if (_flag & kFlagCount) {
            int count = 0;
            _calc_count_statistics(interval, range_interval_num, count);
            s += "\t";
            s += std::to_string(_interval->count);
        }
        if (_flag & kFlagMax) {
            s += "\t";
            Type max;
            _calc_max_statistics(interval, range_interval_num, max);
            s += std::to_string(max);
        }
        if (_flag & kFlagMin) {
            s += "\t";
            Type min;
            _calc_min_statistics(interval, range_interval_num, min);
            s += std::to_string(min);
        }

        if (_flag & kFlagSum) {
            s += "\t";
            Type sum;
            double amount_per_sec;
            double avg;
            _calc_sum_statistics(interval, range_interval_num, _flag & kFlagCount, sum, amount_per_sec, avg);
            s += std::to_string(sum);
            s += "\t";
            s+= std::to_string(amount_per_sec);
            s += "\t";
            s+= std::to_string(avg);
        }
        info.assign(s.c_str());
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
                                uint32_t num,
                                uint32_t& count);

    void _calc_sum_statistics(stat_interval_t<Type>* si,
                              uint32_t num,
                              bool has_count,
                              Type& sum,
                              double& amount_per_sec,
                              double& avg);

    void _calc_max_statistics(stat_interval_t<Type>* si,
                              uint32_t num,
                              Type& max);

    void _calc_min_statistics(stat_interval_t<Type>* si,
                              uint32_t num,
                              Type& min);
private:
    std::string _name;
    uint8_t _flag;
    uint32_t _range;
    uint32_t _sensitivity;
    uint32_t _interval_num;
    uint32_t _sec_count;
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
        if (interval->max < v) {
            interval->max = v;
        }
    }
    if (_flag & kFlagMin) {
        if (interval->min > v) {
            interval->min = v;
        }
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
                                                           uint32_t num, uint32_t& result) {
    stat_interval_t<Type>* interval = si;
    uint32_t count = 0;
    for (uint32_t i = 0; i < num; ++i) {
        interval = _pre_interval(interval); 
        count += interval->count;
    }
    result = count;
}

template<typename Type>
void StatisticImpl<Type>::_calc_sum_statistics(stat_interval_t<Type>* si,
                                                         uint32_t num,
                                                         bool has_count, 
                                                         Type& sum,
                                                         double& amount_per_sec,
                                                         double& avg) {
    stat_interval_t<Type>* interval = si;
    Type total = 0;
    for (uint32_t i = 0; i < num; ++i) {
        interval = _pre_interval(interval); 
        total += interval->sum.get_value();
    }
    sum = total;
    amount_per_sec = total * 1.0 / (_sensitivity * num);
    if (has_count) {
        int32_t count = 0;
        _calc_count_statistics(interval, num, count);
        if (count != 0) {
            avg = total * 1.0 / count;
        } else {
            avg = 0;
        }
    }
}

template<typename Type>
void StatisticImpl<Type>::_calc_max_statistics(stat_interval_t<Type>* si,
                                                         uint32_t num,
                                                         Type& max) {
    stat_interval_t<Type>* interval = _pre_interval(si);
    max = interval->max;
    for (uint32_t i = 1; i < num; ++i) {
        interval = _pre_interval(interval); 
        if (interval->max > max) {
            max = interval->max; 
        }
    }
    if (max == std::numeric_limits<Type>::min()) {
        max = 0;
    }
}

template<typename Type>
void StatisticImpl<Type>::_calc_min_statistics(stat_interval_t<Type>* si,
                                                         uint32_t num,
                                                         Type& min) {
    stat_interval_t<Type>* interval = _pre_interval(si);
    min = interval->min;
    for (uint32_t i = 1; i < num; ++i) {
        interval = _pre_interval(interval); 
        if (interval->min < min) {
            min = interval->min; 
        }
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

void start_monitor();

void end_monitor();

void* print_stat(void *arg);

/********************  declarers  ***********/
#define DECLARE_STATISTIC(name, type) \
        namespace monitor { \
                    extern StatisticImpl<type> g_##name; \
                } \
    using inf_monitor_global::g_##name;

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
        DEFINE_STATISTIC(name, int, ivar_int32_t, ##arg)

#define DEFINE_STATISTIC_INT64(name, arg...) \
        DEFINE_STATISTIC(name, int64, ivar_int64_t, ##arg)

#define DEFINE_STATISTIC_DOUBLE(name, arg...) \
        DEFINE_STATISTIC(name, double, ivar_double_t, ##arg)
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
