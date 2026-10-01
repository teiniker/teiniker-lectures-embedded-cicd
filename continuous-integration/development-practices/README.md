# Development Practices

Continuous Integration is not only a matter of tools. It depends on 
**development practices** that allow developers to make small, 
well-tested changes and integrate them into the shared code base 
many times a day.

In this section, we look at the practices that support the **Pre-Commit Stage**: 
how the team is organized (**cross-functional teams**), how code and tests 
are written (**Test-Driven Development**, **Pair Programming** and the use of 
**Generative AI**), and how changes flow into the code repository 
(**Trunk-Based Development**).


## Cross-Functional Team

A **cross-functional team** contains all the skills required to take an idea 
from concept all the way to production without needing to hand work off to 
another team.

Using cross-functional teams provides several powerful benefits:

* **Faster Feedback and Problem Resolution**: 
    - Issues uncovered in CI (failing builds, test failures, integration conflicts) 
    can be addressed immediately by the right people.
    - No waiting for handoffs between siloed departments.
    - Bottlenecks are reduced because expertise is available inside the team.

* **Better Quality Through Shared Ownership**:
    - Tests are written early and run constantly.
    - Code quality and integration stability are shared responsibilities.
    - Developers are more aware of downstream impacts of changes.    

* **Improved Communication and Transparency**:
    - Early detection of integration problems
    - Better understanding of priorities
    - Faster knowledge sharing    


## Test-Driven Development 

**Test-Driven Development (TDD)** is a software development practice in which 
we **write tests before writing the code that implements the desired behavior**. 
It’s one of the core engineering practices advocated by Kent Beck.

The TDD Cycle:
* **Red**: Write a failing test
    - We write a small, specific test that describes a behavior we want.
    - The test fails because the feature doesn’t exist yet.
    - This ensures the test is valid.

* **Green**: Write the simplest implementation to pass the test
    - We write only the code needed to make the test pass.
    - No architecture, no abstraction - just the minimum required behavior.

* **Refactor**: Clean up code and test
    - Improve design: remove duplication, clarify names, simplify logic.
    - Since the test now passes, we can refactor with confidence.
    
We repeat this cycle dozens of times per day.

**TDD is not just about testing**,  it’s primarily about:

* **Better design**: TDD forces us to design from the outside in:
    - How the code will be used
    - What the behavior should be
    - What the API should look like before we implement it.

* **Continuous and fast feedback**: We know within seconds when changes 
    break something.

* **High confidence to change or refactor**: A comprehensive test suite 
    acts as a safety net.

* **Smaller, cleaner, more modular code**: Because tests enforce 
    small steps and good separation of concerns.

* **Fewer bugs**: Many defects never appear because tests express 
    the intended behavior upfront.


## Pair Programming 

Pair programming is a core technique used in **Extreme Programming (XP)**, 
a type of agile software development methodology. It involves
**two programmers working together at one workstation to develop a software component**. 

Here’s how it typically works:

* **Roles**: The two programmers assume distinct roles:
    * The **Driver** writes the code, focusing on the implementation of 
    a specific task. This role involves typing out the code but also involves 
    immediate decision-making about the details of the coding.
    * The **Navigator** reviews the code as it's written, thinking about the 
    big picture, considering long-term implications, spotting errors, and 
    suggesting improvements. The navigator might also research solutions 
    to problems or consult documentation as the driver codes.

* **Collaboration**: Throughout the process, the two programmers continuously 
    communicate, discussing strategies, solutions, and potential problems. 
    This constant dialogue is critical for spotting mistakes early, brainstorming 
    solutions, and ensuring that the code adheres to the project's standards and goals.

* **Switching Roles**: Periodically, the pair might switch roles. This practice 
    ensures that both programmers stay engaged and gain a deeper understanding of 
    the codebase. It also promotes knowledge transfer, as each programmer brings 
    their own skills and insights to the project.

* **Benefits**: Pair programming is believed to enhance code quality, reduce bugs, 
    foster knowledge sharing, and accelerate the development process. 
    It’s particularly effective for complex or critical tasks that benefit from 
    diverse perspectives. It also aids in creating a collective code ownership and 
    improves the skills of both programmers through continuous feedback.

* **Challenges**: Despite its benefits, pair programming can be challenging. 
    It requires excellent communication skills, compatibility between team members, 
    and a willingness to collaborate closely. 
    Moreover, it might not be as effective for simple tasks that don’t require much 
    discussion or for programmers with significantly different skill levels.

* **Adaptability**: While traditional pair programming involves two programmers working 
    side by side, modern practices have adapted to include remote pair programming, 
    where programmers collaborate online using shared coding environments and 
    communication tools.

In summary, pair programming is a **collaborative approach** that leverages the skills 
and insights of two programmers working in tandem to **improve code quality**, **enhance 
learning**, and **accelerate development**. It embodies the principles of Extreme 
Programming by emphasizing teamwork, feedback, and continuous improvement.


### GenAI Integration 

From the practical perspective, we have to integrate GenAI into the 
**Software Development Cycle (SDL)**.

