# Task 5 - Expressions, precedence and associativity

## Prediction before execution

The original `predict.cpp` and this exact `prediction.txt` are committed before
running. The runner enforces that order and records the comparison.

```text
11 16
9 6 4
6 4
3 1 -3 -1
18
```

## Operator-by-operator explanation

1. Multiplication binds more tightly than addition: 5 + (3 * 2) is 11. Parentheses
   force (5 + 3) * 2 to be 16. Insertion operators group left-to-right to print
   the values and separators.
2. Postfix `x++` supplies the old value 5 and increments x to 6; prefix `++y`
   increments y to 4 and supplies 4. Thus z = 9 and the next line prints 9 6 4.
   Precedence describes grouping, not general evaluation order. These two
   operands modify different variables, so their relative evaluation order does
   not change this result.
3. `x > y` is evaluated for the conditional operator, choosing x = 6. `%` and `*`
   have the same precedence and associate left-to-right: (6 % 4) * 2 = 4. The
   parentheses around the whole conditional avoid accidental grouping with `<<`.
4. Unary minus binds before `/` or `%`. Integer division truncates toward zero:
   10/3 = 3 and -10/3 = -3. The corresponding remainders are 1 and -1, satisfying
   a = (a/b)*b + a%b. A nonzero remainder has the dividend's sign, not necessarily
   the divisor's. Division by zero and INT_MIN/-1 are excluded from valid inputs.
5. i takes 0, 3, 6, 9; `k += i` accumulates 18. Relational `<` forms the loop
   condition. Compound assignment groups right-to-left and updates its left
   operand once; it is not a comparison. The loop body and increment execute in
   the for-loop's defined order.

`x++` is convenient when the old value is required; `++x` when the new value is
required. Avoid `i = i++`: in the C++14 mode used here, the modifications are
unsequenced and the expression has undefined behaviour. In C++17 and later,
assignment sequencing changes its status, but it still assigns the old value
back and fails to express an intended increment. It is discussed, not executed.

## Readable fare expressions

```text
peak <- (hour >= 8 AND hour < 10) OR (hour >= 17 AND hour < 20)
multiplier <- peak ? 120 : 100
rate <- category == Student ? 10 : (category == Senior ? 20 : 0)
fare <- ROUND_HALF_UP(base * multiplier * (100-rate), 10000)
balance -= fare      // only after the affordability guard
```

Comparisons bind more tightly than `&&`, which binds more tightly than `||`.
Thus the unparenthesised peak expression in the assignment is equivalent, but
explicit interval parentheses make boundary review easier. Logical operators
short-circuit. `?:` has lower precedence than those logical/comparison operators
and groups right-to-left; nested conditionals require care. Multiplication and
division group left-to-right, so division is postponed until the complete
bounded numerator is formed; dividing an integer percentage first would lose
fractional information. The peak surcharge is followed by the assumed concession,
with one final rounding operation.

## Lookup alternative and complexity

`expressions.cpp` builds `bool peak[24]` and compares all entries with the
expression, then checks the four required boundary hours and rejects hour 24.
A checked table index is essential: negative/24 indices are invalid memory
accesses. Once initialised, either check is O(1) time and O(1) space for a fixed
24-hour day. The table occupies `24*sizeof(bool)`, measured as 24 bytes here;
the expression requires no table. Initialisation costs O(24). A table may use
fewer comparisons, but cache/compiler effects mean it is not automatically
faster. Minute-based schedules would need more entries or interval logic.
