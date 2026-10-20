# Task 9 - Implement and test the complete fare logic

`include/metro.hpp` is the working baseline implementation. `main.cpp` is an
executable verification driver with independently specified expected values.
Every row prints CASE, EXPECTED, ACTUAL and PASS/FAIL; any failure gives a nonzero
exit status. `test_results.csv` is generated from that actual output, not from
assumed success. Tests use fresh cards for independent cases, then one shared
card for the five-journey sequence.

## Required normal and boundary test expectations

| Case | Fare in paise | Initial balance | Expected outcome/balance |
| --- | --- | --- | --- |
| Inner/Middle/Outer off-peak | 1000 / 2000 / 3000 | 10000 each | Allow: 9000 / 8000 / 7000 |
| Inner/Middle/Outer peak | 1200 / 2400 / 3600 | 10000 each | Allow: 8800 / 7600 / 6400 |
| Inner at 08:00 and 17:00 | 1200 | 10000 | Allow: 8800 |
| Inner at 10:00 and 20:00 | 1000 | 10000 | Allow: 9000 |
| Inner at 07:59 / 09:59 / 16:59 / 19:59 | 1000 / 1200 / 1000 / 1200 | 10000 each | Allow: 9000 / 8800 / 9000 / 8800 |
| Exact Rs50 after Inner off-peak | 1000 | 6000 | Allow: 5000 |
| One paisa less available | 1000 | 5999 | Deny: 5999 unchanged |
| Student Inner peak | 1080 | 10000 | Allow: 8920 |
| Senior Outer peak | 2880 | 10000 | Allow: 7120 |

## Invalid inputs and invariant checks

Unknown zones -1, 3 and 99, hours -1/24, minute 60 and category 99 must throw a
validation exception before pricing/payment. Negative initial balances must be
rejected. Negative/zero recharges and recharges beyond the application cap must
fail without mutation. A positive one-paisa recharge must succeed. LLONG_MAX
recharge/fare requests must be rejected without overflow. Negative fares must
not manufacture credit. A stream with letters then 8 must recover to a valid
hour, demonstrating the same input routine used by Task8.

Rounding helper tests exercise half-paisa ties up and values just below a tie,
even though the currently configured zone/discount combinations yield whole
paise. This separates testing a future pricing rule from inventing fractional
paise in the official fare table.

## Five journeys calculated before execution

Starting balance = 20000 paise (Rs200). The manually calculated fares are:

1. Inner at 11:00 regular: 1000.
2. Middle at 08:00 regular: 2000 * 120/100 = 2400.
3. Outer at 10:00 regular: 3000.
4. Inner at 17:00 student: 1000 * 120/100 * 90/100 = 1080.
5. Outer at 19:00 senior: 3000 * 120/100 * 80/100 = 2880.

Total = 1000 + 2400 + 3000 + 1080 + 2880 = **10360 paise (Rs103.60)**.
Final balance = **9640 paise (Rs96.40)**. Each fare and payment is compared with
these independent expectations, in addition to checking the final totals.

## Table-driven alternative and complexity

The ordinary fare cases already demonstrate the alternative: a table of input
records and expected fares/outcomes is processed in a range-based for loop. A
new boundary case adds one row rather than copying assertion code. Irregular
exception/state tests use separate checks because their setup differs.

For t tests with constant-size records, either repeated handwritten checks or a
table loop costs O(t) time. The stored table occupies O(t) space; a streamed test
file can reduce working storage to O(1), excluding output. Independent tests
still need explicit oracles: a loop comparing the implementation to itself
would not establish correctness. Each individual fare/payment remains O(1).
