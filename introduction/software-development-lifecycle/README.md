# Software Development Lifecycle

> A **software process** is a set of activities (requirements specification,
> design, implementation, verification, evolution) and their results that
> together produce a software product. A **process model** is a simplified
> description of such a process from one point of view.

Different kinds of systems need different processes. Safety-relevant
embedded software in a vehicle is often specified in full before
implementation begins. An e-commerce service is typically specified and
built together, in short cycles. This section reviews the models the rest
of the course builds on.


## Phases of Software Development

The recurring activities of any software process, independent of the model
that orders them:

* **Requirements**: Capture and agree on what the system must do.
* **Design / Architecture**: Decide how it will be structured.
* **Implementation**: Write the code.
* **Verification & Validation**: Check it is built right and is the right
    thing.
* **Deployment**: Put it into the hands of users.
* **Evolution / Maintenance**: Change it after release.


## Linear Models

Phases run once, in sequence - each is completed before the next begins.

* **Waterfall Model**: A strictly sequential flow. Produces documentation
    at every phase and fits classic engineering practice, but partitions the
    project rigidly and tolerates change badly. Appropriate only when
    requirements are well understood and stable.

* **V-Model**: Waterfall folded into a V, pairing each design phase on the
    left with the test phase that verifies it on the right (requirements ↔
    acceptance testing, architecture ↔ integration testing, module design ↔
    unit testing). Encourages early test planning; still needs fully defined
    requirements up front.


## Agile Models

Work proceeds in small increments with frequent feedback and adaptation.

* **[Agile Manifesto](introduction/README.md)**: Four values and twelve principles:
    individuals and interactions, working software, customer collaboration,
    and responding to change.

* **[Scrum](scrum/README.md)**: An iterative, incremental management
    framework. Fixed-length sprints, the roles Product Owner / Scrum Master
    / Team, and the artifacts Product Backlog, Sprint Backlog, and Increment.

* **[Extreme Programming (XP)](extreme-programming/README.md)**: A
    lightweight engineering methodology for small teams facing vague or
    changing requirements. Practices include TDD, pair programming,
    refactoring, collective ownership, and **continuous integration**.

* **[Kanban](kanban/README.md)**: A pull-based method that visualizes the
    workflow, limits work in progress, and optimizes flow. Fits a continuous
    CI/CD pipeline more naturally than time-boxed sprints.


## Relation to the Rest of the Course

Continuous Integration and Continuous Delivery are engineering practices,
not a process model: they work with Scrum, XP, or Kanban. XP is where CI
originates as a named practice, and Kanban's continuous flow maps directly
onto a deployment pipeline. 

## References

* Ian Sommerville. **Software Engineering**. Addison-Wesley, global edition, 2015
* Kent Beck. **Extreme Programming Explained**. Addison-Wesley, 2000
* Ken Schwaber, Mike Beedle. **Agile Software Development with Scrum**. Prentice Hall, 2002
* David J. Anderson. **Kanban: Successful Evolutionary Change for Your Technology Business**. Blue Hole Press, 2010
* [Manifesto for Agile Software Development](https://agilemanifesto.org/)

_Egon Teiniker, 2025-2026, GPL v3.0_
