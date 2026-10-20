# Task 4 - Control implicit conversions

## Prediction before execution

`prediction.txt` and the original `predict.cpp` are committed before this program
is executed. The runner verifies that the current committed prediction matches
the file, then records the prediction commit and SHA-256 alongside the actual
output. The exact prediction on this ASCII-compatible target is:

```text
3 3.5
C 68
1 true
s >= u
```

## Explain every line

1. `a / b` divides two ints, so truncation occurs before the result 3 converts to
   double for `c`. Casting `a` to double changes the division; `b` converts to
   double and `d` becomes 3.5. The default stream format prints 3 rather than 3.0.
2. `'A'` promotes from char to int before adding 2. The int result converts back
   to char for `ch`, giving C in this character set. `ch + 1` promotes to int and
   prints numeric 68. Numeric character codes are target-dependent; this output
   is justified by the measured ASCII-compatible environment, not universal C++.
3. Nonzero int 5 converts to true. The stream first prints its default numeric
   representation 1, then `std::boolalpha` changes subsequent bool formatting to
   true. It does not change the stored value.
4. The usual arithmetic conversions convert signed -1 to unsigned because
   unsigned int and int have equal rank. On this 32-bit target it becomes
   4,294,967,295, so `s < u` is false. The given program intentionally receives
   the signed/unsigned comparison warning under `-Wall -Wextra`.

Other conversion points include lvalue-to-rvalue reads and the array-to-pointer
conversion of string literals used by stream insertion. The baseline fare
model explicitly converts validated enums to array indices; small int percent
operands convert to Money in multiplication. `isPeak()` yields bool directly.
The I/O reader first parses long long and range-checks before a cast to hour/zone.

## Resolution and explicit alternative

`corrected.cpp` casts both division operands to double before division and checks
`s < 0` before an unsigned cast, preserving the intended mathematical comparison.
It uses brace initialisation and explicit char/bool conversions. Its expected
changes are `3.5 3.5` on the first line and `s < u` on the last line. A mere
`static_cast<unsigned>(s)` without the negative guard would explicitly preserve
the original mistake rather than fix it. An explicit cast states intent; it is
not a runtime range check.

`narrowing_fail.cpp` deliberately tries `int x{3.7};`. It is compiled with
`-pedantic-errors` in addition to the ordinary flags and must be rejected. It has
no executable and is never run. The diagnostics provide evidence that braces
catch this narrowing at compile time. Replacing it with
`int x{static_cast<int>(3.7)};` would explicitly request truncation to 3, but would
not prove a floating input lies in range. `bool{5}` likewise needs an intentional
conversion or a Boolean comparison.

## Complexity and observed comparison

All versions perform a fixed number of operations: O(1) time and O(1) storage.
Ordinary representation conversions usually compile to no extra work or a small
fixed instruction sequence; no asymptotic change occurs. Compile-time narrowing
errors prevent execution, while signedness warnings require the programmer to
correct the policy. `execution.json` records whether prediction and actual
output agree; any deliberate changes in the corrected program are explained
above and are not retroactive edits to the prediction.