A promising approach is to use **GenAI as a programming pair**:

**GenAI can play both roles in the context of pair programming**, albeit with 
some adjustments and considerations: 

* **As the Driver**
    * **Writing Code**: GenAI can generate code snippets based on specific 
        instructions, similar to a human driver. You can ask it to write functions, 
        debug code, or implement algorithms.
    * **Implementing Solutions**: It can take a set of requirements and turn them 
        into a working piece of code, offering various solutions or alternatives 
        when possible.
    * **Following Directions**: Just as a human driver would, GenAI can follow 
        the navigator's strategic directions, implementing the ideas and feedback 
        it receives.

* **As the Navigator**
    * **Reviewing Code**: While GenAI can review code to some extent, its ability 
        to catch complex bugs or understand deep implications of certain implementations 
        in real-time is limited compared to a human expert. 
        It can, however, suggest best practices and identify simple syntax or logical errors.
    * **Providing Feedback**: It can offer insights on code optimization, readability, 
        and adherence to programming standards. GenAI can also suggest improvements or 
        alternative approaches to a problem.
    * **Research and Documentation**: GenAI can provide explanations, documentation 
        references, and examples for a wide range of programming concepts and languages, 
        aiding in the research part of the navigator's role.

* **Limitations and Considerations**
    * **Real-Time Collaboration**: GenAI’s static nature means it cannot dynamically 
        interact in real-time like a human pair. The feedback loop is slower, as you need 
        to input queries and wait for responses.
    * **Contextual Understanding**: While it can understand and retain context to a degree 
        within a conversation, its ability to keep track of an evolving codebase or project 
        intricacies in real-time is limited compared to a human.
    * **Complex Debugging**: For more complex debugging tasks, especially those that require 
        understanding of the broader system or external dependencies, GenAI's capabilities 
        may not be as effective as a human expert's.

GenAI can play roles akin to both the driver and the navigator in pair programming, 
providing a valuable resource for coding, learning, and problem-solving. 
However, **its effectiveness is maximized when used as a complement to human expertise**, 
rather than a complete substitute, due to the dynamic and complex nature of software 
development tasks.


## Trunk-Based Development

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


### Why the Branching Model Matters

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


### Branching Models Compared

#### GitFlow / Feature Branching

Every feature is developed on its own branch and merged when "done".
Common in open-source and in teams with gated review processes.

* Branches routinely live for **days or weeks**.
* Integration is deferred to merge time.
* Release branches, hotfix branches, and a separate `develop` branch add
    process overhead.
* **Not compatible with Continuous Integration** in the strict sense.

#### Trunk-Based Development

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


### Keeping the Trunk Releasable

If everyone commits to the trunk, the trunk must **never be broken** and must
**always be releasable**. Unfinished work therefore has to be committed in a
way that does not break users of the system. Several techniques make this
possible.

#### Work in Small Batches

Break every task into a sequence of small, safe steps that each keep the
system working. A large feature becomes many commits, each of which
compiles, passes its tests, and could in principle be released. This is a
skill; it is learned by practising it.

#### Branch by Abstraction

A technique for making a **large change incrementally on the trunk**:

1. Introduce an **abstraction layer** in front of the code you want to
    replace.
2. Move clients over to the abstraction, one at a time, committing after
    each.
3. Build the new implementation behind the same abstraction.
4. Switch clients to the new implementation.
5. Remove the old implementation and, if no longer needed, the abstraction.

At every step the trunk is consistent and shippable.

#### Feature Toggles (Feature Flags)

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

#### Keystone / Dark Launching

Build the feature bottom-up and connect the **user-visible entry point
last** ("the keystone"). The code is fully present and tested on the trunk;
only the final wiring that exposes it to users is withheld until release.


### Releasing from the Trunk

* **Continuous Deployment**: every green commit on the trunk is deployed
    automatically. Requires a strong pipeline and, usually, feature toggles.

* **Release branches**: for products with discrete versions (typical for
    embedded firmware), a **release branch is cut from the trunk** at release
    time. Development continues on the trunk; only critical fixes are
    *cherry-picked* onto the release branch. Release branches are for
    stabilisation and patching, never for feature development.


### Code Review under Trunk-Based Development

Fast integration and thorough review can coexist:

* **Pair programming** is continuous review; a separately reviewed pull
    request is then unnecessary.

* **Short-lived pull requests** with a review turnaround measured in
    **minutes to a few hours**, not days. A slow review process silently
    turns TBD back into feature branching.

* **Post-commit review** for trusted, experienced teams: commit to trunk
    now, review shortly after, backed by the safety net of the pipeline.


### Relevance for Embedded Systems

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


### Common Pitfalls

* Calling it TBD while pull requests routinely live for days.
* A commit-stage build that takes longer than ~10 minutes, so developers
    batch up changes to avoid waiting.
* Committing broken code and relying on someone else to fix the trunk.
* Feature toggles that are never removed.
* Using release branches for new features "just this once".


### Practices Checklist

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
