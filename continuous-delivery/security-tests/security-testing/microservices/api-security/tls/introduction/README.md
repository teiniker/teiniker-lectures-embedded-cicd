# Transport Layer Security 

Using TLS (HTTPS) for REST services is fundamental to security:

* **TLS protects data in transit (confidentiality)**: 
    REST APIs send sensitive data
    (Basic Auth, JWTs, API keys, personal data, business data)

* **TLS prevents credential theft and replay attacks**:
    REST authentication relies on headers. 
    Without TLS, attacker captures the header once and replays 
    it indefinitely

* **TLS guarantees server identity (authentication)**:
    TLS certificates prove the server’s identity and are signed 
    by a trusted Certificate Authority (CA)

* **TLS ensures message integrity**:
    Without TLS, attacker can modify requests or responses.

**In practice, we should use TLS for every REST interaction!**

## References

* Prabath Siriwardena. **Advanced API Security**. Apress 2019

* Prabath Siriwardena, Nuwan Dias. **Microservices Security in Action**. MANNING 2020

_Egon Teiniker, 2025, GPL v3.0_
