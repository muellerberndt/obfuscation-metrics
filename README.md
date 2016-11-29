# Processes and Metrics for Assessing Obfuscation Effectiveness

The [Mobile Application Security Verification Standard (MASVS)](https://github.com/OWASP/owasp-masvs) is a standard for mobile app security. It lists requirements for security controls and software protection mechanisms, and defines four verification levels that can be applied to achieve different grades of security and resiliency. It is accompanied by the [Mobile Security Testing Guide (MSTG)](https://github.com/OWASP/owasp-mstg) which outlines the necessary controls in more details for each mobile operation system (currently Android and iOS).

## The problem

There is no practical, repeatable process to verify whether, and to what grade, a mobile app is resilient against reverse engineering.

Obfuscation is a controversial topic, and there is currently no industry consensus or standard as to what constitutes *strong* obfuscation. The goal of this project is to find workable solutions to this problem. It aims to achieve the following:

* List obfuscating transformations that, when applied correctly, result in (what currently consider) strong resiliency against manual hybrid static / dynamic analysis;

* List verifiable basic parameters that must *always* be fulfilled (e.g. algorithmic complexity added by the transformations, minimum value for normalized compression distance), along with *practical* verification processes.

* List of requirements and recommended parameters for each specific types of obfuscation (e.g. requirements for virtual machine interpreter, white-box must implement counter-measures against SPA and DPA).

* Define processes that can be used by mobile appsec experts for assessing the robustness of obfuscation using white-box and black-box analysis.

## The MASVS model

The MASVS defines a set of high level requirements for software protections. In our model, we differentiate between functional defenses (such as root detection and anti-debugging) and obfuscations. Obfuscating transformations are further categorized into three types:

1. Strip information
2. Obfuscate control flow and data
3. Inhibit reverse engineering processes and tools

In normal cases - for 99% of mobile apps - applying a mix of basic type 1 and type 3 transformations is sufficient. These transformations are generally easy to apply and do not adversely impact the size and performance of the program. However, in some cases it is desirable to apply stronger protections - for example to protect a sensitive computation on Android devices without a dedicated cryptographic processor. In this case, MASVS L4 allows for the use of "advanced" forms of control flow and data obfuscation. This is where things get complicated.

## Goals of this project

The field of control flow and data obfuscation is highly diverse and somewhat controversial. The goal of this project is to distill general rules and guidelines as to what is considered *good enough* obfuscation as per the current industry standards and known de-obfuscation methods. We make the following starting assumptions that should reflect the most common "worst-case" scenario of a highly skilled adversary attempting to reverse engineer a publicly available mobile app:

- Adversaries have are highly skilled and knowledgable about reverse engineering techniques on the target architecture (Android / iOS) and have access to commercial state-of-the-art tools;

- Adversaries are well-informed about current attacks on the type(s) of obfuscation used (e.g. reversing virtual machines using symbolic execution, recovering keys from white-boxes using DFA);

- Adversaries start with zero knowledge about the target app in question, and without details about the particular implementation obfuscating transformations applied.

Given these assumptions, we define *strong* resiliency as a set of transformations and parameters that likely requires the adversary to invest *at least one man-month of work to fully de-obfuscate the program.*

Note that it is unrealistic to assume that strong resiliency as defined above can be proven in a scientifically sound way anytime soon. Initially, we aim for defining guidelines, processes and metrics that enable a human tester to provide a reasonable assessment of whether strong resiliency has been achieved. Ideally, experimental data can then be used to verify (or refute) the proposed metrics.

The situation is similar to "regular" security testing: In practical scenarios, generic, automated static/dynamic analysis in insufficient to prove security of a program. Manual verification by an experienced tester is still the only reliable way to achieve security.

## What is this subproject about?

Note that this project is concerned only with type 2 transformations as defined above. This includes transformations that:

- Result in a measurable increase in one or more properties, such as algorithmic complexity added and compression distance to the original binary;

- Add both static and dynamic complexity (i.e., affect both the binary and instruction trace).

Such transformations also have the following properties:

- They incur a significant size and performance penalty (often scalable with obfuscation settings);
- If applied correctly, de-obfuscation requires a manual static/dynamic approach using highly customized tools.

As a result of these transformations, the original function(s) is computed in more complicated ways and/or encoding data in ways that obscure its “true meaning”. Both code and data transformations belong into this category (note that obfuscation often involves representing code as data and vice versa). For example:

- Pattern-based obfuscation, where instructions are replaced with more complicated instruction sequences
- Adding opaque predicates
- Just-in-Time compilation
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
