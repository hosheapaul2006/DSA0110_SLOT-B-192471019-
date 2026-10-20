# Task 3 - Data types and type compatibility

## Chosen representations

| System value | Type | Reason |
| --- | --- | --- |
| Fare, balance, daily collection | `long long` paise (`Money`) | Exact decimal currency at paisa resolution; daily collections exceed 32-bit paise. |
| Peak flag, payment result | `bool` | Only true/false states. |
| Display character | `char` | One character, such as a separator; not a money type. |
| Hour/minute | `int` | Small range, validated before use; permits detection of negative input. |
| Zone | `enum class Zone` | Scoped, typed identifiers; explicitly validated on external boundaries. |
| Journey count | `long long` | More than sufficient for 3,000,000 and future accumulation. |
| Container index/size | `std::size_t` (unsigned) | Matches array indexing after validation; avoid comparing it directly with negative input. |
| Precision demonstration | `float`, `double` | Measured sizes/ranges; floating currency is examined, not used for card balances. |
| Inferred count | `auto journeys = 3000000LL` | `auto` is deduction, not a new numeric type; here it is long long. |

The executed program prints `sizeof`, lowest and maximum values for all requested
types. Do not assume every platform has these sizes: the measured compiler gives
bool/char 1 byte, int/unsigned/float 4 bytes, long long/double 8 bytes. Char's range
is printed as integers so it is not confused with character glyphs. Floating
`lowest()` is the most negative finite value; `min()` would mean the smallest
positive normal value and would be misleading here. Actual ranges are in the
captured output, including the values from `<climits>`.

## Compatibility and conversions

In `10 + 0.5 + true`, the int is converted to double and the bool contributes 1,
converted to double for addition. The result is double 11.5. Integral promotions
turn many bool/char operands into int; the usual arithmetic conversions then
choose a common arithmetic type. Implicit floating-to-integer conversion drops
the fractional part and can be invalid if outside the destination range.
Explicit braces and validated casts make intention visible (Task4).

## Precision and overflow experiment

At the maximum baseline peak fare of 3600 paise, 3,000,000 journeys collect
10,800,000,000 paise. A measured 32-bit int has INT_MAX 2,147,483,647, so it is
insufficient; long long is sufficient. The first unsafe 3600-paise addition is
number 596,524. In whole rupees, 108,000,000 fits int, but mixing units would be a
serious error. The program detects the mathematical limit without executing
undefined signed overflow.

Ordinary Rs10/Rs20/Rs30 fares and their whole-rupee peak prices are exactly
representable as binary double at these totals. Therefore the program also adds
Rs36 and reports zero error: the assignment's suspected rounding problem must
not be invented when inputs do not exhibit it. A separately labelled diagnostic
fare of Rs10.10 (1010 paise, not an official zone fare) exposes representation
error at n = 20,000; 500,000; 3,000,000. Its actual double total is compared with
the exact integer-paise total using long double for the difference. Error need
not grow monotonically or with a fixed sign; repeated rounding depends on the
running sum. Task10 repeats the comparison with an actual concessional fare
mixture rather than only this diagnostic value.

## Alternative and complexity

With double rupees the loop is `total += fareRupees`; with integer paise it is
`total += farePaise`. Each addition takes O(1) time and a whole run O(n) time;
each accumulator uses O(1) space. Both long long and double occupy 8 bytes here,
but that is an observation, not a universal C++ guarantee. Integer overflow must
still be prevented; exact representation does not mean unlimited range. For
bounded fare inputs and these journey counts, long long paise avoids both the
observed floating accumulation error and the 32-bit collection overflow.
