# Control Flow and Data Obfuscation

## Applicable Metrics

## Results

### OATH-TOTP

- Test program: [mini-totp.c](https://github.com/b-mueller/obfuscation-metrics/blob/master/testprograms/oath-totp/mini-totp.c)
- Language: C
- LoC: 465
- Functions: 13
- Compiler cmdline: clang -Os

Mini-TOTP is a self-contained OATH-TOTP implementation with a hardcoded seed. Normalized compression distance and absolute increase in Kolmogorov complexity were calculated for various combinations of obfuscating transformations.

|Transformation|Cmdline|ΔK|NCD|
|---|---|---|---|
|2 opaque predicates per function|tigress --Transform=InitOpaque --Functions=* --Transform=UpdateOpaque --Functions=* --Transform=AddOpaque --Functions=* --AddOpaqueCount=2 --AddOpaqueKinds=call,bug,true  --Transform=CleanUp --Functions=* --out=test-01.c mini-totp.c| 1,939| 0.7866 |
|10 opaque predicates per function|tigress --Transform=InitOpaque --Functions=* --Transform=UpdateOpaque --Functions=* --Transform=AddOpaque --Functions=* --AddOpaqueCount=10 --AddOpaqueKinds=call,bug,true  --Transform=CleanUp --Functions=* --out=test-01.c mini-totp.c| 5,079| 0.8690 |
|Virtualize 8 functions|tigress --Transform=Virtualize --Functions=/oath/,sha1_read_ctx,sha1_finish_ctx,hmac_sha1,sha1_stream,sha1_buffer,sha1_init_ctx --Transform=CleanUp --CleanUpKinds=annotations --out=test-03.c mini-totp.c| 6,286 | 0.8750 |
|Virtualize 8 functions, use linear dispatch, merge instructions into "superoperators"|tigress --Transform=Virtualize --Functions=/oath/,sha1_read_ctx,sha1_finish_ctx,hmac_sha1,sha1_stream,sha1_buffer,sha1_init_ctx --VirtualizeDispatch=linear --VirtualizeMaxDuplicateOps=2 --VirtualizeOperands=* --VirtualizeSuperOpsRatio=2.0 --VirtualizeMaxMergeLength=10 --VirtualizeOptimizeBody=true --Transform=CleanUp --CleanUpKinds=annotations --out=test-04.c mini-totp.c| 14,318 | 0.9275 |
