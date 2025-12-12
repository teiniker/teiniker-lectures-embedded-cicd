# Authentication 

In REST services, we can use several **authentication methods** depending 
on your security needs, client types, and deployment environment.

* **HTTP Basic Authentication**: The client sends a `username:password` pair 
    encoded in Base64 in the Authorization header.
    - Simple to implement
    - Supported by almost all HTTP clients
    - Must be used **only over HTTPS**
    
    _Example:_ HTTP header for basic authentication 

    ```HTTP
    Authorization: Basic dXNlcjpwYXNz
    ```

* **Token-Based Authentication**: Client authenticates once and receives a 
    token, which is sent with each request.
    - Simple and scalable
    - Token management is custom

* **JSON Web Tokens (JWT)**: Client logs in, server issues a signed JWT,  
    client sends it with every request.   
    - Supports expiration & claims
    - Widely used in REST APIs
    - Tokens must be protected on the client

   _Example:_ HTTP header for JWT authentication 

    ```HTTP
    Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6...
    ```

* **API Key Authentication**: Client sends a unique API key in headers 
    or query params.
    - Easy to use
    - Good for internal or partner APIs
    - No user identity
    - Harder to rotate securely

   _Example:_ HTTP header for API key authentication 

    ```HTTP
    X-API-Key: abc123xyz
    ```

* **OAuth 2.0**: Delegated authorization framework (used by Google, 
    GitHub, etc.).
    - Industry standard
    - Secure and flexible
    - Ideal for third-party integrations
    - Overkill for simple APIs

* **Mutual TLS (mTLS)**: Both client and server authenticate using TLS certificates.
    - Very strong security
    - No tokens or passwords
    - Complex setup
    - Mostly for internal microservices

The choice of authentication mechanism depends on the specific use case. 

Here are some recommendations:

* **Learning / testing**: Basic Auth
* **Small / internal API**: JWT or API Key
* **Microservices**: mTLS or JWT
* **Public API**: OAuth 2.0 or JWT


## Basic Authentication 

Basic Authentication is the simplest protocol available for performing 
authentication over HTTP. 
It involves sending a **Base64–encoded username and password** within 
a request header to the server. 

Protocol:
* An unauthorized client tries to access a secure service.
* The server would reply with an HTTP response of: 

    ```
    HTTP/1.1 401 Unauthorized 
    WWW-Authenticate: Basic realm="ApplicationRealm"
    ```

    The **401** response tells the client that it is **not authorized** 
    to access the URI it tried to invoke on. 

    The WWW-Authenticate header specifies which authentication protocol 
    the client should use. 
    **Basic** means Basic Authentication should be used. 

    The **realm** attribute identifies a collection of secured resources 
    on a website. 

* To perform authentication, the client must send a request with the 
    Authorization header set to a Base 64–encoded string of our 
    username and a colon character, followed by the password:
    
    ```
    Authorization: Basic c3R1ZGVudDpzdHVkZW50
    ```
    
    The **client needs to send this Authorization header with each request** 
    it makes to the server.

    Don’t forget to use an **encrypted connection** like HTTPS!


## Common Design Flaws in Authentication 

* **Vulnerable Transmission of Credentials**: If an application uses an 
    unencrypted HTTP connection to transmit login credentials, an eavesdropper 
    can intercept them. 

* **Bad Passwords**: Many web applications employ no or minimal controls over 
    the quality of users’ passwords, they are:
    - Very short or blank
    - Common dictionary words or names
    - The same as the username
    - Still set to a default value

    It is likely that an application that does not enforce strong password 
    standards will contain a large number of user accounts with weak 
    passwords set. 

* **Brute-Forcible Login**: If an application allows an attacker to make 
    repeated login attempts with different passwords until he guesses the 
    correct one, it is highly vulnerable.
    
    Any serious attacker will use automated techniques to guess passwords, 
    based on lists of common values.

* **Insecure Storage of Credentials**: User credentials are stored in clear text.
    If passwords are being hashed using a standard algorithm such as SHA-256, 
    this still allows an attacker to simply look up passwords against a 
    pre-computed database of hash values. 

## References

* Prabath Siriwardena. **Advanced API Security**. Apress 2019

* Prabath Siriwardena, Nuwan Dias. **Microservices Security in Action**. MANNING 2020

_Egon Teiniker, 2025, GPL v3.0_