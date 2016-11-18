# Evaluating Obfuscators using Kolmogorov Complexity

Kolmogorov complexity - a.k.a. as descriptive complexity or algorithmic entropy - is the length of the shortest computer program that produces an object as output. Effective obfuscating transformations generally increase Kolmogorov complexity of the obfuscated program. In other words, by wrapping a function into more complex, redundant operations and data lookups, more information is required to describe the program. From a blackbox perspective, it becomes more difficult to comprehend what the program is actually doing.

The goal of this project is to compare the impact of various obfuscators on the Kolmogorov complexity (approximated using lossless compression) of a sample binary.

## Background

Rabih Mosen and Alexandre Miranda Pinto proposed the use of a normalized version of Kolmogorov complexity as a metric for obfuscation effectiveness. The intuition behind their approach is based on the following argument: if an adversary fails to capture some patterns (regularities) in an obfuscated code, then the adversary will have difficulty comprehending that code: it cannot provide a valid and brief, i.e., simple description. On the other hand, if these regularities are simple to explain, then describing them becomes easier, and consequently the code will not be difficult to understand (1) (2).

In other words, the reverse engineering effort increases with the amount of random noise in the program being reverse engineered. An obfuscated program has more non-essential information, and thus higher complexity, than a non-obfuscated one, and the reverse engineer has to comprehend at least some of this information to fully understand what’s going on. The authors also provide empirical results showing that common obfuscation techniques managed to produce a substantial increase in the proposed metric. They also found that the metric was more sensitive then Cyclomatic measure at detecting any increase in complexity comparing to original un-obfuscated code (2).

In (2) Pinto et. al. propose the Normalized Kolmogorov Complexity measure which expresses code irregularity of a given program independent of its size. This is calculated by comparing the algorithmic complexity value of the obfuscated binary against the maximum achievable complexity (upper bounded by the length of the program). The problem with this approach is that powerful obfuscation techniques significantly increase the size of a program, but do not necessarily increase code variability relative to the program size. We are therefore more interested in the *absolute increase in algorithmic complexity* in the obfuscated program in comparison to the "clean" binary.

To illustrate why this makes more sense, think of a small function (such as a cryptographic algorithm) being obfuscated. An implementation of a run-of-the-mill cryptographic primitive like AES can be compressed to about 3-4 kB. Typically, to obscure such a function, it isn’t enough to make it look a bit more random. To make it incomprehensible, one needs to increase complexity by a significant amount: For example, one could virtualize the function, or encode parts of the cryptographic computation into lookup tables protected with mixing bijections. Depending on how much overhead is added, the obfuscated program may grow to a size of 100kB or more. Even though this added complexity (in form of virtualization layers or mixing bijections) clearly increases the reverse engineering effort, this is not reflected by the normalized complexity metric: It could conceivably remain unchanged or even decrease. What matters is by how much the obfuscating transformations increase the absolute algorithmic complexity.

Conveniently, Kolmogorov complexity can be approximated by lossless compression (2)(3)(4). We apply bzip2 compression, which is one of the most effective lossless compression algorithms to approximate Kolmogorov complexity. For the purpose of assessing reverse engi the idea is to obtain the absolute Kolmogorov complexity measure (simply put, compressed size in bytes) – or *K-estimator K(.)* - of the compiled binary or byte-code of the clean implementation R and obfuscated implementation, and then calculate the difference by which the obfuscated version is larger:

<center>*ΔK = K(O) - K(R)*</center>

## Sample program

Our test object is a self-contained [OATH TOTP generator written in C](https://github.com/b-mueller/kolmogorov-metric/tree/master/testprograms/oath-totp). OATH TOTP is an extension to the HMAC-based One-Time Password (HOTP) algorithm that adds support for a time-based moving factor (5). The program consists of a TOTP implementation and SHA1 hashing function.

### Compiling

Samples are compiled with clang distributed with [Android NDK 13b](https://github.com/android-ndk/ndk/wiki) and compressed with bzip2 version 1.0.6.

(... todo ... discuss in 'issues' section)

## References

1. *Algorithmic Information Theory for Obfuscation Security.* Rabih Mohsen, Alexandre Miranda Pinto. 2015. International Conference on Security and Cryptography (SECRYPT 2015). ISBN 978-989-758-117-5.

2. *Evaluating Obfuscation Security: A Quantitative Approach.* Rabih Mohsen, Alexandre Miranda Pinto. 2015. Proceedings of the 12th International Conference on Security and Cryptography. ISBN 978-3-319-30303-1.

3. Ming Li and and Paul M. B. Vita ́nyi. *An Introduction to Kolmogorov Complexity and Its Applications.* 3. s.l. : Springer Publishing Company, 2008. ISBN 978-0-387-49820-1.

4. Paul M. B. Vita ́nyi, Frank J. Balbach, Rudi L. Cilibrasi, Ming Li. *Normalized Information Distance.* [book auth.] Frank, Dehmer, Matthias Emmert-Streib. Information Theory and Statistical Learning. s.l. : Springer, 2009. ISBN 978-0-387-84816-7

5. [RFC6238] M'Raihi D., Machani S., Pei M., Rydell J., "TOTP: Time-Based One-Time Password Algorithm", May 2011. ISSN: 2070-1721
