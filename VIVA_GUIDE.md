# Viva preparation - explain, execute, modify

Read the source and reproduce the tests before using these prompts. This guide
does not assert that a viva or the student's own independent execution occurred.

1. **Why paise?** Explain exact integer representation, then show Task3's
   fractional-double accumulation and Task10's network totals. Integer money
   still has a range limit; demonstrate the checked int overflow boundary.
2. **Why not always double?** Rs36 is exact in binary at these counts but Rs28.80
   is not. Formatting to two decimals does not fix the stored value.
3. **What happens at exactly 10 AM?** Evaluate the half-open interval and run
   Task6/boundaries.cpp. Then modify a test to 09:59 and explain the changed fare.
4. **Why is balance private?** Show Task7's public negative assignment and the
   unchanged balance on a denied private-card deduction.
5. **Why does -1 compare greater than unsigned 1?** Explain promotions/usual
   arithmetic conversions and the guarded explicit correction in Task4.
6. **Why does 7/2 assigned to double give 3?** Division happens before conversion.
   Cast an operand before division and explain the 3.5 result.
7. **Can braces catch every conversion mistake?** They reject implicit narrowing;
   an explicit cast can suppress that protection and still be wrong. Explain
   the deliberately rejected source and range validation before a cast.
8. **Does precedence specify evaluation order?** No. Use Task5's distinct x/y
   example and Task7's resolved printing-order problem to explain the difference.
9. **How do you recover after letters?** Show fail(), clear(), ignore(), strict
   suffix validation, and EOF termination in Task8. Compare getline/stoi.
10. **Why not subtract first and compare?** A huge or negative external fare could
    overflow or create credit. Explain the subtraction guard in canTravel().
11. **Modify a fare.** Edit one table entry, independently calculate new expected
    values, rebuild and run tests. Explain why changing tests to reuse quote()
    would remove the independent oracle.
12. **Why does weekend Airport peak cost Rs54?** The weekend rule replaces peak:
    6000*90/100, not 6000*120/100*90/100. Student then costs Rs48.60 under the
    disclosed concession assumption.
13. **What does a monthly pass cover?** Explain calendar month/year, two distinct
    selected zones and all traversed zones. An Inner/Outer pass cannot cover a
    route through unselected Middle. Run expiry and nonadjacent-route cases.
14. **Why retain SmartCard in Task12?** Pricing changes, but zero/nonzero checked
    deductions still satisfy the same reserve invariant. Explain the pass-at-
    Rs50 and pass-below-Rs50 test results.
15. **What would production need?** Persistent accounts, atomic updates across
    gates, authenticated cards and versioned fare deployments. Local arithmetic
    measurements alone do not prove distributed throughput or security.

Suggested practice: run `python scripts/verify.py --task 9`, then change one
boundary expectation deliberately, observe the failure, restore it and explain
the diagnosis. Keep any genuine subsequent work as new commits; do not rewrite
the recorded pre-execution predictions or invent historical work dates.
