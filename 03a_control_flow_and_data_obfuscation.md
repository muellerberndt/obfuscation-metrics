# Control Flow and Data Obfuscation

## Applicable Metrics

## Results

### OATH-TOTP

- Test program: [mini-totp.c](https://github.com/b-mueller/obfuscation-metrics/blob/master/testprograms/oath-totp/mini-totp.c)
- Language: C
- LoC: 465
- Functions: 13
- Compiler cmdline: clang -Os

Mini-TOTP is a self-contained OATH-TOTP implementation with a hardcoded seed. Normalized compression distance and increase in Kolmogorov complexity were calculated for various combinations of obfuscating transformations.

|Transformation|Cmdline|ΔK|NCD|
|---|---|---|---|
|2 Opaque predicates per function|tigress --Transform=InitOpaque --Functions=* --Transform=UpdateOpaque --Functions=* --Transform=AddOpaque --Functions=* --AddOpaqueCount=2 --AddOpaqueKinds=call,bug,true  --Transform=CleanUp --Functions=* --out=test-01.c mini-totp.c| 3,722| 1.0059 |
