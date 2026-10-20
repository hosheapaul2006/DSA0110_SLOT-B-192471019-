# Task 6 - Design the control structures

`pseudocode.txt` was committed before the Task6 C++ source was written. It states
inputs, outputs, validation, zone selection, fare computation, affordability and
daily iteration. It uses conventional structured notation; the separate SIMATS
standard was unavailable, as agreed before implementation.

## Complete the Code

`complete.cpp` retains the supplied headers, enum, constants, main and printing
logic. Only the two TODO regions are completed. TODO1 uses switch, break and
integer paise. Its default rejects an invalid enum with -1; TODO2 rejects negative
fares and guards subtraction. For valid inputs the switch and surcharge produce
1000, 3600, 2000 paise for the given sequence. Starting at 10000 paise yields
9000 then 5400; the last fare is denied because only 3400 would remain.

The exact required output is compared by the runner, including spacing:

```text
Fare Rs 10.00  balance Rs 90
Fare Rs 36.00  balance Rs 54
Denied (low balance)  balance Rs 54
```

The original program has only a bool peak field and no hour field. To respect
the instruction not to alter its main, the additional 10:00 and 20:00 journeys
are executed in `boundaries.cpp`. Both use Outer, an initial balance of Rs80,
off-peak fare Rs30, and a successful final balance of exactly Rs50. This separates
the mandated unchanged example from the required additional boundary evidence.

## Why each control structure is used

Use if/else for validation and mutually exclusive allow/deny actions. A switch
clearly expresses the three named zone cases and prevents accidental fallthrough
with break (or a direct return). With no matching case and no default, control
continues after the switch; a function must still return a valid result. Here
the invalid-zone branch prevents an uninitialised or arbitrary fare. The
conditional operator is suitable for the small peak multiplier selection; avoid
nested side-effecting conditionals for payment.

A range-based for is simplest for a known batch of trips, because it avoids
indexing mistakes and directly supplies each trip. An indexed for is useful
when a row number is required for reports/tests. A while loop is suitable for
streaming input until EOF, not for endlessly retrying EOF. `continue` skips a
validated-invalid input without changing balance; `break` can end a stream on
a user stop command or exit a switch. Neither should silently skip a payment
failure that must be recorded. One day with n journeys uses O(n) time and O(1)
processing space when streamed; storing all journeys first uses O(n) memory.

## Array alternative and complexity

`FareTable::base()` checks the index then directly reads the array; the boundary
program compares all three array results with a switch using the same constants.
For fixed zones both are O(1) per lookup. A compiler may emit comparisons or a
jump table for switch, so there is no universal speed winner. A three-entry
long long array occupies 24 bytes on this compiler. With z configurable zones,
the table requires O(z) storage but lookup remains O(1) after range validation.
Adding Airport requires an enum entry plus a table entry; switch implementations
also need a new case at every switch location. Task12 avoids those repeated
price branches by modifying the design toward one checked table.
