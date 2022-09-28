// EPOS Predictor Declarations

#ifndef __predictor_h
#define __predictor_h

#include <utility/math.h>
#include <utility/buffer.h>

__BEGIN_UTIL

class Predictor_Common
{
public:
    static const bool LISTENER = false;

    // Predictor Types (defined in Traits)
    typedef unsigned char Predictor_Type;
    enum : unsigned char {
        NONE = Traits<Build>::NONE,
        LVP = Traits<Build>::LVP,
        DBP = Traits<Build>::DBP
    };

    // Predictor Model Types
    enum Model_Type {
        VOID             = 0,
        CONSTANT         = 1,
        LINEAR           = 2,
        SPATIAL_CONSTANT = 3,
        SPATIAL_LINEAR   = 4,
    };

    // History Types
    enum History_Type { TEMPORAL, ATEMPORAL };

    // Predictor Models
    template<typename Time, typename Value>
    class Constant
    {
    public:
        Constant(const Value & v = 0) : _value(v) {}

        Value operator()(const Time & t) { return _value; }

        Value value() const { return _value; }
        void value(const Value & v)  { _value = v; }

    private:
        Value _value;
    } __attribute__((packed));

    template<typename Time, typename Value>
    class Linear
    {
    public:
        Linear(const Value & a = 0, const Value & b = 0, const Time & t0 = 0): _a(a), _b(b), _t0(t0) {}

        Value operator()(const Time & t1) { return (_a * (t1 - _t0) + _b); }

        Value a() const { return _a; }
        void a(const Value & a)  { _a = a; }
        Value b() const { return _b; }
        void b(const Value & b) { _b = b; }
        Time t0() const { return _t0; }
        void t0(const Time & t0) { _t0 = t0; }

    private:
        Value _a;
        Value _b;
        Time _t0;
    } __attribute__((packed));

    template<typename Space, typename Model>
    class Spatial: public Model
    {
    public:
        template<typename ... Tn>
        Spatial(Tn ... an): Model(an ...) {}

        template<typename ... Tn>
        Spatial(const Space & s, Tn ... an): Model(an ...), _space(s) {}

        const Space & space() const { return _space; }
        void space(const Space & s)  { _space = s; }

    private:
        Space _space;
    } __attribute__((packed));

    template<unsigned int T, typename _Model>
    class Model: public _Model
    {
    public:
        static const unsigned int TYPE = T;

    public:
        template<typename ... Tn>
        Model(const unsigned char & type, Tn ... an): _Model(an ...), _type(type), _id(0) {}

        unsigned char type() const { return _type; }
        unsigned char id() const { return _id; }

    private:
        void id(unsigned char id) { _id = id; }

    private:
        unsigned char _type;
        unsigned char _id;
    } __attribute__((packed));

    template<typename Time = int, typename Value = int>
    using Dummy_Model = Model<VOID, void>;

    template<typename Time, typename Value>
    using Constant_Model = Model<CONSTANT, Constant<Time, Value>>;

    template<typename Time, typename Value>
    using Linear_Model = Model<LINEAR, Linear<Time, Value>>;

    template<typename Space, typename Time, typename Value>
    using Spatial_Constant_Model = Model<SPATIAL_CONSTANT, Spatial<Space, Constant<Time, Value>>>;

    template<typename Space, typename Time, typename Value>
    using Spatial_Linear_Model = Model<SPATIAL_LINEAR, Spatial<Space, Linear<Time, Value>>>;


    // History records
    template<History_Type TYPE, typename ... Tn>
    class Record;

    // Record for temporal predictors' histories
    template<typename Time, typename Value>
    class Record<TEMPORAL, Time, Value>
    {
    public:
        Record(const Time & time = 0, const Value & value = 0): _time(time), _value(value) {}

        Value value() const { return _value; }
        Time time() const { return _time; }

    private:
        Time _time;
        Value _value;
    };

    // Record for non-temporal predictors' histories
    template<typename Value>
    class Record<ATEMPORAL, Value>
    {
    public:
        Record(const Value & value=0): _value(value) {}

        Value value() const { return _value; }

    private:
        Value _value;
    };

    // History (circular buffer of Records)
    template<History_Type TYPE, unsigned int SIZE, typename ... Tn>
    class History: public Circular_Buffer<Record<TYPE, Tn ...>, SIZE> {};


//protected:
//    Predictor_Common(const unsigned char & type): _type(type) {}
//
//public:
//    unsigned char type() const { return _type; }
//
//private:
//    unsigned char _type;
};

template<unsigned int T>
class Predictor_Common::Model<T, void>
{
public:
    static const unsigned int TYPE = T;

public:
    template<typename ... Tn>
    Model(const Predictor_Type & type, Tn ... an): _type(type), _id(0) {}

