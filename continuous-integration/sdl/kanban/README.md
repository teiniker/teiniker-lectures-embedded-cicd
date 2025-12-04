# Kanban 

The **Kanban development process** is a popular method for managing and 
improving software development workflows. It’s rooted in Lean and Agile 
principles and focuses on **visualizing work**, **limiting work in 
progress (WIP)**, and **maximizing flow efficiency**.


> **Kanban** (Japanese for “visual sign” or “card”) is a 
> **visual workflow management method** used to visualize work, 
> identify bottlenecks, and improve flow.


The Kanban method, as applied to software and knowledge work, has roots 
in **Lean manufacturing** but was adapted and popularized in tech much later.

* **Taiichi Ohno**, an industrial engineer at Toyota, is considered 
    the father of the original Kanban system.
    
    In the 1940s-1950s, he developed Kanban as part of the **Toyota 
    Production System (TPS)** to improve manufacturing efficiency.

    His system used visual cards (Kanban) to signal when new parts should 
    be produced, reducing overproduction and waste — key Lean principles.

* **David J. Anderson** is the key figure who adapted Kanban to knowledge 
    work and software development around 2007.

    He combined ideas from Lean, Theory of Constraints, and Agile, 
    and introduced a flexible, evolutionary approach to process improvement.

    His 2010 book, **Kanban: Successful Evolutionary Change for Your 
    Technology Business**, is the foundational text for the Kanban method 
    in software.


## Core Principles

1. **Visualize the Workflow**  
   - Use a **Kanban board** to represent the process.
   - Work items are represented as **cards**, and columns represent 
    **steps in the process** (e.g., To Do, In Progress, Done).

2. **Limit Work in Progress (WIP)**  
   - Set limits on how many items can be in each stage at a time.
   - This prevents teams from overloading themselves and helps focus 
   on finishing tasks rather than starting too many.

3. **Manage Flow**  
   - Continuously monitor how work moves through the system.
   - Aim to reduce lead time (the time from start to finish) and 
   improve throughput.

4. **Make Process Policies Explicit**  
   - Everyone should understand how the process works, what the rules 
   are, and what “Done” means.

5. **Implement Feedback Loops**  
   - Regular reviews (like daily standups or retrospectives) help 
   identify areas for improvement.

6. **Improve Collaboratively, Evolve Experimentally**  
   - The system is continuously refined based on feedback and 
   experimentation.


## How a Kanban Board Works

A basic Kanban board might look like:

```
| To Do | In Progress | Code Review | Testing | Done |
```

Each work item (e.g., a user story or task) is a **card** that moves from 
left to right across the board.


## Benefits of Kanban

- **Flexibility**: You can change priorities and work items on the fly.
- **Improved Focus**: Limiting WIP encourages completing tasks before starting new ones.
- **Visual Clarity**: Everyone can see what’s being worked on and what’s blocked.
- **Better Flow**: Helps teams identify bottlenecks and inefficiencies.


## Kanban vs Scrum (Quick Comparison)

| Feature         | Kanban                        | Scrum                        |
|-----------------|-------------------------------|------------------------------|
| Roles           | No defined roles              | Defined roles (Scrum Master, PO, Team) |
| Timeframes      | Continuous delivery           | Time-boxed sprints (e.g., 2 weeks) |
| Planning        | On-demand                     | Sprint planning              |
| Metrics         | Lead time, Cycle time         | Velocity, Burndown charts    |

* [YouTube (Dave Farley): Agile vs Kanban: Which is Better? GOTO 2022](https://youtu.be/N9coClM4qf8?si=IFNKwU5xnEEzYCIb)

* Flatten cost of change curve 
    - Optimize for Learning 
    - Proceed in samll steps 
    - Assume you are probably wrong 

* Kanban 
    - Kanban is a flow-based process - no release plan 
    - The unit of work is a single feature 
    - Break down the work into features 
    - Prioritize features 
    - Developers pick the next most important feature 
    - They work on this feature (only) until it is finished 

* Agile (Scrum) 
    - Planing and working is done for an iteration 

Kanban is more flexible, Agile is bound to the iteration cycles 		
Kanban fits best with CI/CD pipeline. 

Fokus auf Features und Durchsatz 

Frage: Wie passt das mit HW zusammen 

TODO: vgl. Toyota Stoy (wenn es für Toyota funktioniert, sollte 
es auch für HW funktionieren)



## Kanban and the CI/CD Pipeline

**Kanban often fits better with a CI/CD (Continuous Integration/Continuous 
Delivery) pipeline** than Scrum. 

CI/CD is all about **frequent, incremental changes** and **continuous flow 
of work** through development, testing, and deployment.
 Kanban naturally supports this with:

* **Continuous Flow over Time-boxed Sprints**
    - **CI/CD** doesn’t rely on fixed-length iterations. It thrives on 
    shipping when things are ready.
    - **Kanban** supports a continuous, pull-based system — perfect for 
    small, fast-moving changes.

* **Visualizing and Managing Flow**
    - Kanban makes it super easy to see where work is stuck (e.g., code in 
    testing or waiting for review), which is critical in fast-moving pipelines.

* **WIP Limits Reduce Bottlenecks**
    - Limiting how much work is “in progress” at once helps maintain high quality 
    and prevents piling up work that hasn’t been tested or deployed.

* **Faster Feedback Loops**
    - Since work is flowing continuously, developers get quicker feedback from 
    builds, tests, and users — aligning well with DevOps practices.


## Scrum in CI/CD? Still Useful, But…

Scrum can work with CI/CD, but it’s usually a better fit for teams that:

- Need structure and predictable planning cycles.
- Are working on features that benefit from Sprint-style time-boxed delivery.
- Prefer regular cadence over continuous delivery.

The potential friction is that **Scrum batches work** into sprints, while 
**CI/CD encourages shipping whenever something is done** — which matches Kanban's 
just-in-time philosophy better.

|             | Kanban                        | Scrum                         |
|-------------|-------------------------------|-------------------------------|
| Flow        | Continuous, pull-based        | Sprint-based, time-boxed      |
| CI/CD Fit   | ⭐⭐⭐⭐ (Great match)            | ⭐⭐ (Possible, but may need tweaks) |
| Feedback    | Fast, ongoing                 | At sprint end (unless adjusted) |

If we are building with CI/CD regularly, **Kanban is a natural companion**. 


## References


* [The Kanban Method | David J Anderson | Kanban Experts Series](https://youtu.be/FmZ-tAV-Gb0?si=xOYLZkCrsh9NJhLE)

* [The Kanban Method](https://kanban.university/kanban-guide/)

* [Kanban Flow](https://kanbanflow.com/gm/SD8X95cdWU#.X2i)

* David J. Anderson, Donald G Reinertsen. **Kanban: Successful Evolutionary Change for Your Technology Business**. Blue Hole Press, 2010

_Egon Teiniker, 2025, GPL v3.0_