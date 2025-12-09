# Executable Specifications

Executable Specifications are requirements written in a format that 
is clear to humans and directly verifiable by automated tests.


## User Stories 

In the context of Agile development, a user story is a short, 
simple description of a feature or requirement told from the 
perspective of the end user. 

> A User Story is a chunk of functionality (feature) that is of 
> value to the customer.

It helps the development team remember what the user wants and 
why, without going into technical details.

A User Story is **focused on WHAT the system is meant to do not 
HOW it does it**.

Over the years, the following templates have become established:

* **User Story Format**:

    ```
    As a <user role>, 	// who 
    I want to <action>, // what 
    so that <value>.	// why 
    ```

* **Acceptance Criteria Format**:

    ```
    Given <context>		// pre-condition 
    When <action>		// some action is carried out 
    Then <outcome> 		// observable outcome and expected results 
    ```

Good and effective User Stories have the following **characteristics**:

* **Independent**: Developed and released on its own (in any order)

* **Valuable**: Each story should increase product value

* **Estimable**: User Storiy can be sized by the team 

* **Small**: A small increment in the behaviour of the system, complete 
    within an iteration

* **Testable**: A User Story shoul be testable as a black box (in a 
    binary way) 


The following terms are frequently encountered in connection with user 
stories:

* **Epic**: 
    - Is a big user story which cannot be implemented in one iteration 
    - We split the Epic down into some User Stories (closely related)
    - Actually there is no clear border between Epic and User Story 

* **Story Points**:
    - Are unitless measures of the amount of work required to deliver a User Story 
    - They scale the size of the task (relative measure)
    - When allocating Story Points we use: 
        - Amount of work 
        - Complexity 
        - Risk / Uncertainty 


### Common Mistakes In User Stories

The following **common mistakes** can occur when using user stories:: 

* **Remote control programming**: Don't describe coding or design practices in US

* **Stories as a contract**: US should be a **simple** description of the problem

* **Monster stories**: US requirements must not be too extensive so that 
    they can be implemented in one iteration. 

* **Dependencies between stories**: US should be able to be implemented independently 
of each other.


## From User Stories to Acceptance Test Cases 

The relationship between User Stories (US) and Acceptance Tests is tight and intentional. 

They are two **complementary views of the same requirement**:

* **A User Story expresses what the user needs and why**
    - It is a short, human-readable description of a requirement.
    - It captures intent, not details or implementation.

    _Example_:
    _As a_ shopper, _I want_ to add items to my cart _so that_ I can buy them later.

    A User Story alone is not enough to begin development; it sets the context but 
    not the conditions of success.

* **Acceptance Tests express how we know the story is satisfied**:
    - They translate the user’s intent into clear, **testable conditions**.
    - They define the boundaries and expected behavior of the story.
    - They remove ambiguity by making implicit assumptions explicit.

    _Example_: Scenario: Add item to cart\
    _Given_ I am viewing a product page, _When_ I click "Add to cart", 
    _Then_ the item appears in my shopping cart.

    Acceptance Tests define the **Definition of Done** for the User Story.
    A User Story is done only when all its acceptance tests pass.


## Behavior-Driven Development 

A **Domain-Specific Language (DSL)** can play a major role in bridging the 
gap between Acceptance Test Cases and their implementation. 

In **Behavior-Driven Development (BDD)**, where tests express behavior in 
business language, but must still be executable by software.

* A DSLs provide a structured format that can be automatically mapped to code.

* A DSL is formal enough that tools can interpret it.

* A DSL decouples test intent from technical implementation.

There are explicit BDD frameworks, like [Cucumber](https://cucumber.io/) 
that define their own language (**External DSL**).

In practice, however, abstractions such as the **Page Object Pattern** 
are often used, which are implemented within an existing programming 
language (**Internal DSL**).


## Executable Specifications

An Executable Specification is a requirement or **behavior description** 
written in a structured, testable format so that it can be **automatically 
checked by a machine**.

* **They eliminate ambiguity**: Because they must be precise enough 
    to execute, executable specifications force clarity about behavior.

* **They act as both documentation and tests**: They are always up to 
    date because they fail when the system no longer meets the spec.

* **They align business and engineering**: They are written in domain 
    language but backed by code execution.

* **They reduce maintenance**: Specification and test are one artifact, 
    not two separate ones that can drift apart.


## References

* User Stories:
    - [YouTube (Mike Clayton): What are Agile Epics, User Stories, and Story Points?](https://youtu.be/ogV2r9579WI?si=evbsrGYdnWaXT-AX)
    - [YouTube (Modern Software Engineering): 5 Common Mistakes In User Stories](https://youtu.be/0HMsh459h5c?si=4uK5R1WJfHeIbPtf)
    -  [YouTube (The Agile Shop): How to write good User Stories in Agile](https://youtu.be/7hoGqhb6qAs?si=miYIX2hQkc-BCQOm)

    - Mike Cohn. **User Stories Applied**. Addison-Wesley, 2004
        - Chapter 1: An Overview 
        - Chapter 2: Writing Stories 
        - Chapter 3: User Role Modeling 
        - Chapter 6: Acceptance Testing User Stories 

* Behavior-Driven Development:
    - [Cucumber](https://cucumber.io/)

* Executable Specification:
    - [YouTube (Modern Software Engineering):Acceptance Testing with Executable Specifications](https://youtu.be/knB4jBafR_M?si=w16bhno3R2qoFFWU)   


_Egon Teiniker, 2025, GPL v3.0_