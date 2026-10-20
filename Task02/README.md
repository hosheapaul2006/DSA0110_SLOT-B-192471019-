# Task 2 - Tokens, keywords, identifiers and constants

## Token classification

| Kind requested in the assignment | Examples in this program | Meaning |
| --- | --- | --- |
| Keyword | `constexpr`, `const`, `enum`, `class`, `return`, `int` | Reserved language words; cannot be variable names. |
| Identifier | `BASE_PAISE`, `FareTable`, `safeZone`, `quote` | Names for data, types and functions. |
| Constant/literal | `1000`, `20`, `0`, `' '`, string literals | Values appearing in source. A named constexpr object is an identifier token referring to a constant. |
| Operator | `=`, `<<`, `==`, `::` | Initialisation syntax, stream insertion/comparison and scope qualification. |
| Punctuator | `;`, `{`, `}`, `(`, `)`, `,` | Delimit declarations, statements, scopes and argument lists. |

The exercise's five groups are pedagogical: the formal language grammar also
distinguishes literals and groups many operators with punctuators. `#define` is a
preprocessing directive, not a C++ keyword. Comments are not runtime tokens.

## Constants and zone representation

`include/metro.hpp` is the authoritative baseline set of three fares, surcharge,
concession assumptions and minimum balance. `constexpr std::array<Money,3>` gives
typed entries with a known bound. A fare revision changes this one definition;
the quote function reads it rather than repeating numeric fares in branches.
The fixed Task6 skeleton keeps its mandated constants because the task explicitly
forbids edits outside its TODOs. Demonstration/test expected values are independent
oracles, not additional operational fare definitions.

A macro is textual substitution before compilation: it has no namespace or type,
and arithmetic macros need parentheses. `const` prevents later assignment but
can be initialised from runtime input. `constexpr` requires a constant expression
and can be used by `static_assert` and compile-time computations. Prefer constexpr
for this fixed fare table; prefer validated runtime data for faculty-approved
configuration revisions (Task11).

An unscoped `enum` leaks enumerators into the enclosing scope and implicitly
converts to an integer; `LegacyInner` demonstrates that conversion. `enum class`
keeps names under `Zone::` and requires an explicit cast for indexing. It prevents
accidental comparison with an unrelated enum or integer. A cast can still create
an invalid enumeration value, so public quote methods validate their input.

Names use letters/digits/underscores, cannot start with a digit or equal a
keyword, and are case-sensitive. Avoid reserved implementation names: double
underscores and underscore-uppercase combinations, plus leading underscores at
global scope. Classes use PascalCase, variables/functions camelCase, and
constants UPPER_SNAKE_CASE consistently here.

## Macro alternative and complexity

```text
#define DEMO_PEAK_PERCENT 20
fare <- base * (100 + DEMO_PEAK_PERCENT) / 100
```

The executed demonstration compares macro, const and constexpr values. The macro
creates no object/storage by itself. A constexpr scalar often needs no storage
when its value is substituted; taking its address can require storage. The fare
array can occupy storage when indexed at runtime: 3 * sizeof(long long), measured
as 24 bytes on this compiler. Constants are not automatically all storage-free.
Typed and macro versions both have O(1) time per fare and O(1) space for this fixed
table. Changing token mechanisms does not change the algorithm's complexity.
