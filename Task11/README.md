# Task 11 - Recommend and defend the final design

## Recommendation based on experiments

Retain integer-paise Money (`long long`), scoped zone/category enums, one typed
fare table, named percentage/reserve constants, a pure quote operation and a
SmartCard with a private validated balance. Compose these responsibilities;
do not create inheritance hierarchies for every zone or passenger category.
Keep quote separate from payment so denied trips do not accidentally change
state. Validate inputs before casts, check arithmetic bounds and round the final
percentage result once. Use table-driven tests for ordinary cases and explicit
state/exception tests for invalid data.

Evidence: Task9 passes 52 independent checks, including all zone/time cases and
the hand total Rs103.60. Task7's private/public examples have the same measured
8-byte object size, while only the public representation permits direct negative
assignment. Task10 exactly collects 6,888,000,000 paise at 3,000,000 mixed journeys;
int paise fails at journey 935,316. The measured double total differs slightly,
so a compact floating representation offers no correctness benefit. Task8
demonstrates rejection and recovery rather than relying on valid input alone.

## Carrying out a fare revision

For the small fixed baseline, edit the one BASE_PAISE definition, rebuild, rerun
normal/boundary/concession tests, and deploy the tested binary. Add a fare-policy
version to a production deployment and audit the active configuration. Test
expected values must be revised independently from the policy so that tests do
not merely repeat a changed implementation. The mandated standalone Task6
skeleton remains a teaching snapshot, not a second deployed pricing service.

## Runtime configuration alternative

`config.cpp` implements a separate validated table loaded once at startup. A
configuration gives z followed by z positive paise amounts (1 <= z <= 32), with
a conservative per-fare limit preventing arithmetic overflow. An unreadable file,
bad count, missing/negative fare or trailing data fails closed. The table is fully
validated before use; a partial load is never installed as a working tariff.

The normal fixture gives Outer peak Rs36.00; a separate demonstration revision
to 1100/2200/3300 paise gives Rs39.60 without recompiling. Five invalid fixture/
file cases are executed and must exit 2. This is a concrete prototype of the
alternative, not a silent revision of the official assignment fares. It changes
base fares only; concessions, passes and reserve are still application rules.

Reading z bounded numeric values costs O(z) startup time, or O(B) when accounting
for B file characters, and O(z) storage. Once loaded, an indexed fare quote is
O(1). Compiled constexpr data avoids file parsing and load failure but requires
rebuild/redeployment after a revision. Runtime configuration reduces that cost
but adds schema, validation, version consistency and tamper-protection concerns.
For this assignment retain compiled constants; for a network with frequent fare
changes, adopt a centrally versioned, validated configuration service.

## Network correctness and limitations

The type/range evidence establishes numerical capacity for the specified daily
load. Per-journey work is O(1), so n journeys take O(n) processing time with O(1)
streaming working storage; c active cards require O(c) state. The loop timings
do not establish actual gate latency or distributed throughput. This prototype
does not persist accounts, authenticate cards, prevent simultaneous gate races,
implement secure configuration distribution, or resolve real station routes.
A production network needs transactional balance updates, durable audit logs,
fault handling and fare-policy rollout coordination. Its correctness cannot be
claimed solely from a fast local arithmetic loop.

Concession/pass conventions are disclosed assumptions requiring policy approval,
not facts asserted by the PDF. The following Task12 modifies pricing and
entitlements while retaining the tested money and card invariants.
