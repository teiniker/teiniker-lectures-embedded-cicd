# Example: Analyze and Refactor an Existing Project

## Analyze a Given Project

Before Claude can find or fix anything, it needs to understand what
the project actually does. 

Where reading alone cannot confirm a claim, for example whether an
endpoint really returns the status code the README shows, Claude runs
the project and exercises it with the same `curl` commands the README documents, comparing the real response to what was actually written. 
For a larger codebase where the relevant files are not known in 
advance, this reading step can be delegated to a fast, read-only 
Explore agent that locates them first, so the main analysis is not 
spent on broad directory search.

_claude>_ **Analyze the following example:
    introduction/agentic-engineering/examples/documentation/book-service**

_claude>_ **In this directory, write an Analysis.md file with your
    findings.**

_claude>_ **Add a class diagram in Mermaid format to Analysis.md**


## Fix Problems

From the list of problems, we can pick one by one to fix them:

1. **#1 (id generation)**: a real bug that the example's own POST request
   triggers.
2. **#8 (debug mode)**: the most important security issue.
3. **#2 and #3 (validation and error handling)**: make the API robust
   against bad input.
4. **#10 (tests)**: needed before refactoring (#4 to #7).

_claude>_ **Fix #1 (id generation)**

After the coding agent has fixed the problem, we can review these changes 
as a diff to the repository version (use VS Code to get a git diff).

So we can handle all these problems and verify the applied fixes.

*Egon Teiniker, 2026, GPL v3.0*
