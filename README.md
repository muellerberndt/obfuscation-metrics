# Processes and Metrics for Assessing Obfuscation Effectiveness

The Mobile Application Security Verification Standard (MASVS) is a standard for mobile app security. It is meant to be used by mobile software architects and developers seeking to develop secure mobile applications and as a basis for mobile app security testing methodologies. The MASVS lists requirements for both security controls and software protection mechanisms, and defines four verification levels that can be applied to achieve different grades of security and resiliency.

Level 4 of the MASVS requires the use of hardware-based isolation features, such as SEE or TEE. However, as specialized hardware is not always available, it allows for strong software protection and obfuscation as a substitute. The software protection requirements are listed in MASVS V8 - "Resiliency Against Reverse Engineering":

- **V8.16: Verify that sensitive computations take place in a trusted environment that is isolated from the mobile operating system. Hardware-based SE or TEE should be used whenever available.**

- **V8.17: If hardware-based isolation is unavailable, verify that a strong form of obfuscation has been applied to isolate sensitive data and computations, and verify the robustness of the obfuscation.**

Obfuscation is a controversial topic however, and there is currently no industry consensus or standard as to what constitutes *strong* obfuscation. The goal of this project is to find workable solutions to this problem. It aims to achieve the following:

* List obfuscating transformations that, when applied correctly, result in (what we consider) strong resiliency against static and dynamic analysis.

* A list of requirements and recommended parameters for each obfuscation type (e.g. minimum algorithmic complexity of virtual machine interpreter, white-box must implement counter-measures against SPA and DPA).

* *Practical* verification processes that can be used by mobile appsec experts for testing the robustness of obfuscation using white-box and black-box analysis.

## Allowed Transformations

Before we can attempt to define minimal requirements for *good enough* obfuscation, we need to find quantifiable properties that correlate with obfuscation strength. Some types of transformations, such as layout obfuscations (which work by discarding semantic information meaningful to humans, but do not impact alter program size and performance) do not easily lend themselves to being measured. To fulfill our notion of strong obfuscation, transformations must be applied that:

- Result in a measurable increase in one or more properties, such as algorithmic complexity added and compression distance to the original binary;

- Add both static and dynamic complexity (i.e., affect both the binary and instruction trace).

Such transformations also have the following properties:

- They incur a significant size and performance penalty (often scalable with obfuscation settings);
- If applied correctly, de-obfuscation requires a manual static/dynamic approach using highly customized tools.

As a result of these transformations, the original function(s) is computed in more complicated ways and/or encoding data in ways that obscure its “true meaning”. Both code and data transformations belong into this category (note that obfuscation often involves representing code as data and vice versa). For example:

- Pattern-based obfuscation, where instructions are replaced with more complicated instruction sequences
- Adding opaque predicates
- Control flow flattening with added logic for context manipulation
- Inserting junk code
- Data encoding and reordering
- Variable splitting
- Virtualization
- White-box cryptography

## Metrics
- [Kolmogorov Complexity](https://github.com/b-mueller/obfuscation-metrics/blob/master/02a_kolmogorov_complexity.md)
- [Normalized Compression Distance](https://github.com/b-mueller/obfuscation-metrics/blob/master/02b_normalized_compression_distance.md)

## Transformations and Testing Processes

- [Control Flow and Data Obfuscation](https://github.com/b-mueller/obfuscation-metrics/blob/master/03a_control_flow_and_data_obfuscation.md)
- [Virtualization](https://github.com/b-mueller/obfuscation-metrics/blob/master/03b_virtualization.md)
- [White-box Cryptography](https://github.com/b-mueller/obfuscation-metrics/blob/master/03c_whitebox_cryptography.md)

## Tools

- [Metrics script](https://github.com/b-mueller/obfuscation-metrics/blob/master/tools/obfm.py)

## Sample Programs

- [OATH-TOTP](https://github.com/b-mueller/obfuscation-metrics/tree/master/testprograms/oath-totp)
