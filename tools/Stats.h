#ifndef Stats_DEFINED
#define Stats_DEFINED

#include <math.h>

#include "SkString.h"
#include "SkTSort.h"

static const char* kBars[] = { "▁", "▂", "▃", "▄", "▅", "▆", "▇", "█" };

struct Stats {
    Stats(const double samples[], int n) {
        min = samples[0];
        max = samples[0];
        for (int i = 0; i < n; i++) {
            if (samples[i] < min) { min = samples[i]; }
            if (samples[i] > max) { max = samples[i]; }
        }

        double sum = 0.0;
        for (int i = 0 ; i < n; i++) {
            sum += samples[i];
        }
        mean = sum / n;

        double err = 0.0;
        for (int i = 0 ; i < n; i++) {
            err += (samples[i] - mean) * (samples[i] - mean);
        }
        var = err / (n-1);

        SkAutoTMalloc<double> sorted(n);
        memcpy(sorted.get(), samples, n * sizeof(double));
        SkTQSort(sorted.get(), sorted.get() + n - 1);
        median = sorted[n/2];

        // Normalize samples to [min, max] in as many quanta as we have distinct bars to print.
        for (int i = 0; i < n; i++) {
            if (min == max) {
                // All samples are the same value.  Don't divide by zero.
                plot.append(kBars[0]);
                continue;
            }

            double s = samples[i];
            s -= min;
            s /= (max - min);
            s *= (SK_ARRAY_COUNT(kBars) - 1);
            const size_t bar = (size_t)round(s);
            SK_ALWAYSBREAK(bar < SK_ARRAY_COUNT(kBars));
            plot.append(kBars[bar]);
        }
    }

    double min;
    double max;
    double mean;    // Estimate of population mean.
    double var;     // Estimate of population variance.
    double median;
    SkString plot;  // A single-line bar chart (_not_ histogram) of the samples.
};

#endif//Stats_DEFINED
