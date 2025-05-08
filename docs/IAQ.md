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

## IAQ3

| CO (ppm)    | AQI Min | AQI Max | Qualità dell'aria       |
| ----------- | ------- | ------- | ----------------------- |
| 0.0 – 4.4   | 0       | 50      | Buona                   |
| 4.5 – 9.4   | 51      | 100     | Moderata                |
| 9.5 – 12.4  | 101     | 150     | Insalubre per sensibili |
| 12.5 – 15.4 | 151     | 200     | Insalubre               |
| 15.5 – 30.4 | 201     | 300     | Molto insalubre         |
| 30.5 – 40.4 | 301     | 400     | Pericolosa              |
| 40.5 – 50.4 | 401     | 500     | Pericolosa              |

| CO₂ (ppm)   | IAQI (non ufficiale) | Qualità dell'aria       |
| ----------- | -------------------- | ----------------------- |
| 250 – 600   | 0 – 50               | Eccellente              |
| 601 – 1000  | 51 – 100             | Buona                   |
| 1001 – 1500 | 101 – 150            | Discreta (sonnolenza)   |
| 1501 – 2000 | 151 – 200            | Scarsa                  |
| 2001 – 5000 | 201 – 300            | Cattiva / soglia OSHA   |
| >5000       | >300                 | Pericolosa (prolungata) |

| Etanolo (ppm) | IAQI (stimato) | Commento                    |
| ------------- | -------------- | --------------------------- |
| 0 – 200       | 0 – 50         | Normale (es. disinfettanti) |
| 201 – 400     | 51 – 100       | Odore percepibile           |
| 401 – 800     | 101 – 150      | Possibile irritazione       |
| 801 – 1000    | 151 – 200      | Irritante                   |
| 1001 – 2000   | 201 – 300      | Mal di testa, sonnolenza    |
| >2000         | >300           | Rischio tossico             |

| Toluene (ppm) | IAQI (stimato) | Commento                         |
| ------------- | -------------- | -------------------------------- |
| 0 – 0.1       | 0 – 50         | Tipico fondo ambientale          |
| 0.1 – 0.3     | 51 – 100       | Percepibile                      |
| 0.3 – 1       | 101 – 150      | Lieve irritazione possibile      |
| 1 – 10        | 151 – 200      | Irritazione, effetti neurologici |
| 10 – 50       | 201 – 300      | Tossicità cronica                |
| >50           | >300           | Pericoloso                       |

| NH₃ (ppm)   | IAQI (stimato) | Commento                   |
| ----------- | -------------- | -------------------------- |
| 0 – 0.25    | 0 – 50         | Tipico in casa             |
| 0.26 – 1.0  | 51 – 100       | Odore percettibile         |
| 1.1 – 5.0   | 101 – 150      | Irritante per occhi e naso |
| 5.1 – 10.0  | 151 – 200      | Disagio respiratorio       |
| 10.1 – 25.0 | 201 – 300      | Effetti gravi              |
| >25         | >300           | Tossico acuto              |

| Acetone (ppm) | IAQI (stimato) | Commento                  |
| ------------- | -------------- | ------------------------- |
| 0 – 5         | 0 – 50         | Tipico indoor (tracce)    |
| 6 – 20        | 51 – 100       | Odore percettibile        |
| 21 – 100      | 101 – 150      | Mal di testa, irritazione |
| 101 – 200     | 151 – 200      | Effetti neurologici lievi |
| 201 – 500     | 201 – 300      | Sonno, nausea             |
| >500          | >300           | Tossicità acuta           |

=> l'inquinante peggiore determina l'indice totale!
