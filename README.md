# Finding Sensible Kolmogorov Complexity Values for Obfuscation Requirements

Compare the impact of various obfuscators on the algorithmic complexity of the target binary.

(TODO)

## Background

Rabih Mosen and Alexandre Miranda Pinto proposed the use of a normalized version of Kolmogorov complexity as a metric for obfuscation effectiveness. The intuition behind their approach is based on the following argument: if an adversary fails to capture some patterns (regularities) in an obfuscated code, then the adversary will have difficulty comprehending that code: it cannot provide a valid and brief, i.e., simple description. On the other hand, if these regularities are simple to explain, then describing them becomes easier, and consequently the code will not be difficult to understand (1) (2).

In other words, the reverse engineering effort increases with the amount of random noise in the program being reverse engineered. An obfuscated program has more non-essential information, and thus higher complexity, than a non-obfuscated one, and the reverse engineer has to comprehend at least some of this information to fully understand what’s going on. The authors also provide empirical results showing that common obfuscation techniques managed to produce a substantial increase in the proposed metric. They also found that the metric was more sensitive then Cyclomatic measure at detecting any increase in complexity comparing to original un-obfuscated code (20). 

In (2) Pinto et. al. propose the Normalized Kolmogorov Complexity measure which expresses code irregularity of a given program independent of its size. The problem with that approach is that many of the more powerful obfuscation techniques significantly increase the size of a program, which is something that gets lost when using the normalized measure.  Instead, the we are interested absolute increase in algorithmic complexity effected by the obfuscating transformations. 

To illustrate why this makes more sense, think of a small function (such as a cryptographic algorithm) being obfuscated. An implementation of a run-of-the-mill cryptographic primitive like AES can be compressed to about 3-4 kB. Typically, to obscure such a function, it isn’t enough to make it look a bit more random. To make it incomprehensible, you’d have to increase complexity by a significant amount: For example, one could virtualize the function, or encode parts of the cryptographic computation into lookup tables protected with mixing bijections. Depending on how much overhead is added, the obfuscated program may grow to 100kB or more. Even though this added complexity (in form of virtualization layers or mixing bijections) clearly increases the reverse engineering effort for the adversary, this might not be reflected by normalized complexity, which expresses the randomness of the program as a whole: This measure could conceivably remain unchanged or even decrease. What matters is by how much the obfuscating transformations increase complexity in absolute terms.
Conveniently, Kolmogorov complexity can be approximated by lossless compression (2)(3)(4). We apply bzip2 compression, which is one of the most effective lossless compression algorithms to approximate Kolmogorov complexity. For the purpose of the RED score, the idea is to obtain the absolute Kolmogorov complexity measure (simply put, compressed size in bytes) – denoted as K(.) - of the compiled binary or byte-code of the clean implementation R and obfuscated implementation O, and then calculate the difference by which the obfuscated version is larger:

ΔK = K(O) - K(R)

## References

1. *Algorithmic Information Theory for Obfuscation Security.* Rabih Mohsen, Alexandre Miranda Pinto. 2015. International Conference on Security and Cryptography (SECRYPT 2015). ISBN 978-989-758-117-5.

2. *Evaluating Obfuscation Security: A Quantitative Approach.* Rabih Mohsen, Alexandre Miranda Pinto. 2015. Proceedings of the 12th International Conference on Security and Cryptography. ISBN 978-3-319-30303-1.

3. Ming Li and and Paul M. B. Vita ́nyi. *An Introduction to Kolmogorov Complexity and Its Applications.* 3. s.l. : Springer Publishing Company, 2008. ISBN 978-0-387-49820-1.

4. Paul M. B. Vita ́nyi, Frank J. Balbach, Rudi L. Cilibrasi, Ming Li. *Normalized Information Distance.* [book auth.] Frank, Dehmer, Matthias Emmert-Streib. Information Theory and Statistical Learning. s.l. : Springer, 2009. ISBN 978-0-387-84816-7