    int operator()(const int & t) { assert(false); return 0; }

    unsigned char type() const { return _type; }
    unsigned char id() const { return _id; }

private:
    void id(unsigned char id) { _id = id; }

private:
    Predictor_Type _type;
    unsigned char _id;
} __attribute__((packed));

template<typename Time, typename Value>
class Dummy_Predictor: public Predictor_Common
{
public:
    static const Predictor_Type TYPE = NONE;

    typedef Dummy_Model<Time, Value> Model;
    struct Configuration {};

public:
    Dummy_Predictor(): _model(TYPE) {}
    Dummy_Predictor(const Configuration & c, bool r): _model(TYPE) {}

    int predict(const Time & time) const { return 0; }

    template<typename Config>
    void configure(const Config & conf) {}
    bool trickle(const Time & t, const Value & v) { assert(false); return false; }

    const Model & model() const { assert(false); return _model; }
    void model(const Model & m) { assert(false); }

    void update(const Model & m, const bool & from_sink) { _model = m; }

private:
    Model _model;
};

// Last Value Predictor
template<typename Time, typename Value>
class LVP: public Predictor_Common
{
public:
    static const Predictor_Type TYPE = Predictor_Common::LVP;

    typedef Constant_Model<Time, Value> Model;

    struct Configuration
    {
        Configuration(Value r = 0, Value a = 0, Time t = 0): relative_error(r), absolute_error(a), time_error(t) {}

        template<typename Config>
        Configuration(const Config & conf): relative_error(conf.relative_error), absolute_error(conf.absolute_error), time_error(conf.time_error) {}

        friend Debug & operator<<(Debug & db, const Configuration & c) {
            db << "{r=" << c.relative_error << ",a=" << c.absolute_error << ",t=" << c.time_error << "}";
            return db;
        }

        Value relative_error;
        Value absolute_error;
        Time time_error;
    } __attribute__((packed));

public:
    LVP(Value r = 0, Value a = 0, Time t = 0): _config(r, a, t), _miss_predicted(0) {
        db<Predictors>(TRC) << "LVP(r=" << r << ",a=" << a << ",t=" << t << ")" << endl;
        db<Predictors>(INF) << "LVP:config=" << _config << ",model=" << _model << ")" << endl;
    }

    LVP(const Configuration & c, bool r = false): _config(c), _miss_predicted(0) {
        db<Predictors>(TRC) << "LVP(c=" << c << ",r=" << r << ")" << endl;
        db<Predictors>(INF) << "LVP:config=" << _config << ",model=" << _model << ")" << endl;
    }

    template<typename ... Tn>
    Value predict(const Time & t, const Tn & ... an) const {
        return _model(t, an...);
    }

    bool trickle(const Time & time, const Value & value) {
        db<Predictors>(TRC) << "LVP::trickle(t=" << time << ",v=" << value << ",t=" << time << ")" << endl;

        float predicted = predict(time);
        float max_acceptable_error = Math::max(Math::abs(((float)value * (float)_config.relative_error)/100.0f), (float)_config.absolute_error);
        float error = Math::abs((float)value - predicted);

        db<Predictors>(TRC) << "LVP::trickle:real=" << value << ",pred=" << predicted << ",err=" << error << ",max=" << max_acceptable_error << ",t_err:" << _config.time_error << ",miss:" << _miss_predicted << ")" << endl;

        if(error > max_acceptable_error) {
            if(++_miss_predicted > _config.time_error) {
                _model->value(value);
                _miss_predicted = 0;
                return false;
            }
        } else {
            _miss_predicted = 0;
        }

//            Value predicted = (*_model)(time);
//            if(predicted != value){
//                _model->value(value);
//                return false;
//            }
        return true;
    }

    const Model & model() const { return _model; }
    void model(const Model & m) { _model = m; }

    void update(const Model & m, const bool & from_sink) { _model = m; }

    template<typename Config>
    void configure(const Config & c) {
        _config.relative_error = c.relative_error;
        _config.absolute_error = c.absolute_error;
        _config.time_error = c.time_error;
    }

private:
    Configuration _config;
    Model _model;
    unsigned int _miss_predicted;
};


// Derivative-based Predictor (DBP)
template<typename Time, typename Value>
class DBP: public Predictor_Common
{
private:
    static const Predictor_Type TYPE = Predictor_Common::DBP;
    static const unsigned int MAX_WINDOW = 100;

public:
    typedef Linear_Model<Time, Value> Model;

    struct Configuration
    {
        Configuration(Value r = 0, Value a = 0, Time t = 0, unsigned int w = 0, unsigned int p = 0)
        : relative_error(r), absolute_error(a), time_error(t), window_size(w), points(p) {}

