#ifndef FREQUENCY
#define FREQUENCY

namespace sensor {

enum class Freq {
    HZ_5  = 5,
    HZ_10 = 10,
    HZ_20 = 20,
    HZ_30 = 30,
};

class Frequency {
public:
    Frequency()  = delete;
    ~Frequency() = delete;
    static Freq parseFrequency(int freq) {
        switch (freq) {
            case 10: return Freq::HZ_10;
            case 20: return Freq::HZ_20;
            case 30: return Freq::HZ_30;
            default: throw std::invalid_argument("Invalid frequency.");
        }
    }
};

} // sensor

#endif // !FREQUENCY
