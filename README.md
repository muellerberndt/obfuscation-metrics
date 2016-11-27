# Processes and Metrics for Assessing Obfuscation Effectiveness

The Mobile Application Security Verification Standard (MASVS) is a standard for mobile app security. It is meant to be used by mobile software architects and developers seeking to develop secure mobile applications and as a basis for mobile app security testing methodologies. The MASVS lists requirements for both security controls and software protection mechanisms, and defines four verification levels that can be applied to achieve different grades of security and resiliency.

Level 4 of the MASVS requires the use of hardware-based isolation features, such as SEE or TEE. However, as specialized hardware is not always available, it allows for strong software protection and obfuscation as a substitute. The software protection requirements are listed in MASVS V8 - "Resiliency Against Reverse Engineering":

- **V8.16: Verify that sensitive computations take place in a trusted environment that is isolated from the mobile operating system. Hardware-based SE or TEE should be used whenever available.**

- **V8.17: If hardware-based isolation is unavailable, verify that a strong form of obfuscation has been applied to isolate sensitive data and computations, and verify the robustness of the obfuscation.**

Obfuscation is a controversial topic however, and there is currently no industry consensus or standard as to what constitutes *strong* obfuscation. The goal of this project is to find workable solutions to this problem. It aims to achieve the following:

* List obfuscating transformations that, when applied correctly, result in (what we consider) strong resiliency against static and dynamic analysis. Allowed types must have quantifiable properties.

* A list of requirements and recommended parameters for each obfuscation type (e.g. minimum algorithmic complexity of virtual machine interpreter, white-box must implement counter-measures against SPA and DPA).

* Outline *practical* verification processes that can be used by mobile appsec experts for assessing and / or testing the robustness of obfuscation using white-box and black-box analysis.

## Threat Model

By obfuscating programs, we aim to increase the effort adversaries need to invest to achieve certain goals. The following table states a threat model that looks at the obfuscated app from the attacker's perspective.

|Goal of the adversary|Example|Countermeasure|
|---|---|---|
|1.) Comprehend all or parts of the code and/or data in the program with the goal of replicating some functionality. | Extract a cryptographic key from the implementation of a cryptographic primitive, e.g. AES key.  |Transform code and/or data to hide its semantics. E.g.: computing the same function(s) in more complicated ways and/or encoding data in ways that obscure its original meaning. Effective transformations make the code and data appear near-random. |
|2.) Tamper with the program to change its function. | Modify code of an online game to enable features such as infinite in-game currency.  |Transform code and/or data to obscure the implementation, making it difficult to make changes. Additionally, defensive mechanisms such as anti-debugging, anti-tampering and integrity checks. |
|3.) Re-use all or parts of the program to replicate some functionality (code lifting). |Extract the encryption routine from a media player and include it in a counterfeit player, which decrypts the digital media without enforcing the contained usage policies.|Transform code and/or data to hide its semantics, and tie the code to the environment so that it only executes correctly on the specific device (device binding). |

The obfuscation metrics project is concerned only with obfuscating transformations applied to a program. In practice, defensive mechanisms, such as root detection, file and memory integrity checks and anti-debugging are an integral part of an effective software protection scheme. Obfuscation should always be applied in combination with multiple defensive mechanisms. The MASVS and MSTG contains separate requirements and testing procedures for such mechanisms.

## Allowed Transformations

Before we can even attempt to define minimal requirements for *good enough* obfuscation, we need to find properties that are quantifiable.
Many types of obfuscating transformations are available, but not all of them offer such properties. For example, layout obfuscation makes a compiled program less intelligible to humans, but is "free" in the sense that it doesn’t increase the size or speed of operation of the program. To fulfill our notion of strong obfuscation, the transformations must be applied must:

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
