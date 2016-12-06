# Processes and Metrics for Assessing Obfuscation Effectiveness

The [Mobile Application Security Verification Standard (MASVS)](https://github.com/OWASP/owasp-masvs) is a standard for mobile app security. It lists requirements for security controls and software protection mechanisms, and defines four verification levels that can be applied to achieve different grades of security and resiliency. It is accompanied by the [Mobile Security Testing Guide (MSTG)](https://github.com/OWASP/owasp-mstg) which outlines the necessary controls in more details for each mobile operation system (currently Android and iOS).

One of our main goals is to develop a framework for assessing the effectiveness of a given set of software protections. "Obfuscation metrics" is an auxiliary project that deals with specific forms of control flow and data obfuscation.

## The problem

There is no practical, repeatable process to verify whether, and to what grade, a mobile app is resilient against reverse engineering. One of the main goals in the guide is to develop a framework for assessing the effectiveness of a given set of software protections. 

Effective protection schemes combine a variety of of obfuscating transformations and protection mechanisms. In some cases, it is desirable to protect a secret function, or piece of data, using advanced forms of control flow and/or data obfuscation. In the obfuscation metrics project, we catalogue, discuss and assess these transformations. Ideally, we want to arrive at an agreement on what is considered *good enough* obfuscation in certain context(s) (hopefully generalized to a small amount of threat scenarios).

### The OWASP model

The MASVS defines a set of high level requirements for software protections. In our model, we differentiate between functional defenses (such as root detection and anti-debugging) and obfuscations. Obfuscating transformations are further categorized into two types:

#### 1. Strip information

Compiled programs often retain explanative information that is helpful for the reverse engineer, but isn’t actually needed for the program to run. Debugging symbols that map machine code or byte code to line numbers, function names and variable names are an obvious example.

For instance, class files generated with the standard Java compiler include the names of classes, methods and fields, making it trivial to reconstruct the source code. ELF and Mach-O binaries have a symbol table that contains debugging information, including the names of functions, global variables and types used in the executable.
Stripping this information makes a compiled program less intelligible while fully preserving its functionality. Possible methods include removing tables with debugging symbols, or renaming functions and variables to random character combinations instead of meaningful names. This process sometimes reduces the size of the compiled program and doesn’t affect its runtime behavior.

#### 2. Obfuscate control flow and data

Program code and data can be transformed in unlimited ways - and indeed, the field of control flow and data obfuscation is highly diverse, with a large amount of research dedicated to both obfuscation and de-obfuscation. Deriving general rules as to what is considered *strong* obfuscation is not an easy task. Our working hypothesis that reverse engineering effort generally increases with program complexity, as long as no well-known automated de-obfuscation techniques for the particular transformation exists. Note that it is unrealistic to assume that *strong resiliency* against manual static/dynamic analysis can be proven in a scientifically sound way for a complex program. We merely attempt to define guidelines, processes and metrics that enable a human tester to provide a reasonable assessment of whether strong resiliency has been achieved. Ideally, experimental data can then be used to verify (or refute) the proposed metrics.

Our assessment methods involves two steps:

1. Apply complexity and distance metrics to quantify the overall impact of the obfuscating transformations;
2. Define domain-specific criteria based on the state-of-the-art in obfuscation research.

Different types of obfuscating transformations vary in their impact on program complexity. In general, there is a gradient from simple *tricks*, such as packing and encryption of large code blocks and manipulations of executable headers, to more "intricate" forms of obfuscation that add significant complexity to parts of the code, data and execution trace.

Simple transformations can be used to defeat standard static analysis tools without causing too much impact on size on performance. The execution trace of the obfuscated function(s) remains more or less unchanged. De-obfuscation is relatively trivial, and can be accomplished with standard tools without scripting or customization.

Advanced methods aim to hide the semantics of a computation by computing the same function in a more complicated way, or encoding code and data in ways that are not easily comprehensible. Transformations in this category have the following properties:

- The size and performance penalty can be sizable (scales with the obfuscation settings)
- De-obfuscation requires advanced methods and/or custom tools

A simple example for this kind of obfuscations are opaque predicates. Opaque predicates are redundant code branches added to the program that always execute the same way, which is known a priori to the programmer but not to the analyzer. For example, a statement such as if (1 + 1) = 1 always evaluates to false, and thus always result in a jump to the same location. Opaque predicates can be constructed in ways that make them difficult to identify and remove in static analysis.
Some types of obfuscation that fall into this category are:

- Pattern-based obfuscation, when instructions are replaced with more complicated instruction sequences
- Control flow obfuscation
- Control flow flattening
- Function Inlining
- Data encoding and reordering
- Variable splitting
- Virtualization
- White-box cryptography

### What is this project about?

The goal of this sub-project is to find sensible requirements for advanced control flow and data obfuscations. The results of the project will feed back into the MASVS and MSTG. Here, we are only concerned with tranformations of control flow and data that:

- Result in a measurable increase in one or more properties, such as [algorithmic complexity](https://github.com/b-mueller/obfuscation-metrics/blob/master/02a_kolmogorov_complexity.md) added and [compression distance](https://github.com/b-mueller/obfuscation-metrics/blob/master/02b_normalized_compression_distance.md) to the original binary;

- Add both static and dynamic complexity (i.e., affect both the binary file(s) and instruction trace).

In practice, such transformations must always be augmented with other types of defenses, such as anti-debugging and anti-tampering. These measures are discussed in the MASVS and MSTG as well, but are not within the scope of this project.

### Project goals

* Find an agreeable definition of *strong resiliency* against reverse engineering against manual hybrid static / dynamic analysis (one possible definition is proposed below);

* List obfuscating transformations that, when applied correctly, result in *strong resiliency*;

* List verifiable basic requirements that must *always* be fulfilled (e.g. algorithmic complexity added by the transformations, minimum value for normalized compression distance), along with *practical* verification processes.

* List requirements for each specific type of obfuscation (e.g. white-box must implement counter-measures against SPA and DPA).

As a starting point, we make the following starting assumptions that should reflect the most realistc "worst-case" scenario of a highly skilled adversary attempting to reverse engineer a publicly available mobile app:

- Adversaries are highly skilled and knowledgable about reverse engineering techniques on the target architecture (Android / iOS) and have access to commercial state-of-the-art tools;

- Adversaries are well-informed about current attacks on the type(s) of obfuscation used (e.g. reversing virtual machines using symbolic execution, recovering keys from white-boxes using DPA);

- Adversaries start with zero knowledge about the proprietary parts of the target app, and without details about the particular implementation of the obfuscating transformations applied.

Given these assumptions, we define *strong* resiliency as a set of transformations and parameters that likely requires the adversary to invest *at least one man-month of work to fully de-obfuscate the program.*

The situation is analogue to "regular" security testing: For real-world apps, generic, automated static/dynamic analysis in insufficient to prove security of a program. Manual verification by an experienced tester is still the only reliable way to achieve security.

## Prososed Metrics

- [Kolmogorov Complexity](https://github.com/b-mueller/obfuscation-metrics/blob/master/02a_kolmogorov_complexity.md)
- [Normalized Compression Distance](https://github.com/b-mueller/obfuscation-metrics/blob/master/02b_normalized_compression_distance.md)

### Traditional code complexity metrics

- Number of edges in the call graph
- Cumulated number of edges in the control flow graph
- Cumulated number of basic blocs
- Cumulated number of instructions
- Cyclomatic number
- Oviedo Complexity

## Transformations and Testing Processes

- [Control Flow and Data Obfuscation](https://github.com/b-mueller/obfuscation-metrics/blob/master/03a_control_flow_and_data_obfuscation.md)
- [Virtualization](https://github.com/b-mueller/obfuscation-metrics/blob/master/03b_virtualization.md)
- [White-box Cryptography](https://github.com/b-mueller/obfuscation-metrics/blob/master/03c_whitebox_cryptography.md)

## Tools

- [Metrics script](https://github.com/b-mueller/obfuscation-metrics/blob/master/tools/obfm.py)
- [LLVM bitcode metrics pass plugin](https://github.com/b-mueller/obfuscation-metrics/tree/master/tools/llvm-bitcode-metrics)

## Sample Programs

- [OATH-TOTP](https://github.com/b-mueller/obfuscation-metrics/tree/master/testprograms/oath-totp)
