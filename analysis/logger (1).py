"""
Serial logger for the cold-gas nozzle testbed.

Connects to the Arduino running daq_main.ino, waits for the tare sequence
to complete, triggers a firing, and writes the returned CSV stream to disk.

Filenames are generated from the nozzle geometry, regulator setting, and
run number entered at the prompts.

Requires: pyserial  (pip install pyserial)
"""

import serial
import time
import csv

PORT = 'COM7'
BAUD = 115200
OUT_DIR = r"C:\Documents_nozzledata"

nozzle = input("Nozzle: ").strip()
psi = input("Pressure (psi): ").strip()
run = input("Run number: ").strip()

fname = rf"{OUT_DIR}\{nozzle}_{psi}psi_run{run}.csv"

ser = serial.Serial(PORT, BAUD, timeout=2)
time.sleep(2)

# Wait for the Arduino to finish taring and signal it is ready
print("Waiting for tare...")
meta = []
while True:
    line = ser.readline().decode(errors='ignore').strip()
    if not line:
        continue
    print(line)
    if line.startswith('#'):
        meta.append(line)
    if 'send f to fire' in line:
        break

input("Press ENTER to fire...")
ser.write(b'f')

# Read the burn data until the Arduino reports it is done
rows = []
header = None
while True:
    line = ser.readline().decode(errors='ignore').strip()
    if not line:
        continue
    if line.startswith('#'):
        meta.append(line)
        if 'done' in line:
            break
    elif header is None:
        header = line.split(',')
    else:
        rows.append(line.split(','))

ser.close()

# Metadata lines are written first, then the header and data rows
with open(fname, 'w', newline='') as f:
    w = csv.writer(f)
    for m in meta:
        w.writerow([m])
    w.writerow(header)
    w.writerows(rows)

print(f"Saved {fname} - {len(rows)} samples")
