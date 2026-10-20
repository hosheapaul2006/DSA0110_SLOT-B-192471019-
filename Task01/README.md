# Task 1 - Understand the problem and identify the objects

## Object analysis and justification

| Object | Data | Behaviour and justification |
| --- | --- | --- |
| SmartCard | Private balance in paise | Checks and atomically deducts an affordable fare; validates recharges. One owner of balance prevents scattered checks and accidental negative values. |
| Passenger | Name and category | Supplies eligibility for a regular, student or senior fare. It is separate from the card because eligibility belongs to the person. |
| Journey | Farthest zone, hour, minute, category | Describes one fare request. A value record keeps transaction inputs together without owning the balance. |
| Zone | A scoped enumeration | Identifies a permitted network region. It is a value, not a class with an unnecessary mutable fare. |
| FareTable | Shared constexpr fare data | Validates a zone and computes a quote without changing the card. Separating pricing from payment permits independent testing. |

The UML source is `objects.mmd`; the report includes a rendered class diagram.
Association: a passenger uses a card, a journey supplies a passenger category and
zone, and the fare table produces the amount the card attempts to deduct.

## OOP principles

Encapsulation is the private `SmartCard::balance_` and checked public methods.
Abstraction is `quote()` and `deduct()`: callers request pricing/payment without
manipulating percentage arithmetic or stored balances. Inheritance could express
a common `FarePolicy` interface with regular and pass policy implementations.
Polymorphism could select a policy through that interface. The small baseline
uses enum categories and composition instead: three data-driven rates do not
justify a passenger subclass hierarchy. These are reasoned possible uses, not a
claim that every OOP principle must be forced into the baseline.

## Rules and assumptions

Money is nonnegative integer paise. A valid journey leaves at least 5000 paise;
a denied transaction leaves the balance unchanged. Only known zones and valid
24-hour times are accepted. Peak is [08:00,10:00) or [17:00,20:00), so 10 AM and
8 PM are off-peak. One category applies per journey; assumed student/senior
concessions are 10%/20%. Fare calculations round only the final nonnegative
fraction to the nearest paisa, ties up. Recharges must be positive and remain
within the documented application balance limit of 10^12 paise. This limit also
makes arithmetic bounds explicit. The problem prices by farthest zone, not
distance; real station routing is outside this baseline.

## Procedural alternative and complexity

```text
GLOBAL balance, fareTable, minimumBalance
FUNCTION deduct(fare)
    IF fare < 0 OR balance < minimumBalance THEN RETURN false
    IF fare > balance - minimumBalance THEN RETURN false
    balance <- balance - fare
    RETURN true
END FUNCTION
```

Procedural code can also centralise a fare constant, so OOP is not automatically
faster or cheaper to maintain. With global mutable balance, any function can
bypass the guard; with the class, all writes pass through `deduct()` or
`recharge()`. A minimum-balance revision changes one constant in either carefully
factored design. An unstructured procedural design with k copied checks needs
O(k) edits, whereas this design has one policy definition. A fare quote and
payment are O(1) time and O(1) extra space. Each card stores one `long long`,
independent of the number of journeys; n cards occupy O(n) storage. The fare
table is shared rather than copied into every card.

## Verification

`main.cpp` executes the object collaboration: Rs 100 pays an Outer peak fare of
Rs 36, leaving Rs 64. A subsequent Rs 30 deduction is denied and leaves Rs 64.
The program checks these invariants and returns nonzero on failure. Actual
output and build diagnostics are recorded beside the source.
