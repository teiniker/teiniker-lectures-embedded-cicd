# Acceptance Stage 

The transition from the Commit Cycle to the Acceptance Cycle represents a shift 
from a technical, developer-centred focus, to considering whether our code does 
what the users of the software want it to do. 

![CD Process](figures/CD-Process.png)

We carry out evaluations from the **perspective of the external users** of the 
system, until we achieve enough confidence that we are ready to release.

Aims of the Acceptance Stage:

* To evaluate our changes from an external user perspective

* To test in life-like scenarios

* To evaluate in production-like test environments

* To eliminate the need for manual regression testing

* To achieve a sufficient level of confidence that our software is 
    functionally ready for production


## Acceptance Tests

Before running the tests, we first deploy the Release Candidate to 
a production-like environment so that it is ready for use.

Here are the steps implemented in an Acceptance Test strategy:

* **Configure the environment**, ready for the Release Candidate

* **Deploy the Release Candidate**

* Carry out a **Smoke Test** to ensure that the Release Candidate is up 
    and running

* **Run Acceptance Tests**


Effective Acceptance Tests:

* Are written from the **perspective of an external user** of the system

* Evaluate the system in **life-like scenarios**

* Are evaluated in **production-like test environments**

* Interact with the System Under Test (SUT) through **public interfaces** 
(no back-door access for tests)

* Focus only on **WHAT** the system does, not HOW it does it


The most effective way to create an Acceptance Test is to write an 
**Executable Specification**, that describes the desired behaviour of the 
new piece of software, before we write any code.

We work to fulfil these Executable Specifications as we carry out 
lower-level, TDD testing until the specification is met.

We make the **scenarios that these tests capture atomic**, and don’t share 
test-data between test cases. **Each test-case starts from the assumption 
of a running, functioning system, but one that contains no data**.


### Four-Layer Approach 

Dave Farley recommends a four layer architecture to Acceptance Test infrastructure
so that the system can change, without invalidating the test cases.

![Four-Layer Approach ](figures/FourLayerModel-AcceptanceTests.png)

_Figure: Four layer acceptance testing approach (Farley, 2021)_ 

* **Executable Specifications**: 
    We capture what we expect the software to do, and not how it should do it.
    The Executable Specifications should be **readable by a non-technical person** 
    who understands the problem domain.

* **Domain Specific Language (DSL)**:
    This layer supports the easy creation of Executable Specifications. This 
    layer enables consistency, re-use and efficiency in the creation of 
    Acceptance Test Cases.

    We aim to grow the DSL pragmatically, i.e. create two or three simple 
    test-cases that exercise the most common / valuable behaviour of the 
    system, and create the infrastructure that allows these tests to execute, 
    and to pass.

* **Protocol Drivers**:
    Protocol Drivers sit between the DSL and System Under Test (SUT) and 
    **translate from the high-level language of the problem domain** 
    to the language of the system, using more detailed concepts.

    We isolate all knowledge of how to communicate with the system here.
    Protocol Drivers **encode real interactions with the SUT** and are the 
    only part of the test infrastructure that understand how the system 
    works.

* **System Under Test (SUT)**:
    We deploy the system using the **same tools and techniques as we will 
    use to deploy it in Production**. It is important to delineate the SUT 
    from systems that other people are responsible for, in particular, 
    third-party dependencies.
    
    We **fake all its external dependencies**, so we can thoroughly test 
    to the boundaries of the system that we are responsible for.

We aim to **eliminate the need for manual regression testing**, and automate 
any and all repeatable processes, in the Deployment Pipeline.

**Manual processes** are slow, unreliable and expensive.
We should use human beings where they can have the best effect, in **creative 
processes and in qualitative assessments**.

Acceptance Tests take time and can be expensive.
We can design the test infrastructure to **run multiple tests in parallel**.


### Best Practices 

* Incorporate Acceptance Tests into the development process from the start.

* Create an **Executable Specification** for the desired behaviour of each 
new piece of software before starting on the code.

* Think of the least technical person who understands the problem-domain, 
reading the Acceptance Tests. The tests should make sense to that person.

* Create a new **Acceptance Test for every Acceptance Criteria for every 
User Story**.

* Make it easy to identify Acceptance Tests and differentiate them from 
other sorts of tests.

* Automate control of test environments, and control the variables, so 
the tests are reproducible.

* Make it easy for development teams to run Acceptance Tests and get 
results, by automating deployment to the test environment and automating 
control of test execution.

* **Automate the collection of results**, so developers can easily get 
the answer to the question "Has this Release Candidate passed 
Acceptance Testing?"

* **Don’t chase test coverage as a goal**. Good coverage comes as a 
side-effect of good practice, but makes a poor target.


## Manual Tests 

There is a valuable role for Manual Testing. It is just that manual regression 
testing is slow, unreliable and expensive, and not a good use of human skills.

We can use our **computers to carry out routine, repeatable tasks** more 
reliably and quickly, and **free up people to fulfil the more valuable, more 
appropriate role of exploratory testing**.

This plays to human strengths, like ‘fuzzy pattern matching’. 
We want people to evaluate the software and make a subjective assessment of how 
easy and enjoyable it is to use, and to spot 'silly' mistakes early on.

We only make Release Candidates that have passed acceptance testing available 
for Manual Testing.

Manual Testing is **not an essential component** of every Deployment Pipeline.
However, it is useful for software with a significant UI component, where 
a broader **assessment of the usability** of a system is helpful.

The **Manual Testers** cannot work to the same timescale as automated acceptance 
testing, they are **too slow**. 
It may be better to think about Manual Testing **running in parallel** with the 
developer's work.


## References

* Jez Humble, Davis Farley. **Continuous Delivery**. Addison-Wesley, 2010
* Davis Farley. **Continuous Delivery Pipelines**. Independently published, 2021

_Egon Teiniker, 2025, GPL v3.0_    