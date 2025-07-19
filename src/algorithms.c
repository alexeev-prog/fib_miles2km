#include <float.h>
#include <math.h>
#include <stdint.h>

uint64_t fibonacci(int num) {
    if (num < 0) return 0;
    if (num == 0) return 0;

    uint64_t a = 0;   // F(0)
    uint64_t b = 1;   // F(1)

    // Специальная обработка для F(1)
    if (num == 1) return b;

    // Вычисляем числа Фибоначчи итеративно
    for (int i = 2; i <= num; i++) {
        uint64_t next = a + b;
        a = b;
        b = next;
    }

    return b;
}

float basic_miles2km(int miles) {
    return miles * 1.609344f;
}

float fib_interpolate(float miles) {
    if (miles < 5.0f) return basic_miles2km(miles);

    uint64_t a = 0, b = 1;  // F0, F1
    uint64_t prev_mile = 0, prev_km = 1;  // F0, F1
    uint64_t curr_mile = 1, curr_km = 2;  // F1, F2

    // Find Fibonacci interval
    while (curr_mile <= miles) {
        prev_mile = curr_mile;
        prev_km = curr_km;

        // Next Fibonacci pair
        curr_mile = prev_km;
        curr_km = prev_mile + prev_km;

        // Check for overflow
        if (curr_km < prev_km || curr_mile < prev_mile) {
            break;
        }
    }

    // Linear interpolation
    return prev_km + (miles - prev_mile) *
        ((float)(curr_km - prev_km) / (curr_mile - prev_mile));
}

float fib_cache_convert(float miles) {
    #define MAX_CACHE 94
    static uint64_t cache[MAX_CACHE] = {0};
    static int initialized = 0;

    // Initialize cache once
    if (!initialized) {
        cache[0] = 0;
        cache[1] = 1;
        for (int i = 2; i < MAX_CACHE; i++) {
            cache[i] = cache[i-1] + cache[i-2];
        }
        initialized = 1;
    }

    if (miles < 5.0f) return basic_miles2km(miles);

    // Find interval
    int i = 2;
    while (i < MAX_CACHE-2 && cache[i] <= miles) i++;

    if (i >= MAX_CACHE-2) {
        return basic_miles2km(miles);
    }

    // Interpolation
    uint64_t Fn = cache[i-1];    // F_k
    uint64_t Fn1 = cache[i];     // F_{k+1}
    uint64_t Fn2 = cache[i+1];   // F_{k+2}

    return Fn1 + (miles - Fn) *
        ((float)(Fn2 - Fn1) / (Fn1 - Fn));
}

float fib_golden_ratio(float miles) {
    const double phi = (1.0 + sqrt(5.0)) / 2.0;

    if (miles < 1e-5) return 0.0f;

    // Find Fibonacci index
    double n = log(miles * sqrt(5.0)) / log(phi);
    int k = (int)floor(n);

    // Calculate Fibonacci numbers using Binet's formula
    double Fk = (pow(phi, k) - pow(-phi, -k)) / sqrt(5.0);
    double Fk1 = (pow(phi, k+1) - pow(-phi, -k-1)) / sqrt(5.0);
    double Fk2 = (pow(phi, k+2) - pow(-phi, -k-2)) / sqrt(5.0);

    // Handle edge cases
    if (Fk1 - Fk < DBL_EPSILON) {
        return basic_miles2km(miles);
    }

    // Linear interpolation
    return Fk1 + (miles - Fk) *
        ((float)(Fk2 - Fk1) / (Fk1 - Fk));
}
