#include "cmdparser.h"
#include "algorithms.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

int main(int argc, char **argv) {
    int help_flag = 0;
    char* fib_value = NULL;
    char* basic_value = NULL;

    struct CommandOption options[] = {
        {
            .help = "Show help information",
            .long_name = "help",
            .short_name = 'h',
            .has_arg = 0,
            .default_value = NULL,
            .handler = &help_flag
        },
        {
            .help = "Convert miles to km using Fibonacci (1-93 miles)",
            .long_name = "fib",
            .short_name = 'f',
            .has_arg = 1,
            .default_value = NULL,
            .handler = &fib_value
        },
        {
            .help = "Convert miles to km using standard formula",
            .long_name = "basic",
            .short_name = 'b',
            .has_arg = 1,
            .default_value = NULL,
            .handler = &basic_value
        }
    };

    struct CLIMetadata meta = {
        .prog_name = argv[0],
        .description = "Distance converter: miles to kilometers",
        .usage_args = "[distance]",
        .options = options,
        .options_count = sizeof(options) / sizeof(options[0])
    };

    int pos_index = parse_options(argc, argv, meta.options, meta.options_count);
    if (pos_index < 0) {
        return EXIT_FAILURE;
    }

    if (help_flag) {
        print_help(&meta);
        return EXIT_SUCCESS;
    }

    if (fib_value && basic_value) {
        fprintf(stderr, "Error: Cannot use both --fib and --basic options simultaneously\n");
        return EXIT_FAILURE;
    }

    if (fib_value) {
        char* endptr;
        long miles = strtol(fib_value, &endptr, 10);

        if (*endptr != '\0' || miles <= 0) {
            fprintf(stderr, "Error: Invalid distance value '%s'. Must be positive integer.\n", fib_value);
            return EXIT_FAILURE;
        }

        if (miles > 93) {
            fprintf(stderr, "Error: Distance too large. Maximum supported value is 93 miles.\n");
            return EXIT_FAILURE;
        }

        uint64_t fib_result = fibonacci(miles + 1);

        if (fib_result == 0 && miles != 0) {
            fprintf(stderr, "Error: Fibonacci calculation failed\n");
            return EXIT_FAILURE;
        }

        printf("%ld miles = %" PRIu64 " km (using Fibonacci)\n", miles, fib_result);
        return EXIT_SUCCESS;
    }

    if (basic_value) {
        char* endptr;
        double miles = strtod(basic_value, &endptr);

        if (*endptr != '\0' || miles < 0) {
            fprintf(stderr, "Error: Invalid distance value '%s'. Must be non-negative number.\n", basic_value);
            return EXIT_FAILURE;
        }

        float km = basic_miles2km(miles);
        printf("%.2f miles = %.2f km\n", miles, km);
        return EXIT_SUCCESS;
    }

    if (pos_index < argc) {
        for (int i = pos_index; i < argc; i++) {
            char* endptr;
            double miles = strtod(argv[i], &endptr);

            if (*endptr != '\0' || miles < 0) {
                fprintf(stderr, "Error: Invalid distance value '%s'. Skipping.\n", argv[i]);
                continue;
            }

            float km = basic_miles2km(miles);
            printf("%.2f miles = %.2f km\n", miles, km);
        }
        return EXIT_SUCCESS;
    }

    print_help(&meta);
    return EXIT_SUCCESS;
}
