# Processes and Metrics for Assessing Obfuscation Effectiveness

Effective software protection schemes combine a variety of of obfuscating transformations and defensive mechanisms. In some cases, it is desirable to protect a secret function, or piece of data, using advanced forms of control flow and/or data obfuscation. In this project, we survey and discuss widely used transformations, as well as the current state-of-the-art in de-obfuscation. Ideally, we want to agree on what constitutes *good enough* obfuscation in particular contexts.

## The Problem

One of the main goals in the guide is to develop a framework for assessing the effectiveness of a given set of software protections.

Client-side software protections are increasingly used in mobile apps. Unfortunately, there is no practical, repeatable process to verify whether, and to what grade, a mobile app is resilient against reverse engineering. One of the goals in the [OWASP Mobile Security Testing Guide Project](https://www.owasp.org/index.php/OWASP_Mobile_Security_Testing_Guide) is to develop a framework for testing the effectiveness of a given set of software protections.

### What is This Project About?

"Processes and Metrics for Assessing Obfuscation Effectiveness" is an auxiliary project, the results of which will feed back into the [OWASP Mobile Application Verification Standard](https://github.com/OWASP/owasp-masvs) and [Mobile Security Testing Guide](https://github.com/OWASP/owasp-mstg). It deals with forms of control flow and data obfuscation that aim to make comprehension tasks more difficult.

We are only concerned with transformations that:

- Result in a measurable increase in one or more properties, such as [algorithmic complexity](02a_kolmogorov_complexity.md) added and [compression distance](02b_normalized_compression_distance.md) to the original binary;
- Add both static and dynamic complexity (i.e., affect both the binary file(s) and instruction trace).

Our basic premise is that manual comprehension tasks become more time-intensive as program complexity increases, provided that:

1. Robust transformations are applied appropriately to the code and/or data;
2. A sufficient increase in program complexity is achieved;
3. The transformations used are resilient against currently available automated de-obfuscation tools.

The high-level goal of this project is to elaborate on notions such as "robust transformations", "sufficient complexity" - based on the current state-of-the-art - and to define processes and metrics that enable a human tester to provide a reasonable assessment of the resiliency level achieved by a set of transformations. Ideally, experimental data can then be used to verify (or refute) the proposed metrics.

### Limitations

Obviously, obfuscation should never be mindlessly applied to a program. Instead, it may be (carefully) employed in the context of a larger software protection schem, to defend against specific client-side threats. The decision whether any form of software protection should be used follows from the threat model. While threat modeling is a requirement in MASVS L2, is it not discussed in the "obfuscation metrics" project.

Furthermore, control flow and data transformations must always be augmented with other types of defenses in practice, such as anti-debugging and anti-tampering. These measures are discussed in the MASVS and MSTG as well, but are not within the scope of this project.

### Project Goals

The project aims to achieve the following:

* List obfuscating transformations that are currently seen as robust against manual comprehension tasks (causing high cognitive load);

* Survey automated de-obfuscation methods as well as transformations to counter each method;

* List verifiable minimum requirements for different types of control flow and data obfuscation (e.g. the particular type of transformations used and the minimum increase in algorithmic complexity added by the transformations), along with practical verification processes.

### Resiliency Against What?

As a starting point, we assume the "worst-case" scenario of a highly skilled and resourceful adversary performing a tool-supported manual attack.

- Adversaries are highly skilled and knowledgable about reverse engineering techniques on the target architecture and have access to commercial state-of-the-art tools;

- Adversaries are well-informed about current attacks on the type(s) of obfuscation used (static analysis, dynamic analysis using concrete and/or symbolic execution, domain-specific attacks such as DFA/DPA, etc.);

- Adversaries start with zero knowledge about the particular implementation / set of obfuscating transformations applied.

(TODO: Find better definition?) Given these assumptions, we define resiliency as a set of transformations and parameters that - considering the current state-of-the-art in binary analysis - forces the skilled adversary to invest significant effort (e.g. a man-month of focused manual analysis) to reach their specified goal (e.g. fully comprehending a program, extracting a particular piece of data, or tampering with some functionality).

## Transformations

- [Control Flow and Data Obfuscation](writeups/03a_control_flow_and_data_obfuscation.md)
- [Virtualization](writeups/03b_virtualization.md)
- [White-box Cryptography](writeups/03c_whitebox_cryptography.md)

## Effectiveness Criteria

- [Difficulty of CFG Recovery]
- [Increase in Program Complexity]
- [Slowdown of Program Analysis Algorithms]

## Useful Metrics

- [Kolmogorov Complexity](writeups/02a_kolmogorov_complexity.md)
- [Normalized Compression Distance](writeups/02b_normalized_compression_distance.md)

### Traditional Code Complexity Metrics

- Number of edges in the call graph
- Cumulative number of edges in the control flow graph
- Cumulative number of basic blocs
- Cumulative number of instructions
- Cyclomatic number
- Oviedo complexity

## Metrics Tools

- [Metrics script](tools/python-metrics/obfm.py)
- [LLVM bitcode metrics pass plugin](tools/llvm-bitcode-metrics)

## Crackmes

- [License Check 1 - Android](crackmes/android/01_license_check_1/)

## About the OWASP MASVS and MSTG
The [OWASP Mobile Security Verification Standard (MASVS)](https://github.com/OWASP/owasp-masvs) is a standard for mobile app security. It lists requirements for security controls and software protection mechanisms, and defines verification levels that can be applied to achieve different grades of security and resiliency. It is accompanied by the [OWASP Mobile Security Testing Guide (MSTG)](https://github.com/OWASP/owasp-mstg) which outlines the controls in more details for each mobile operation system (currently Android and iOS).
