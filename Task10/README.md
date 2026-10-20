# Task 10 - Compare expectations with execution

## Before execution

`expectations.txt` records the repeated five-fare cycle and mathematical totals
before execution. The cycle is Inner peak Rs12, Middle off-peak Rs20, Outer peak
Rs36, Student Middle off-peak Rs18, Senior Outer peak Rs28.80. It totals 11480
paise. All three required counts are divisible by five, so the expected exact
totals are 45,920,000; 1,148,000,000; 6,888,000,000 paise respectively. These are
actual configured concessional fares, unlike the separate Task3 diagnostic.

The program checks these totals and the first would-overflow journey. Actual
measurements, including differences and timings, are in `measurements.csv` and
the captured output. A generated `observations.md` lists the measured numbers
beside the independent expectations. Time/error values are not invented or
rounded to hide differences.

## Why int, long long and double differ

On this system int is 4 bytes, long long and double are 8 bytes each. The 32-bit
int paise total fits 20,000 and 500,000 journeys, but fails at the network count.
The cycle reaches 2,147,483,240 paise after 935,315 journeys. The next 1200-paise
fare exceeds INT_MAX, so a checked int accumulator stops at journey 935,316.
It never deliberately executes signed integer overflow: C++ signed overflow is
undefined behaviour, and an apparently wrapped result would not be reliable
evidence of a defined language result. The representability check demonstrates
the failure without relying on that behaviour.

Long long paise remains exact at 6,888,000,000 and leaves ample headroom. It is
still finite: overflow checks or bounded totals are necessary for arbitrary
lifetime data. Double rupees has a wide numeric range but represents 28.8 only
approximately; repeated additions round at changing magnitudes. Its difference
from the long-double conversion of the exact paise total is measured at all
three scales. This is different from Task3's Rs36 experiment, which correctly
reports no double error for exact whole-rupee values at these counts.

## Compiler diagnostics and actual differences

All operational examples are built with `-std=c++14 -Wall -Wextra -pedantic -O2`.
The required unmodified Task4 prediction example emits `-Wsign-compare`: int -1
is compared with unsigned 1 after an unsigned conversion. Its explicit guarded
comparison is warning-free. The deliberate brace-narrowing example is rejected
with `-Wnarrowing` under the additional `-pedantic-errors` flag. It is an expected
compile-time error, not an unexecuted required working program.

The final operational sources compile without warnings. Warnings alone did not
catch the Task7 reporting-order issue: a runtime exact-output test detected it,
and separating deduction from printing resolved it. Task4 and Task5 predictions
match actual output exactly; their deliberate corrected/alternative versions are
explained separately. Numeric accumulation is the measured expectation-versus-
execution difference in this task.

## Complexity and measurements

Every scalar addition is O(1) time and each accumulator is O(1) space. Processing
n fares is O(n) time. Long long/double do not change that complexity; their
constants and numerical semantics differ. The benchmark times the integer and
double loops separately with steady_clock and volatile accumulators so the
compiler cannot replace the whole accumulation with a constant. Modulo indexing,
volatile reads/writes and loop overhead are included. It is one local run, not
a controlled repeated statistical benchmark or an end-to-end gate throughput
test. The totals and type limits justify the correctness recommendation; tiny
timing differences do not justify abandoning exact money.
