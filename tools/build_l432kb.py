#!/usr/bin/env python3
"""Build driver for BlueLRS PSA l432kb targets using system arm-none-eabi toolchain."""
import os, sys

os.environ['MLRS_ST_DIR'] = '/usr'
os.environ['MLRS_GNU_DIR'] = 'bin'

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, SCRIPT_DIR)

import run_make_firmwares as rmf

rmf.GCC_DIR = '/usr/bin'

BUILD_DIR = os.path.join(SCRIPT_DIR, 'build_l432kb')
rmf.MLRS_BUILD_DIR = BUILD_DIR
os.makedirs(BUILD_DIR, exist_ok=True)

targets = [
    rmf.cTargetL432KB(
        'rx-PSA-2400-l432kb',
        'RX_PSA_2400_L432KB',
        [],
        'rx-PSA-2400-l432kb',
        'rx-PSA-2400-l432kb',
        'ux',
    ),
    rmf.cTargetL432KB(
        'tx-PSA-2400-l432kb',
        'TX_PSA_2400_L432KB',
        [],
        'tx-PSA-2400-l432kb',
        'tx-PSA-2400-l432kb',
        'ux',
    ),
]

for target in targets:
    rmf.mlrs_build_target(target, [])
