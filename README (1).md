# Cold-Gas Nozzle Testbed

## My Goal

- **Design, build, and instrument a cold-gas thrust stand to determine whether an aerospike nozzle outperforms a conventional bell nozzle at off-design pressure ratios**

## Software and Skills

- **SOLIDWORKS**
- **Python**
- **Arduino / Embedded C++**
- **Fused Deposition 3D Printing**
- **Sensor Instrumentation and Calibration**
- **Circuit Design and Soldering**
- **Compressible Flow Analysis**

## Constraints

- **All nozzles share a common throat area of 12.6 mm² and expansion ratio of 4 so that geometry is the only variable**
- **Cold gas only — no combustion, isolating aerodynamic performance from thermal effects**
- **All components fabricated or assembled from commercially available parts**
- **Test conditions limited to 40, 80, and 120 psi regulator settings**

## Nozzle Design

Four geometries were designed to identical throat area and expansion ratio, differing only in the diverging section:

- **Conical** — 15° half-angle divergence, serving as the analytical baseline
- **Bell** — 80% Rao approximation
- **Annular aerospike** — central spike with three support struts inside an outer cowl
- **Truncated plug** — the aerospike spike cut at 35% length with a flat base

All were fabricated in PLA on an AnkerMake M5C.

The aerospike spike contour was generated using a Python implementation of **Angelino's approximate method** for axisymmetric plug nozzles. For CO₂ at γ = 1.29 and ε = 4, this produces:

| Parameter | Value |
|---|---|
| Design exit Mach number | 2.758 |
| Cowl lip radius | 4.000 mm |
| Throat flow angle | 50.6° inward from axis |
| Annular throat gap | 0.522 mm |
| Spike length | 10.69 mm |

Nozzle Geometries

<img width="400" alt="image" src="https://github.com/user-attachments/assets/b1a7a5f4-5e77-4056-aa54-ec434e921302" />
*Aerospike Nozzle*
<img width="400" alt="image" src="https://github.com/user-attachments/assets/1ba6f6ae-f07d-40cd-b524-ce9348fda91e" />
<img width="400" alt="image" src="https://github.com/user-attachments/assets/8aef1877-245a-438a-8636-42407a024cf1" />
<img width="400" alt="image" src="https://github.com/user-attachments/assets/a8d5a88e-c31d-498d-91d9-ca69a1809b7b" />




Aerospike CAD Model


<img width="676" height="624" alt="image" src="https://github.com/user-attachments/assets/706e35e9-dcda-4354-8a2e-63fe53a0589e" />


## Test Stand

The thruster fires vertically upward, aligning the thrust vector with the load cell's sensitive axis and allowing the static weight of the plenum assembly to be removed by taring before each run.

- **Plenum epoxied into a hexagonal recess in a 3D-printed upper plate**
- **Single-point load cell mounted between upper and lower plates**
- **Standoffs at each mounting point preserving beam deflection clearance**
- **Supply hose routed with slack to prevent parasitic force transmission**

Assembled Test Stand
<img width="952" height="1269" alt="image" src="https://github.com/user-attachments/assets/02592690-d3f0-4b48-abcd-3ee646376bcb" />



## Gas System

CO₂ cylinder → CGA-320 adapter → adjustable regulator → solenoid valve → plenum → nozzle

- **12 V normally-closed brass solenoid valve, 2.5 mm internal orifice**
- **Brass tee-fitting plenum with dedicated sensor ports**
- **Garden-hose-thread nozzle interface for tool-free geometry swaps**

Plenum Assembly
<img width="1242" height="2208" alt="IMG_4561" src="https://github.com/user-attachments/assets/fd7d0ab9-b3e8-4fb9-8abb-531d4dfef401" />

## Solenoid Drive Circuit

The Arduino cannot source enough current to actuate the valve directly, so a low-side MOSFET switch was built to let a 5 V logic signal control a 12 V coil drawing several hundred milliamps.

- **IRLZ44N logic-level N-channel MOSFET switching the coil ground path**
- **220 Ω series resistor limiting gate charge current from the Arduino pin**
- **10 kΩ pulldown holding the gate low during boot so the valve fails closed**
- **1N4007 flyback diode across the coil, clamping the inductive voltage spike at switch-off**
- **Separate 12 V supply sharing a common ground reference with the Arduino**

The flyback diode matters here: without it, collapsing current in the coil drives the drain node to several hundred volts and destroys the MOSFET.

Solenoid Drive Circuit
<img width="1960" height="1400" alt="solenoid_circuit" src="https://github.com/user-attachments/assets/f661c395-eea8-4da5-88a9-ba5e7492d220" />


## Instrumentation

Six sensor channels plus valve actuation on an Arduino Mega 2560:

| Measurement | Device |
|---|---|
| Thrust | 1 kg load cell, HX711 24-bit amplifier |
| Propellant mass | 5 kg load cell, HX711 |
| Chamber pressure | 150 psi transducer, 0.5–4.5 V output |
| Plenum gas temperature | K-type thermocouple, MAX31855 |
| Cylinder temperature | K-type thermocouple, MAX31855 |
| Ambient conditions | BME280 |
| Valve actuation | Solenoid via MOSFET drive circuit |

