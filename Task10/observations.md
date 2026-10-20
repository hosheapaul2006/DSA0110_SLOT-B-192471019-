# Measured expectations versus execution

These values come from the recorded execution on this machine.

| Journeys | Expected/actual integer paise | Actual double rupees | Difference in rupees | Checked int fits |
| --- | --- | --- | --- | --- |
| 20000 | 45920000 | 459199.999999969092 | -0.000000030908 | yes |
| 500000 | 1148000000 | 11480000.000010456890 | 0.000010456890 | yes |
| 3000000 | 6888000000 | 68879999.999236658216 | -0.000763341784 | no |

The network double error is about -0.0763 paisa in this run. The exact paise totals match the hand calculations. The integer and floating loop timings are in measurements.csv and include volatile access and loop overhead.
