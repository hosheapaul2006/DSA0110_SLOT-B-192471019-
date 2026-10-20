# Task 12 - Re-evaluate and adapt the design

## A. Impact analysis (2 marks)

The new Airport zone requires a fourth enum value and a 6000-paise fare. The
baseline's three-case switch and three-entry bounds cannot price it without
modification. The fare adjustment expression must choose weekend OR peak, because
10% weekend reduction and the 20% peak surcharge cannot combine. Monthly passes
need selected-zone data and a calendar-month validity check that a simple
passenger concession cannot represent. A single farthest-zone field is not
enough to decide pass coverage; journeys now carry origin and destination zones.

`include/metro_extended.hpp` adds a four-entry enum/table, Date, a route Journey,
MonthlyPass and FareRules. It reuses baseline Money, roundHalfUp, concession
rates, peak checks and SmartCard. The original three fares are referenced from
the baseline constexpr array rather than retyped. Task6's required skeleton and
the baseline are preserved as the evaluated original version; the extension is
explicitly separate rather than silently rewriting previous evidence.

## B. Evaluation of the original design (2 marks)

The original switch could absorb Airport with a fourth case, but every copy of
that selection would need auditing. It cannot alone express pass entitlement or
weekend priority without mixing several concerns. Adding a public pass flag to
SmartCard would also be insufficient: a flag cannot encode two zones, expiry or
route coverage. Conversely, private balance and checked deduct already accept
any nonnegative fare, including zero, so they do not need redesign.

Worked comparisons: Airport weekday at 08:00 is 6000 * 120/100 = 7200 paise;
Airport Saturday at 08:00 is 6000 * 90/100 = 5400, not 6480 from incorrectly
combining weekend and peak. Student Airport Saturday is 6000 * 90/100 * 90/100 =
4860. A valid Inner/Middle pass yields zero for that fully covered route, while
Middle/Outer must pay its normal fare. A pass selecting Inner/Outer does not
cover the intervening Middle zone on an Inner-to-Outer route. These examples
identify exactly which assumptions the earlier single-zone design could not
represent.

## C. Design decision and implementation (2 marks)

**MODIFY** the design: retain the exact-money/card core, use a checked table for
four-zone pricing, and separate monthly entitlement from fare adjustments.

```text
INPUT journey, monthlyPass, card
VALIDATE route zones, actual calendar date, time, passenger category
IF pass month/year match AND every traversed zone is selected THEN
    fare <- 0
ELSE
    base <- fareTable[MAX(originZone, destinationZone)]
    IF calendar date is Saturday or Sunday THEN adjustment <- 90
    ELSE IF peak time THEN adjustment <- 120
    ELSE adjustment <- 100
    END IF
    fare <- ROUND_HALF_UP(base * adjustment * (100-concession) / 10000)
END IF
CALL card.deduct(fare)  // retains Rs50 minimum, even for a pass
```

The route model assumes zones form the ordered corridor Inner-Middle-Outer-
Airport; all intermediate zones are traversed. This is documented, not claimed
as a full city railway graph. A pass has exactly two distinct selected zones,
valid from the first through last day of its stated calendar month/year. Outside
coverage or validity it falls back to ordinary pricing. Weekend status is
derived from a validated Gregorian date, so an arbitrary caller Boolean cannot
misclassify a weekday. A covered pass takes priority over discounts/surcharges;
the Rs50 reserve remains mandatory. No pass purchase price was supplied, so
purchase/payment for the pass is outside the prototype.

## D. Evidence-based defence (2 marks)

`main.cpp` prints expected and actual values for Airport regular/peak/weekend,
student and senior prices, pass coverage, both month boundaries, expiry, wrong
year, uncovered routes, reverse routes, same-zone travel, and retained baseline
fares. It also tests exact/insufficient reserve, an unchanged denied balance,
1000 consecutive zero-fare pass journeys, leap-year validity and invalid route/
time/date/pass inputs. The full executed comparison table is `test_results.csv`.
Task9's original suite is retained so new evidence does not erase baseline tests.

The separate policy makes revision easier without weakening payment. Table
lookup, calendar checking and two-zone membership are O(1) for the fixed four
zones; a route coverage loop visits at most four entries. For a general ordered
network with z zones and k traversed zones, pricing lookup remains O(1), the table
occupies O(z), and coverage checking is O(k); membership in the two-zone pass is
O(1). A real graph would add route discovery cost. Per-card/pass state remains
O(1), and n journeys still take O(n) for this fixed model. Additional date/zone
fields cost fixed storage; no lifetime trip history is stored for an unlimited
pass. The extension is more complex than the original switch, but the tests
show that complexity corresponds to required new policy rather than a wholesale
rewrite of proven balance handling.
