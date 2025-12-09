# Release Into Production


**The Deployment Pipeline is the only route to Production**.


## Production Environment 

This is the **final stage of our Deployment Pipeline**. 
We have done everything that we can think of to evaluate our Release 
Candidate, so that we have assurance that it is as safe, as it can be, 
to be pushed into Production.

We know that the deployment works because we have already configured 
the environment, deployed this version of the Release Candidate many 
times, and carried out **smoke tests** so that we know it is up and 
running and ready for use.

There is always the chance that we may get **unexpected problems**, 
there may be things that we have missed, simply because we did not think
of them - another opportunity to learn.


## Continuous Delivery vs. Continuous Deployment 

* In **Continuous Delivery**, we work so that our software is always 
    in a releasable state.
    We are free to make a **business decision** about when it makes most 
    sense to deploy, depending on the nature of the product, any risks 
    or safety considerations, the impact on the customers, etc.

* **Continuous Deployment**, on the other hand, means that the 
    **deployment is automated**, so, as soon as the Deployment Pipeline 
    determines that our Release Candidate is good, it is automatically 
    deployed into Production.


## Release Strategies

In an ideal world, we would be able to switch from the old version of 
the software to the new version, without any interruption of service. 
In the real world, we want to **manage this transition**. 

Here are some of the common approaches:

* **Blue/Green Deployment**: We work with two different versions of the system:
    we update one while the other is in operation, and switch over when we are 
    ready to deploy.

* **Rolling Transition**: Both the old and new versions are live, and we gradually 
    migrate traffic to the new system, until the old system is no longer used.

* **Canary Release**: We start by deploying into low risk: low volume environments 
    and, once assured that the changes are safe, we progressively deploy into more 
    and higher risk environments.

* **A/B Tests**: We deploy two different versions of the system, monitor both, 
    and compare the impact in Production, on customers, and for the business.


## Feedback from Production

To learn about our software we want to monitor feedback from our Production systems. 

We can monitor:

* **Technical Information**: Memory usage, disk usage, CPU usage, queue depth, error 
    logs, and performance measures.

* **Functional Information**: The business performance of the system, data on user 
    journeys, A/B testing, and other business metrics.

**We continue to learn**. We gather information to inform business decisions.
We get feedback to understand what our customers make of our software.

The **Deployment Pipeline automates the collation of data**, to provide rapid, 
quantifiable feedback and this should continue once we are in Production.

Immediately following the release into Production, we may use **smoke tests** 
to validate that everything is ready for use. The acceptance test DSL is a great 
tool for this.


## References

* Jez Humble, Davis Farley. **Continuous Delivery**. Addison-Wesley, 2010
* Davis Farley. **Continuous Delivery Pipelines**. Independently published, 2021

_Egon Teiniker, 2025, GPL v3.0_    