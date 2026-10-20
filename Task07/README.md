# Task 7 - From procedural to object-oriented

## Two working versions

`procedural.cpp` keeps a separate global balance and free `canTravel()` and
`deduct()` functions. `object_oriented.cpp` uses the shared SmartCard, whose
balance is private. Both execute the same four requests: 1000, 3600, 2000 and -1
paise from an initial 10000. The expected/actual sequence is accepted, accepted,
denied, denied, ending at 5400 paise. The runner checks exact matching output
for the two versions, including the measured 8-byte balance storage.

The initial procedural test caught an output-order bug: a single C++14 stream
expression both called deduct and read the balance. Evaluation order allowed the
old balance to be formatted first. Payment now occurs in its own statement before
printing, and the same correction is applied to the OO version. The final exact
output checks verify the corrected reports. This illustrates why side effects
and observations should be separated even when the numeric payment logic is sound.

## How the class protects the balance

`canTravel()` first rejects a negative fare and a balance below the reserve, then
compares fare with `balance - minimum`. This avoids overflowing an expression
such as `minimum + fare` or subtracting an arbitrary huge fare. `deduct()` calls
the guard itself, so callers cannot forget to check first. On denial it changes
nothing. `recharge()` rejects nonpositive values and tests the upper bound using
`amount <= maximum - balance` before addition. Constructor validation prevents
negative or over-limit initial balances. A card may start below Rs50 and be
recharged, but cannot travel until the reserve is satisfied.

Private storage prevents ordinary external code from directly assigning a
negative balance; access control is a compile-time boundary. This is not a claim
of protection against memory corruption or concurrent unsynchronised writes.
The gate prototype is single-threaded. A real shared account service would need
atomic transactions/locking and persistent audit records.

## Which design changes more easily?

Both well-factored versions use the same MIN_BALANCE_PAISE constant. A reserve
revision therefore takes one policy edit in either version, and both need the
same tests rerun. The OO advantage is that clients cannot legally bypass the
policy by changing the field directly. The global procedural version exposes
balance to all code in its translation unit and couples callers to that shared
state. A procedural version with opaque state and a carefully controlled API
could obtain many of the same protections; OOP is not a substitute for good
factoring. Multiple card objects naturally hold independent balances, whereas
the single-global example would need additional parameters or containers.

## Public-member alternative

`public_balance.cpp` deliberately uses `struct PublicCard { long long balance; }`
and assigns -1. It compiles and runs: the language allows the write even though
the application rule is broken. The output records both the broken value and
the public/private object sizes. Exposing a balance would allow any caller to
evade the Rs50 reserve, manufacture credit or subtract a negative fare.

Both private and public representations store one long long and have no virtual
methods, so they have the same measured storage here (8 bytes). The standard
does not promise every independently designed class layout will be identical.
Both guarded deduct functions take O(1) time and O(1) extra space; access control
does not add a per-operation algorithmic cost. Inlining can eliminate wrapper
call overhead. A public field is therefore not a justified performance trade-off
for abandoning the central balance invariant.