Both load cells were calibrated against a 370 g reference mass. Thrust resolution reached 0.021 gf per ADC count with a noise floor below 1 gf.

DAQ Architecture and Gas Path
<img width="2360" height="1760" alt="system_diagram" src="https://github.com/user-attachments/assets/437fafc4-2b11-4e8a-9395-00769dd37d81" />


## Data Acquisition

The Arduino tares both load cells and captures the pressure transducer zero offset at startup, then waits for a fire command over serial. On command it logs a pre-fire baseline, opens the solenoid for a fixed 2-second burn, and continues logging through the blowdown decay.

Thermocouples are sampled before and after the burn rather than during, since polling them mid-burn would throttle the load cell sampling rate.

A Python script on the host receives the stream, prompts for run metadata, and writes a timestamped CSV per firing.

## Results

**36 test firings** — four geometries, three supply pressures, three replicates each.

Mean steady-state thrust in grams-force:

| Geometry | 40 psi | 80 psi | 120 psi |
|---|---|---|---|
| Bell | 60.3 | 138.0 | 187.3 |
| Aerospike | 45.6 | 111.6 | 145.8 |
| Plug | 35.3 | 95.6 | 145.0 |
| Conical* | 29.9 | 92.9 | not resolved |

- **Bell, aerospike, and plug all showed within-burn scatter below 1% of the mean at 80 and 120 psi**
- **Thrust increased monotonically with supply pressure for every geometry**
- **Bell produced the highest thrust at every matched condition**
- **Aerospike and plug converged at 120 psi (145.8 vs 145.0 gf) despite differing by 22% at 40 psi**

***Conical exclusions.** The first replicate at each conical test condition read anomalously low (12.5, 30.5, and 5.9 gf at 40, 80, and 120 psi) with within-burn scatter reaching 43%, compared to under 1% for the other geometries. These three runs were excluded and the reported conical means are taken from replicates 2 and 3 only. All raw files remain in the repository.

Replicates 2 and 3 agreed to within 0.7% at 40 psi and 2.2% at 80 psi. At 120 psi they disagreed by 45% (103.6 and 150.4 gf), so no mean is reported for that condition. Conical testing was also conducted on a cold-soaked cylinder at 13–15 °C versus 19–21 °C for the other geometries, which reduces available supply pressure. The specific cause of the first-run anomaly was not isolated before testing concluded.

Representative Thrust Trace

<img width="1280" height="720" alt="thrust_trace (1)" src="https://github.com/user-attachments/assets/8afaa46e-a253-466b-82f6-6fe44a545474" />

Thrust vs. Supply Pressure

<img width="1200" height="800" alt="thrust_comparison" src="https://github.com/user-attachments/assets/c8288b70-0c73-4681-8853-d9abf58cd0c2" />


## The Flow Regime Finding

Measured plenum pressure never exceeded **0.5 psi** at any regulator setting, showing no correlation with supply pressure — despite thrust and propellant mass loss both confirming substantial gas flow.

The cause was isolated through controlled testing:

- **The pressure transducer was verified against a known reference on the bench**
- **The solenoid was held open continuously while plenum pressure was monitored, confirming pressure stayed near ambient under sustained flow**
- **The valve orifice measures 2.5 mm; the nozzle throat measures 4.0 mm**

With an exit less restrictive than the inlet, gas passes through the plenum as fast as it is admitted and no pressure accumulates. Chamber volume affects only how long steady state takes to reach, not what that steady state is.

**The nozzles were therefore operating as subsonic flow restrictions rather than de Laval nozzles.** Without a choked throat there is no supersonic expansion, and the mechanism responsible for aerospike altitude compensation was never present. Measured differences between geometries reflect effective flow area and internal losses, not expansion efficiency.

This was identified using the project's own instrumentation.

## Limitations

- **Flow regime** — the system operated below choked conditions, so the data does not support conclusions about supersonic expansion or altitude compensation
- **Conical repeatability** — the first replicate at each conical condition was excluded as anomalous, and the 120 psi condition remains unresolved
- **Throat area verification** — printed dimensions were not measured against design geometry, which matters most for the aerospike's 0.522 mm annular gap
- **Supply conditions** — cylinder temperature dropped measurably during firing, and back-to-back runs showed reduced mass flow as the cylinder cold-soaked

## Future Work

- **Reduce throat diameter below the 2.5 mm valve orifice to restore choked flow**
- **Directly measure the printed aerospike annular gap**
- **Repeat conical testing under controlled cylinder temperature**
- **Compare measured performance against CFD**

## Repository Contents

```
├── firmware/     Arduino sketches - main DAQ plus sensor bring-up and diagnostics
├── analysis/     Python serial logger and analysis notebook
├── data/         Raw CSV output from all 36 test firings
├── cad/          Nozzle geometry - STL and SOLIDWORKS files
└── images/       Photos of the apparatus
```

Each CSV contains a metadata header followed by time-series columns: elapsed milliseconds, commanded valve state, thrust in grams-force, cylinder mass in grams, and chamber pressure in psi.
