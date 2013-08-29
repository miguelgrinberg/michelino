/**
 * @file moving_average.h
 * @brief Helper class that maintains a moving average over a set of samples.
 * @author Miguel Grinberg
 */
 
template <typename V, int N> class MovingAverage
{
public:
    /*
     * @brief Class constructor.
     * @param n the size of the moving average window.
     * @param def the default value to initialize the average.
     */
    MovingAverage(V def = 0) : sum(0), p(0)
    {
        for (int i = 0; i < N; i++) {
            samples[i] = def;
            sum += samples[i];
        }
    }
    
    /*
     * @brief Add a new sample.
     * @param new_sample the new sample to add to the moving average.
     * @return the updated average.
     */
    V add(V new_sample)
    {
        sum = sum - samples[p] + new_sample;
        samples[p++] = new_sample;
        if (p >= N)
            p = 0;
        return sum / N;
    }
    
private:
    V samples[N];
    V sum;
    V p;
};
