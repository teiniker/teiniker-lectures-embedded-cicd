# Continuous Delivery in Embedded Systems

This repository contains examples and documentation for using Continuous Integration and Continuous Delivery in embedded systems.

![CI/CD Process](figures/CICD-Process.png)

* [Introduction](introduction/README.md)

* [Continuous Integration](continuous-integration/)

* [Continuous Delivery](continuous-delivery/)


## Use of Coding Agents
This repository was developed using coding agents to assist with design 
discussions, implementation, and documentation. 
All materials have been manually reviewed and verified.


## Python Examples 

To use the Python examples in this repository, we must create a virtual 
environment and install the necessary libraries.

```bash
# Activate environment
$ python -m venv .venv
$ source .venv/bin/activate

# Install libraries
$ pip install -r requirements.txt

$ pip list

# Deactivate environment
$ deactivate
```

## References

* Martin Fowler. [**Continuous Integration**](http://martinfowler.com/articles)
* Jez Humble, Davis Farley. **Continuous Delivery**. Addison-Wesley, 2010
* Davis Farley. **Continuous Delivery Pipelines**. Independently published, 2021

_Egon Teiniker, 2025-2026, GPL v3.0_
