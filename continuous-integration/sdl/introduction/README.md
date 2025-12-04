# Software Development Lifecycle

A **software process** is a set of activities (requirements specification, 
design, implementation, verification, evolution) and associated results that 
produce a software product.

See: [Phases of Software Development](https://github.com/teiniker/teiniker-lectures-embedded-softwaredesign/blob/master/introduction/README.md)

Different types of systems need different development processes:
- Real-time software in a car has to be completely specified before 
    development begins
- In e-commerce systems, the specification and the program are  
    usually developed together.

> A **software process model** is a simplified description of a software process 
> that represents one view of that process.
> Process models may include activities that are part of the software process 
> and the roles of people involved in software engineering.

There are general paradigms of software development:
- **The waterfall approach**: This takes the fundamental process activities and 
    represents them as separate process phases. 
- **Iterative development**: This approach interleaves the activities of specification, 
    development and validation. 
- **Component-based software development**: This approach is based on the existence 
    of a significant number of reusable components.


## Waterfall Model

The Waterfall Model is one of the earliest and most traditional software development 
life cycle (SDLC) approaches. 

> The **Waterfall Model** follows a linear and sequential flow, where each phase must 
> be fully completed before the next one begins. 
> Like water flowing downward through steps of a waterfall.

Phases of the Waterfall Model:

* **Requirements Analysis**:
    Collect and document all system requirements.

* **System Design**:
    Create the architecture, data models, and system 
    design based on requirements.

* **Implementation (Coding)**:
    Developers write the code according to the design 
    specifications.

* **Integration & Testing**
    Integrate modules and test the complete system for 
    defects, performance, and compliance.

* **Deployment**:
    Release the system to users or customers.

* **Maintenance**:
    Fix issues, apply updates, and support the software 
    after deployment.

The **advantages** of the waterfall model are that documentation 
is produced at each phase and that it fits with other engineering 
process models.

Its **major problem** is its inflexible partitioning of the project 
into distinct stages.

**The waterfall model should only be used when the requirements 
are well understood and unlikely to change radically during system development**.


## V-Model



## Agile Methods


### Agile Manifesto 

The Agile Manifesto (2001) is the **foundational document for Agile 
software development**. It outlines 4 core values and 12 guiding 
principles that emphasize flexibility, collaboration, and continuous 
improvement.

The **Core Values** of the Agile Manifesto:

* **Individuals and interactions over processes and tools**:
    Agile prioritizes people—communication, teamwork, and 
    collaboration—because good individuals can adapt even when 
    tools or processes fall short.

* **Working software over comprehensive documentation**:
    Documentation matters, but delivering functional software 
    is more valuable. Agile avoids excessive paperwork that 
    slows progress.

* **Customer collaboration over contract negotiation**:
    Instead of locking everything into rigid contracts, Agile 
    focuses on continual feedback and partnership with customers.

* **Responding to change over following a plan**:
    Agile embraces change, even late in development, because 
    adapting leads to better results than forcing outdated plans.

The **Principles** of the Agile Manifesto:

* **Customer satisfaction through early and continuous delivery**:
    Deliver value frequently so customers see progress and 
    give feedback.

* **Welcome changing requirements, even late**:
    Change is a competitive advantage in Agile.

* **Deliver working software frequently**:
    Short, incremental cycles (iterations/sprints).

* **Close collaboration between business and developers**:
    Daily engagement ensures alignment and reduces 
    misunderstandings.

* **Build projects around motivated individuals**:
    Give teams the environment, support, and trust they need.

* **Face-to-face communication is best**:
    Direct communication reduces delays and improves clarity.

* **Working software is the primary measure of progress**:
    Output is tangible value—not documents or hours worked.

* **Promote sustainable development**:
    Teams should maintain a consistent pace indefinitely 
    (no burnout cycles).

* **Continuous attention to technical excellence and good design**:
    Quality enables faster development over time.

* **Simplicity, the art of maximizing work not done, is essential**:
    Focus on what truly matters; avoid over-engineering.

* **Self-organizing teams create the best designs and solutions**:
    Teams decide how to work; autonomy fosters innovation.

* **Regular reflection and adaptation**:
    Retrospectives help teams continuously improve processes 
    and collaboration.


### Scrum 

> **Scrum** is a management and control process that cuts through complexity 
> to focus on building software that meets business needs.

Using Scrum, teams develop products **incrementally and empirically**.



### Extreme Programming 

> **Extreme Programming (XP)** is a lightweight methodology for 
> small-to-medium-sized teams developing software in the face of 
> vague or rapidly changing requirements.

XP is designed to work with projects that can be built by teams 
of **two to ten programmers**.



### Kanban



## References

* Scrum
    - Ken Schwaber, Mike Beedle. **Agile Software Development with Scrum**. Prentice Hall, 2002
    - Ken Schwaber. **Agile Project Management with Scrum**. Microsoft Press, 2004


* XP
    - Kent Beck. **Extreme Programming Explained**. Addison-Wesley, 2000
    - Kent Beck, Martin Fowler. **Planning Extreme Programming**. Addison-Wesley, 2001

* Agile Methods 
    - [Manifesto for Agile Software Development](https://agilemanifesto.org/)

_Egon Teiniker, 2025, GPL v3.0_