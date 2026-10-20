# Task 8 - Read and print data safely

## Safe input design

`main.cpp` uses the shared `readInteger()` routine with `std::cin`. It performs
formatted extraction and checks `in.fail()` explicitly. On letters or an
unrepresentable integer, it clears the error state and uses
`ignore(numeric_limits<streamsize>::max(), '\n')` to discard the bad line before
retrying. Clearing without discarding would repeatedly encounter the same bad
token. EOF terminates with exit status 2, rather than retrying forever. Values
are range-checked as long long before the explicit cast to a zone/hour/category.

After a successful numeric prefix, the routine consumes the rest of the line and
rejects non-whitespace characters. Consequently `10junk` is not silently accepted
as 10. This is stricter than plain `cin >> hour`. Input convention is one value
per line, with balance in integer paise; arbitrary decimal currency input is not
implicitly rounded. A real user-facing kiosk could add a validated decimal-text
parser without changing the exact-money model.

## Output formatting

The journey report uses `left` and `setw` to align status/fare/balance columns.
`metro::money()` prints the whole-rupee part and a two-digit remainder using
`setw(2)` and `setfill('0')`. Each call has its own stream, so fill state does not
leak into the report. No double is used to compute or format the actual fare.
`fixed` plus `setprecision(2)` is demonstrated separately on a harmless display
value: precision changes floating output formatting, not representation or
arithmetic accuracy. `setw` applies to the next field; fill/precision flags persist.

## Test cases and expected results

| Fixture | Deliberate condition | Expected verified result |
| --- | --- | --- |
| input.txt | Letters for balance/hour; zone 9; trailing junk; minute 60 | Recover, accept Middle/Senior at 10:00; fare Rs16.00; balance Rs84.00. |
| exact_reserve.txt | Rs60, Inner at 10:00, regular | Allowed, Rs10.00 fare, exactly Rs50.00 left. |
| low_balance.txt | Rs59.99, same journey | Denied; Rs59.99 unchanged. |
| eof.txt | Immediate EOF | Clear termination, exit 2. |
| line_input.txt | stoi errors, huge integer, bad suffix, then valid 10 | Four rejection messages followed by Accepted hour 10. |

The runner checks expected fragments/exact outputs and exit statuses, rather than
regarding successful compilation as proof of recovery. Actual outputs and input
fixtures are retained for reproduction.

## getline/stoi alternative and complexity

`getline_stoi.cpp` reads each complete line, calls `stoi` inside try/catch, checks
the consumed-character index and then validates the hour. Without that index
check, `stoi("10junk")` would also accept 10. `invalid_argument` catches absent
numbers; `out_of_range` catches values outside int and invalid hour limits. A bad
line is already consumed, so no cin fail-state reset is needed for a conversion
exception; EOF still must be handled. For long long money use `stoll` rather than
`stoi` so parsing matches the chosen representation.

For a line of length L, scanning and discarding/parsing cost O(L) in either
approach. A basic direct extractor with discard can use O(1) extra program
storage, while getline stores O(L). This implementation's strict suffix check
also stores the remainder in a string, so its worst-case buffer space is O(L).
The fixed number of fare fields makes computation O(1) after parsing; input cost
depends on the characters consumed, including rejected lines.