        template<typename Config>
        Configuration(const Config & conf)
        : relative_error(conf.relative_error), absolute_error(conf.absolute_error), time_error(conf.time_error), window_size(conf.window_size), points(conf.points) {}

        friend Debug & operator<<(Debug & db, const Configuration & c) {
            db << "{r=" << c.relative_error << ",a=" << c.absolute_error << ",t=" << c.time_error << ",w=" << c.window_size << ",l=" << c.points << "}";
            return db;
        }

        Value relative_error;
        Value absolute_error;
        Time time_error;
        unsigned int window_size;
        unsigned int points;
    } __attribute__((packed));

public:
    DBP(unsigned int w, unsigned int p, Value r = 0, Value a = 0, Time t = 0): _ready(false), _config(w, p, r, a, t), _miss_predicted(0) {
        db<Predictors>(TRC) << "DBP(r=" << r << ",a=" << a << ",t=" << t << ")" << endl;
        db<Predictors>(INF) << "DBP:config=" << _config << ",model=" << _model << ")" << endl;
    }

    DBP(const Configuration & c, bool r = false): _ready(false), _config(c), _miss_predicted(0) {
        db<Predictors>(TRC) << "DBP(c=" << c << ",r=" << r << ")" << endl;
        db<Predictors>(INF) << "DBP:config=" << _config << ",model=" << _model << ")" << endl;
    }

    template<typename ... Tn>
    Value predict(const Time & t, const Tn & ... an) const {
        if(_ready)
            return _model(t, an ...);
        else if(!_history.empty())
            return _history->tail().value();
        else
            return 0;
    }

    void update(const Time & t, const Value & v) { _history.insert(t, v); }

    bool trickle(const Time & time, const Value & value) {
        update(time, value);

        if(!_model) {
            if(_history.full()){
                build_model(time, value);
                return false;
            }
        } else {
            float predicted = predict(time);
            OStream cout;
            float max_acceptable_error = Math::max(Math::abs(((float)value * (float)_config.relative_error)/100.0f), (float)_config.absolute_error);
            float error = Math::abs((float)value - predicted);

            db<Predictors>(TRC) << "LVP::trickle:real=" << value << ",pred=" << predicted << ",err=" << error << ",max=" << max_acceptable_error << ",t_err:" << _config.time_error << ",miss:" << _miss_predicted << ")" << endl;

            if(error > max_acceptable_error) {
                if(++_miss_predicted > _config.time_error) {
                    build_model(time, value);
                    _miss_predicted = 0;
                    return false;
                }
            } else {
                _miss_predicted = 0;
            }
        }

        return true;
    }

    const Model & model() const { return _model; }
    void model(const Model & m) { _model = m; }

    void update(const Model & m, const bool & from_sink) { _model = m; }

    template<typename Config>
    void configure(const Config & c) {
        _config.points = c.points;
        _config.relative_error = c.relative_error;
        _config.absolute_error = c.absolute_error;
        _config.time_error = c.time_error;
    }

protected:
    void build_model(const Time & t, const Value & v){
        assert(_history.full());

        float avg_oldest = 0;
        float avg_recent = 0;

        _model.t0(_history[0].time());

        for(unsigned int i = 0; i < _config.points; i++)
            avg_oldest += _history[i].value();
        avg_oldest /= _config.points;

        for(unsigned int i = 0; i < _config.point; i++)
            avg_recent += _history[_history.size() -1 - i].value();
        avg_recent /= _config.points;

        float t_oldest = (_history[0].time() + _history[_config.points - 1].time()) / 2;
        float t_recent = (_history[_history.size() - 1].time() + _history[_history.size() -1 - (_config.point - 1)].time()) / 2;

        _model.a((avg_recent - avg_oldest) / (t_recent - t_oldest));
        _model.b(avg_oldest);
        _model.t0(t_oldest);

        float predicted = predict(t);
        float error = v - predicted;
        _model.b(avg_oldest + error);
        _ready = true;
    }

private:
    bool _ready;
    unsigned int _miss_predicted;

    Configuration _config;
    Model _model;
    History<TEMPORAL, MAX_WINDOW, Time, Value> _history;
};

template<Predictor_Common::Predictor_Type TYPE>
struct Select_Predictor
{
    template<typename Time, typename Value>
    using Predictor = Dummy_Predictor<Time, Value>;
};

template<>
struct Select_Predictor<Predictor_Common::LVP>
{
    template<typename Time, typename Value>
    using Predictor = LVP<Time, Value>;
};

template<>
struct Select_Predictor<Predictor_Common::DBP>
{
    template<typename Time, typename Value>
    using Predictor = DBP<Time, Value>;
};

__END_UTIL

#endif
