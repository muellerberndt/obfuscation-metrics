Bitcode Metrics
===============

A very simple analysis that dumps some bit code statistics!

- Number of edges in the call graph
- Cumulated number of edges in the control flow graph
- Cumulated number of basic blocs
- Cumulated number of instructions
- `Cyclomatic number <https://en.wikipedia.org/wiki/Cyclomatic_complexity>`_
- `Oviedo Complexity <http://dl.acm.org/citation.cfm?id=168040>`_
- An approximation of `Kolmogorov complexity <https://en.wikipedia.org/wiki/Kolmogorov_complexity>`_

Install
-------

You need `LLVM <http://llvm.org/>`_ development headers, installed in ``LLVM_ROOT``, and a decent C++ compiler. Then from source:

.. code:: shell

    mkdir _build && cd _buil
    cmake .. -DLLVM_ROOT=/my/path/to/llvm/install
    make

Run
---

Once built, the project should contain a ``LLVMBitcodeMetrics.so`` file. That's the pass plugin. To run it, fire off:

.. code:: shell

    opt -load LLVMBitcodeMetrics.so -analyze -bitcode-metrics a.ll

