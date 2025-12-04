# User Stories 

In the context of Agile development, a user story is a **short, 
simple description of a feature** or requirement told from the 
perspective of the end user. 

It helps the development team remember what the user wants and 
why, without going into technical details.

### [Requirement Specification vs User Stories](https://youtu.be/KP0U3I-f9-Y?si=CYUPhv30zrW0UnxE)

Requirement (IEEE Definition): A condition or capability needed by a user to 
solve a problem or achieve an objective.

Requirements = Guesses 

User Story is not UI Design, it defines user's mental model 
- What is my Goal?
- What do I need to be Aware of?
- What do I need to know when it breaks?

Story:
- Story Title
- Character (Role) 
- Narrative (what to do)
- Acceptance Criteria	

Good User Stories:
- Describe what a user wants the software to do
- Don't include technical specifications 
- Test stories, by confirming they work for other solutions
- Write in the language of the user, not the system 
- Test stroies, by checking they are understandable to all. 


### [5 Common Mistakes In User Stories](https://youtu.be/0HMsh459h5c?si=4uK5R1WJfHeIbPtf)

A User Story is: 
- a description of the problem we aim to solve.
- a placeholder for a conversation
- a small increment in the behaviour of the system.
- focused on WHAT the system is meant to do not HOW it does it.
- atomic! They can be implemented in any order

Mistakes: 
- Remote Control Programming (don't describe coding or design practices)
- Stories as a Contract (US should be a **simple** description of the problem)
- Monster Stories 
- "Value" to a User means "Valuable"
- Dependencies between stories 


* Gojko Adzic, David Evans. **Fifty Quick Ideas To Improve Your User Stories**. Neuri Consulting LLP, 2013-2015, Kindle-Version.

### [How to write good User Stories in Agile](https://youtu.be/7hoGqhb6qAs?si=miYIX2hQkc-BCQOm)

User Story Format:
```
    As a <user role>, 	// who 
    I want to <action>, // what 
    so that <value>.	// why 
```

Acceptance Criteria:
```
    Given <context>		// pre-condition 
    When <action>		// some action is carried out 
    Then <outcome> 		// observable outcome and expected results 
```

Good and effective US:
- Independent: developed and released on its own (in any order)
- Negotiable: methods by which that goal is achieved should be negotiable 
- Valuable: each story should increase product value  
- Estimable: can be sized by the team 
- Small: complete within an iteration
- Testable: in a binary way 


### [What are Agile Epics, User Stories, and Story Points?](https://youtu.be/ogV2r9579WI?si=evbsrGYdnWaXT-AX)

User Story: Describes one product requirement that the user wants. All US together forms the Backlock 
```
    As a <user/persona> 
    I want to [action/task]
    so that [benefit]
```

Definition of Done: 
```
    When I [action]
    This happens [outcome]
```

Epic: 
- Is a big user story which cannot be implemented in one iteration 
- We split the Epic down into some User Stories (closely related)
- Actually there is no clear border between Epic and User Story 

Story Points 		 
- Are related tu User Stories 
- Are unitless measures of the amount of work required to deliver a User Story 
- They scale the size of the task (relative measure)
- When allocating Story Points we use 
    - Amount of work 
    - Complexity 
    - Risk / Uncertainty 


## References

* Mike Cohn. **User Stories Applied**. Addison-Wesley, 2004
    - Chapter 1: An Overview 
    - Chapter 2: Writing Stories 
    - Chapter 3: User Role Modeling 
    - Chapter 6: Acceptance Testing User Stories 

_Egon Teiniker, 2025, GPL v3.0_