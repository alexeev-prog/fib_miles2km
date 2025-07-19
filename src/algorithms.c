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
