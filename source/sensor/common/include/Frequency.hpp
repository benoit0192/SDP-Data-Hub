#ifndef FREQUENCY
#define FREQUENCY

namespace sensor {

enum class Freq{
    HZ_10 = 10,
    HZ_20 = 20,
    HZ_30 = 30,
};

class Frequency{
public:
    Frequency()  = delete;
    ~Frequency() = delete;
    // TODO: Fix default case returning no frequency type compiler warning
    static Freq parseFrequency(int freq){
        switch (freq) {
            case 10: return Freq::HZ_10;
            case 20: return Freq::HZ_20;
            case 30: return Freq::HZ_30;
            default: std::cerr << "Invalid frequency provided." << "\n";
        }
    }
};

} // sensor

#endif // FREQUENCY