# Trunk-Based Development

> **Trunk-Based Development (TBD)** is a source-control branching model where
> developers collaborate on code in a single branch called the *trunk*
> (`main`, `master`), resist any pressure to create long-lived development
> branches, and therefore integrate their work with everyone else at least
> once a day.

Trunk-Based Development is the branching model that makes **Continuous
Integration** possible. Continuous Integration is, by definition, the
practice of integrating everyone's work frequently; a branching model that
keeps changes isolated for days or weeks is the opposite of that, no matter
how much automation sits on top of it.


## Why the Branching Model Matters

When a change lives on a separate branch, it is **not integrated**. The
longer it stays there, the more the trunk moves on without it, and the
larger and riskier the eventual merge becomes:

* **Merge conflicts** grow with the age and size of the branch.

* **Semantic conflicts** (code that merges cleanly but behaves wrongly
    together) are invisible to the version-control tool and are only caught
    by tests that run *after* integration.

* **Feedback is delayed**: a design problem introduced on a two-week branch
    is discovered two weeks late, when it is expensive to fix.

* **The trunk's real state is unknown**: if ten branches are outstanding,
    nobody knows whether the combined system actually works.

> The core insight of Continuous Integration is that integrating often and
> in small pieces is **less** work than integrating rarely in big pieces,
> because problems are found while they are still small.


## Branching Models Compared

### GitFlow / Feature Branching

Every feature is developed on its own branch and merged when "done".
Common in open-source and in teams with gated review processes.

* Branches routinely live for **days or weeks**.
* Integration is deferred to merge time.
* Release branches, hotfix branches, and a separate `develop` branch add
    process overhead.
* **Not compatible with Continuous Integration** in the strict sense.

### Trunk-Based Development

All developers commit to the trunk, or to branches that live for
**hours, not days**.

* **Committing straight to trunk**: works well for small, experienced,
    co-located or well-coordinated teams. Every commit runs the commit-stage
    tests.

* **Short-lived feature branches**: a developer branches from the trunk,
    works for at most a day, opens a pull request, gets a fast review, and
    merges. The branch exists only long enough to run CI and get a review.
    This is the pragmatic form of TBD used by most teams and is still
    Continuous Integration, as long as the branch is genuinely short-lived.

The distinction that matters is **not** "branch or no branch" but **how
long a change stays unintegrated**.


## Keeping the Trunk Releasable

If everyone commits to the trunk, the trunk must **never be broken** and must
**always be releasable**. Unfinished work therefore has to be committed in a
way that does not break users of the system. Several techniques make this
possible.

### Work in Small Batches

Break every task into a sequence of small, safe steps that each keep the
system working. A large feature becomes many commits, each of which
compiles, passes its tests, and could in principle be released. This is a
skill; it is learned by practising it.

### Branch by Abstraction

A technique for making a **large change incrementally on the trunk**:

1. Introduce an **abstraction layer** in front of the code you want to
    replace.
2. Move clients over to the abstraction, one at a time, committing after
    each.
3. Build the new implementation behind the same abstraction.
4. Switch clients to the new implementation.
5. Remove the old implementation and, if no longer needed, the abstraction.

At every step the trunk is consistent and shippable.

### Feature Toggles (Feature Flags)

A **feature toggle** is a conditional that lets code be committed to the
trunk while remaining **inactive in production**:

```
if (features.isEnabled("new-payment-flow")) {
    newPaymentFlow();
} else {
    oldPaymentFlow();
}
```

* The incomplete feature ships **dark** (present but switched off).
* It can be enabled for developers, for a test environment, or for a subset
    of users.
* Toggles used only to hide unfinished work should be **short-lived** and
    removed once the feature is released; otherwise they accumulate as
    technical debt and combinatorial test complexity.

### Keystone / Dark Launching

Build the feature bottom-up and connect the **user-visible entry point
last** ("the keystone"). The code is fully present and tested on the trunk;
only the final wiring that exposes it to users is withheld until release.


## Releasing from the Trunk

* **Continuous Deployment**: every green commit on the trunk is deployed
    automatically. Requires a strong pipeline and, usually, feature toggles.

* **Release branches**: for products with discrete versions (typical for
    embedded firmware), a **release branch is cut from the trunk** at release
    time. Development continues on the trunk; only critical fixes are
    *cherry-picked* onto the release branch. Release branches are for
    stabilisation and patching, never for feature development.


## Code Review under Trunk-Based Development

Fast integration and thorough review can coexist:

* **Pair programming** is continuous review; a separately reviewed pull
    request is then unnecessary.

* **Short-lived pull requests** with a review turnaround measured in
    **minutes to a few hours**, not days. A slow review process silently
    turns TBD back into feature branching.

* **Post-commit review** for trusted, experienced teams: commit to trunk
    now, review shortly after, backed by the safety net of the pipeline.


## Relevance for Embedded Systems

* **Long build and flash times** make big-bang integration especially
    painful; small, frequent commits keep each failure cheap to diagnose.

* **Hardware variants** are a natural fit for *branch by abstraction* and
    *link-time* / *compile-time* selection rather than long-lived
    per-board branches (see
    [Embedded Architectures / Hardware Abstraction](../../embedded-architectures/)).

* **Certified or safety-critical products** still need TBD for development
    speed, combined with **release branches** per certified version and
    disciplined cherry-picking of fixes.

* **Hardware-in-the-loop tests** are slow, so they usually run against the
    trunk on a schedule or per merge, while fast host-based unit tests gate
    every commit.


## Common Pitfalls

* Calling it TBD while pull requests routinely live for days.
* A commit-stage build that takes longer than ~10 minutes, so developers
    batch up changes to avoid waiting.
* Committing broken code and relying on someone else to fix the trunk.
* Feature toggles that are never removed.
* Using release branches for new features "just this once".


## Practices Checklist

* Integrate to the trunk **at least once a day**.
* Keep branches shorter than a day; prefer committing straight to trunk.
* Every commit keeps the trunk **compiling, tested, and releasable**.
* Hide unfinished work with **feature toggles** or the **keystone** pattern.
* Make large changes with **branch by abstraction**.
* Never commit on a broken build; never go home on a broken build.
* Keep the commit stage **under 10 minutes**.
* Review fast, or pair.


## References

* Jez Humble, David Farley. **Continuous Delivery**. Addison-Wesley, 2010
* David Farley. **Continuous Delivery Pipelines**. Independently published, 2021
* Nicole Forsgren, Jez Humble, Gene Kim. **Accelerate**. IT Revolution, 2018
* Paul Hammant et al. [**trunkbaseddevelopment.com**](https://trunkbaseddevelopment.com/)
* Martin Fowler. [**Patterns for Managing Source Code Branches**](https://martinfowler.com/articles/branching-patterns.html)
* Martin Fowler. [**FeatureToggle**](https://martinfowler.com/bliki/FeatureToggle.html)
* Martin Fowler. [**BranchByAbstraction**](https://martinfowler.com/bliki/BranchByAbstraction.html)

_Egon Teiniker, 2025-2026, GPL v3.0_
