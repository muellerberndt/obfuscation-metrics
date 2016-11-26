#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import bz2
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('files', metavar='input_file', type=argparse.FileType('r'), nargs='+', help='List of files')
args = parser.parse_args()

bytesX, bytesY = args.files[0].read(), args.files[1].read()

ncBytesXY = len(bz2.compress(bytesX + bytesY))
ncBytesX = len(bz2.compress(bytesX))
ncBytesY = len(bz2.compress(bytesY))

# see http://phrack.org/issues/68/15.html

ncd = float(ncBytesXY - min(ncBytesX, ncBytesY)) / max(ncBytesX, ncBytesY)
kc = ncBytesY - ncBytesX

print("ΔK: " + str(kc))
print("NCD: " + str(ncd))

