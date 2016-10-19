#!/bin/sh

cat *.o > out && ls -l out && bzip2 out && ls -l out.bz2 && rm out.*


