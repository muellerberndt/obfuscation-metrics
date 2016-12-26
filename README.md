# Processes and Metrics for Assessing Obfuscation Effectiveness

Effective software protection schemes combine a variety of of obfuscating transformations and defensive mechanisms. In some cases, it is desirable to protect a secret function, or piece of data, using advanced forms of control flow and/or data obfuscation. In this project, we survey and discuss widely used transformations, as well as the current state-of-the-art in de-obfuscation. Ideally, we want to agree on what constitutes *good enough* obfuscation in certain contexts.

## The Problem

One of the main goals in the guide is to develop a framework for assessing the effectiveness of a given set of software protections.

Client-side software protections are increasingly used in mobile apps. Unfortunately, there is no practical, repeatable process to verify whether, and to what grade, a mobile app is resilient against reverse engineering. One of the goals in the OWASP Mobile Project is to develop a framework for testing the effectiveness of a given set of software protections.

### What is This Project About?

"Processes and Metrics for Assessing Obfuscation Effectiveness" is an auxiliary project, the results of which will be used in the OWASP Mobile Application Verification Standard (MASVS) and Mobile Security Testing Guide (MSTG). It deals with specific forms of control flow and data obfuscation.

Here, we are only concerned with transformations that:

- Result in a measurable increase in one or more properties, such as [algorithmic complexity](02a_kolmogorov_complexity.md) added and [compression distance](02b_normalized_compression_distance.md) to the original binary;

- Add both static and dynamic complexity (i.e., affect both the binary file(s) and instruction trace).

Our working hypothesis that reverse engineering effort increases with program complexity, provided that transformations are used effectively and well-known automated de-obfuscation techniques are countered. Based on this assumption, we attempt to define criteria, processes and metrics that enable a human tester to provide a reasonable assessment of the resiliency level achieved by a set of transformations. Ideally, experimental data can then be used to verify (or refute) the proposed metrics.

In practice, control flow and data transformations must always be augmented with other types of defenses, such as anti-debugging and anti-tampering. These measures are discussed in the MASVS and MSTG as well, but are not within the scope of this project.

### Project Goals

* Find an agreeable definition of *strong resiliency* against a number of tasks, such as program comprehension and modification;

* List obfuscating transformations that, when applied correctly, result in *strong resiliency* against those tasks;

* List verifiable requirements that must *always* be fulfilled (e.g. algorithmic complexity added by the transformations, minimum value for normalized compression distance), along with practical verification processes.

* List specific requirements for each specific type of obfuscation.

As a starting point, we assume the "worst-case" scenario of a highly skilled and resourceful adversary performing a tool-supported manual attack:

- Adversaries are highly skilled and knowledgable about reverse engineering techniques on the target architecture and have access to commercial state-of-the-art tools;

- Adversaries are well-informed about current attacks on the type(s) of obfuscation used (program analysis, symbolic execution, DPA/DFA,...);

- Adversaries start with zero knowledge about the proprietary parts of the target app, and without details about the particular implementation of the obfuscating transformations applied.

Given these assumptions, we define *strong* resiliency as a set of transformations and parameters that - considering the current state-of-the-art in binary analysis - forces the skilled adversary to invest significant effort (i.e. a man-month or more of manual analysis) to reach their goal (TODO: Find better definition).

The situation is analogue to "regular" security testing: For real-world apps, generic, automated static/dynamic analysis in insufficient to prove security of a program. Manual verification by an experienced tester is still the only reliable way to achieve security.

## Transformations

- [Control Flow and Data Obfuscation](writeups/03a_control_flow_and_data_obfuscation.md)
- [Virtualization](writeups/03b_virtualization.md)
- [White-box Cryptography](writeups/03c_whitebox_cryptography.md)

## Proposed Metrics

- [Kolmogorov Complexity](writeups/02a_kolmogorov_complexity.md)
- [Normalized Compression Distance](writeups/02b_normalized_compression_distance.md)

### Traditional Code Complexity Metrics

- Number of edges in the call graph
- Cumulated number of edges in the control flow graph
- Cumulated number of basic blocs
- Cumulated number of instructions
- Cyclomatic number
- Oviedo complexity

## Testing Tools

- [Metrics script](tools/python-metrics/obfm.py)
- [LLVM bitcode metrics pass plugin](tools/llvm-bitcode-metrics)

## Sample Code and Crackmes

- [Self-contained OATH-TOTP Generator in C](sample_code/oath-totp)

## About the OWASP MASVS and MSTG
The [OWASP Mobile Security Verification Standard (MASVS)](https://github.com/OWASP/owasp-masvs) is a standard for mobile app security. It lists requirements for security controls and software protection mechanisms, and defines verification levels that can be applied to achieve different grades of security and resiliency. It is accompanied by the [OWASP Mobile Security Testing Guide (MSTG)](https://github.com/OWASP/owasp-mstg) which outlines the controls in more details for each mobile operation system (currently Android and iOS).
