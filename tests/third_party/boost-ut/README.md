# Boost.UT

This directory vendors the single-header distribution of Boost.UT v2.3.1,
commit `f923e6fe4b7542d75e0c4ee54ad0af6a5382a87c`, from
<https://github.com/boost-ext/ut>.

It is kept in-tree so that configuring and running the tests does not require
Git or network access. See `LICENSE.md` for the upstream license.

The vendored header has one local compatibility patch: its command-line glob
matcher now implements the `*`, `?`, and escaping syntax that v2.3.1's own
argument parser emits. Upstream v2.3.1 otherwise turns those globs into patterns
that its matcher cannot consume.
