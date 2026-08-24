Cold-Gas Nozzle Testbed
My Goal
Design, build, and instrument a cold-gas thrust stand to determine whether an aerospike nozzle outperforms a conventional bell nozzle at off-design pressure ratios
Software and Skills
SOLIDWORKS
Python
Arduino / Embedded C++
Fused Deposition 3D Printing
Sensor Instrumentation and Calibration
Soldering and Circuit Assembly
Compressible Flow Analysis
Constraints
All nozzles share a common throat area of 12.6 mm² and expansion ratio of 4 so that geometry is the only variable
Cold gas only — no combustion, isolating aerodynamic performance from thermal effects
All components fabricated or assembled from commercially available parts
Test conditions limited to 40, 80, and 120 psi regulator settings
Nozzle Design

Three geometries were designed to identical throat area and expansion ratio, differing only in the diverging section. Conical, bell, and aerospike were fabricated in PLA on an AnkerMake M5C. A truncated plug geometry was designed but not tested.

The aerospike spike contour was generated using a Python implementation of Angelino's approximate method for axisymmetric plug nozzles. For CO₂ at γ = 1.29 and ε = 4, this produces:

Parameter	Value
Design exit Mach number	2.758
Cowl lip radius	4.000 mm
Throat flow angle	50.6° inward from axis
Annular throat gap	0.522 mm
Spike length	10.69 mm

Nozzle Geometries

<!-- ![Nozzle geometries](images/nozzles.jpg) -->

Aerospike CAD Model

<!-- ![Aerospike](images/aerospike_cad.jpg) -->
Test Stand

The thruster fires vertically upward, aligning the thrust vector with the load cell's sensitive axis and allowing the static weight of the plenum assembly to be removed by taring before each run.

Plenum epoxied into a hexagonal recess in a 3D-printed upper plate
Single-point load cell mounted between upper and lower plates
Standoffs at each mounting point preserving beam deflection clearance
Supply hose routed with slack to prevent parasitic force transmission

Assembled Test Stand

<!-- ![Test stand](images/test_stand.jpg) -->
Gas System

CO₂ cylinder → CGA-320 adapter → adjustable regulator → solenoid valve → plenum → nozzle

12 V normally-closed brass solenoid valve, 2.5 mm internal orifice
Driven through an IRLZ44N MOSFET with flyback diode protection
Brass tee-fitting plenum with dedicated sensor ports
Garden-hose-thread nozzle interface for tool-free geometry swaps

Plenum Assembly

<!-- ![Plenum](images/plenum.jpg) -->
Instrumentation

Six channels on an Arduino Mega 2560:

Measurement	Device
Thrust	1 kg load cell, HX711 24-bit amplifier
Propellant mass	5 kg load cell, HX711
Chamber pressure	150 psi transducer, 0.5–4.5 V output
Plenum gas temperature	K-type thermocouple, MAX31855
Cylinder temperature	K-type thermocouple, MAX31855
Ambient conditions	BME280

Both load cells were calibrated against a 370 g reference mass. Thrust resolution reached 0.021 gf per ADC count with a noise floor below 1 gf.

Breadboard and DAQ Wiring

<!-- ![Electronics](images/electronics.jpg) -->
Data Acquisition

The Arduino tares both load cells and captures the pressure transducer zero offset at startup, then waits for a fire command over serial. On command it logs a pre-fire baseline, opens the solenoid for a fixed 2-second burn, and continues logging through the blowdown decay.

Thermocouples are sampled before and after the burn rather than during, since polling them mid-burn would throttle the load cell sampling rate.

A Python script on the host receives the stream, prompts for run metadata, and writes a timestamped CSV per firing.

Results

27 test firings across three geometries at three supply pressures, three replicates each.

Condition	Replicate thrust (gf)
Bell, 80 psi	139.2, 141.0, 133.9
Aerospike, 80 psi	117.6, 107.7, 109.5
Aerospike, 120 psi	143.2, 146.9, 147.4
Within-burn scatter below 1% of the mean for the majority of runs
Thrust increased monotonically with supply pressure for every geometry
Bell produced higher thrust than aerospike at every matched condition

Representative Thrust Trace

<!-- ![Thrust trace](images/thrust_trace.png) -->

Thrust vs. Supply Pressure

<!-- ![Thrust comparison](images/thrust_comparison.png) -->
The Flow Regime Finding

Measured plenum pressure never exceeded 0.5 psi at any regulator setting, showing no correlation with supply pressure — despite thrust and propellant mass loss both confirming substantial gas flow.

The cause was isolated through controlled testing:

The pressure transducer was verified against a known reference on the bench
The solenoid was held open continuously while plenum pressure was monitored, confirming pressure stayed near ambient under sustained flow
The valve orifice measures 2.5 mm; the nozzle throat measures 4.0 mm

With an exit less restrictive than the inlet, gas passes through the plenum as fast as it is admitted and no pressure accumulates. Chamber volume affects only how long steady state takes to reach, not what that steady state is.

The nozzles were therefore operating as subsonic flow restrictions rather than de Laval nozzles. Without a choked throat there is no supersonic expansion, and the mechanism responsible for aerospike altitude compensation was never present. Measured differences between geometries reflect effective flow area and internal losses, not expansion efficiency.

This was identified using the project's own instrumentation.

Limitations
Flow regime — the system operated below choked conditions, so the data does not support conclusions about supersonic expansion or altitude compensation
Throat area verification — printed dimensions were not measured against design geometry, which matters most for the aerospike's 0.522 mm annular gap
Supply conditions — cylinder temperature dropped measurably during firing, and back-to-back runs showed reduced mass flow as the cylinder cold-soaked
Future Work
Reduce throat diameter below the 2.5 mm valve orifice to restore choked flow
Directly measure the printed aerospike annular gap
Test the truncated plug geometry
Compare measured performance against CFD
Repository Contents
├── firmware/     Arduino sketches - main DAQ plus sensor bring-up and diagnostics
├── analysis/     Python serial logger and analysis notebook
├── data/         Raw CSV output from all test firings
├── cad/          Nozzle geometry - STL and SOLIDWORKS files
└── images/       Photos of the apparatus

Each CSV contains a metadata header followed by time-series columns: elapsed milliseconds, commanded valve state, thrust in grams-force, cylinder mass in grams, and chamber pressure in psi.
