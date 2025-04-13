# IAQ (Indoor Air Quality)

IAQ normalised from 0 to 100 (where 0 is bad and 100 is very good).

## IAQv1

> IAQ = 100 − (w₁⋅G + w₂⋅∣H − θ∣ + w₃⋅∣T − λ∣)

Where

- G: gas from MQ-135
- H: relative humidity from DHT11 [%]
- T: temperature from DHT11 [°C]
- w₁, w₂, w₃: empirical coefficients to adjust the influence of parameters
- θ: ideal RH [%]
- λ: ideal Temperature [°C]

| w₁   | w₂   | w₃   | θ   | λ   |
| ---- | ---- | ---- | --- | --- |
| 0.05 | 0.50 | 1.00 | 50  | 22  |

## IAQv2

This version use [MQ135 calibration](./MQ135.md#calibration)

> IAQ = 100 − (w₁⋅$f(ρ)$ + w₂⋅∣H − θ∣ + w₃⋅∣T − λ∣)

Where

- H: relative humidity from DHT11 [%]
- T: temperature from DHT11 [°C]
- ρ: estimate CO₂
- $f(ρ)$: penalisation based on ppm read
- w₁, w₂, w₃: empirical coefficients to adjust the influence of parameters
- θ: ideal RH [%]
- λ: ideal Temperature [°C]

| w₁   | w₂   | w₃   | θ   | λ   |
| ---- | ---- | ---- | --- | --- |
| 0.05 | 0.50 | 1.00 | 50  | 22  |

```cpp
// ideal CO₂ should be under 880 ppm
float f(float ppm) {
  if (ppm < 800) {
    return 0;
  }
  if (ppm < 1500) {
    return (ppm - 800) * 0.01;
  }
  return (ppm - 800) * 0.02;
}
```

## Understand values

| IAQ Score | Air Quality | Suggested Action        |
| --------- | ----------- | ----------------------- |
| 80 - 100  | Very good   | Nothing                 |
| 60 - 79   | Good        | Light ventilation       |
| 40 - 59   | Acceptable  | Opening windows         |
| 20 - 39   | Poor        | Increasing ventilation  |
| 0 - 19    | Very poor   | Alarm / active purifier |
