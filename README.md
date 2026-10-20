# Assignment 1 - Metro Smart-Card Fare System

**Student:** D Hoshea Paul  
**Register number:** 192471019  
**Course:** DSA0110, Slot B  
**Repository:** https://github.com/hosheapaul2006/DSA0110_SLOT-B-192471019-

All **12 tasks are complete**. Each Task01-Task12 folder contains C++ source,
input fixtures, build diagnostics, captured actual output, an output screenshot
and a written explanation with the required alternative/complexity comparison.

**Report:** [Assignment1_Report_192471019.pdf](Assignment1_Report_192471019.pdf)  
**Viva preparation:** [VIVA_GUIDE.md](VIVA_GUIDE.md)  
**Sources and provenance:** [SOURCES.md](SOURCES.md)

## Compiler and verification

- Compiler: **g++.exe (tdm64-1) 4.9.2** on Windows, 64-bit target.
- Compiler location used: `C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\g++.exe`.
- Language/build flags: **`-std=c++14 -Wall -Wextra -pedantic -O2`**.
  C++14 is supported by the installed compiler; the PDF's C++17 command is an
  example, not a mandatory language version.
- 19 C++ source files: 18 working executables and one deliberately rejected
  brace-narrowing diagnostic. All 18 working programs were executed, across
  **27 execution cases**; the diagnostic makes **28 verification cases**.
- **52/52 baseline checks** and **37/37 extension checks** pass. Additional
  exact-output comparisons check predictions, examples and input recovery.
- Only the original required Task4 program has an intentional signedness
  warning. The narrowing diagnostic is an expected compile rejection. All
  operational examples compile without warnings.

From the repository root, with Python 3 and g++ available:

```powershell
python scripts/verify.py
python scripts/verify.py --task 9
python scripts/verify.py --compiler "C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\g++.exe"
```

This rebuilds every program and checks outputs/exit statuses without overwriting
the original recorded evidence. It writes `build/verification.json`. The wrapper
also adds the compiler's directory to the child process PATH for MinGW DLLs.

## Exact compilation and manual execution

Create `build` first, then compile each source independently; never link all
the files with multiple `main()` functions into one program.

```powershell
New-Item -ItemType Directory -Force build
g++ -std=c++14 -Wall -Wextra -pedantic -O2 Task09/main.cpp -o build/Task09_main.exe
.\build\Task09_main.exe
g++ -std=c++14 -Wall -Wextra -pedantic -O2 Task08/main.cpp -o build/Task08_main.exe
Get-Content Task08/input.txt | & .\build\Task08_main.exe
g++ -std=c++14 -Wall -Wextra -pedantic -O2 Task11/config.cpp -o build/Task11_config.exe
Push-Location Task11
..\build\Task11_config.exe fares.txt
..\build\Task11_config.exe revised_fares.txt
Pop-Location
```

For every other source use the same flags and the file paths in the index below.
The executable name is `TaskNN_<source-stem>.exe`. All programs except Task8 use
an empty stdin fixture. Task11 takes a configuration filename argument and must
run with Task11 as its working directory. On Linux/macOS omit `.exe`; use
`./build/Task08_main < Task08/input.txt` for redirected input.

The intentional diagnostic is checked separately and must fail compilation:

```powershell
g++ -pedantic-errors -std=c++14 -Wall -Wextra -pedantic -O2 Task04/narrowing_fail.cpp -o build/Task04_narrowing_fail.exe
```

## Task index and results

| Task | Source files | Summary/result |
| --- | --- | --- |
| [01](Task01/README.md) | main.cpp | Justified objects, UML, invariants; accepted/denied payment demo. |
| [02](Task02/README.md) | main.cpp | Token examples, constants and safe scoped enums. |
| [03](Task03/README.md) | main.cpp | Measured sizes/ranges, precision errors and int capacity. |
| [04](Task04/README.md) | predict.cpp, corrected.cpp, narrowing_fail.cpp | Prediction matches; explicit fixes and expected compile rejection. |
| [05](Task05/README.md) | predict.cpp, expressions.cpp | Prediction matches; 24/24 peak lookup entries agree. |
| [06](Task06/README.md) | complete.cpp, boundaries.cpp | Required TODO skeleton output matches; 10:00/20:00 off-peak. |
| [07](Task07/README.md) | procedural.cpp, object_oriented.cpp, public_balance.cpp | Working styles agree; public-field invariant violation demonstrated. |
| [08](Task08/README.md) | main.cpp, getline_stoi.cpp | Invalid-input recovery, exact reserve, denial, EOF, strict stoi parsing. |
| [09](Task09/README.md) | main.cpp | 52/52 checks; five-journey total Rs103.60, balance Rs96.40. |
| [10](Task10/README.md) | main.cpp | Measured 20k/500k/3m totals; exact network collection 6,888,000,000 paise. |
| [11](Task11/README.md) | config.cpp | Evidence-based recommendation; valid/revised configs and five rejections. |
| [12](Task12/README.md) | main.cpp | Modified design; 37/37 Airport, weekend, pass and calendar checks. |

Each `runs.json` lists every source, input fixture, runtime argument, expected
output/fragment and expected exit code. Each `execution.json` records actual
stdout, timestamps, diagnostics and hashes. Task9/Task12 contain full expected-
versus-actual CSV test tables; Task10 contains CSV measurements and observations.
`evidence.html` displays these captured outputs and `output.png` is its browser
screenshot. Screenshots are rendered real transcripts, not native terminal
captures. Plain text is retained so evidence is searchable and reproducible.

## Progressive commit evidence

Each task has its own descriptive commit. Task4/Task5 predictions were committed
before those programs were executed; Task6 pseudocode was committed before its
C++ implementation. Task10 hand totals were recorded before measurement. Actual
timestamps are retained; no history is backdated. See `git log --oneline` and
the prediction commit recorded in the corresponding execution metadata.

## Agreed assumptions

- Student concession: 10%; senior concession: 20%; one passenger category per trip.
- Apply the concession after the applicable fare adjustment and round once to the
  nearest paisa, with half-paisa ties rounded up.
- Peak intervals: [08:00, 10:00) and [17:00, 20:00).
- In Task12, the weekend reduction replaces the peak surcharge. A pass covers a
  calendar month and all traversed zones must lie in its two selected zones.
- The Rs 50 minimum remains necessary even for a zero-fare pass journey.
- Use conventional structured pseudocode. The exact institutional style guide was
  not supplied, so conformance to that separate guide is not claimed.

These concessions, rounding and pass details are documented assumptions, not
additional facts from the assignment PDF. Sources and evidence are included in
the final report. Code and explanations were prepared with Codex assistance;
the student should review, execute and understand them before the viva.
