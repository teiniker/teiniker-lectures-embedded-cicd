# Authentication 

To enforce authentication for our REST services, we must decide which 
authentication protocol to use.

Most service infrastructures support the following mechanisms:
* Basic Authentication
* Digest Authentication
* Authentication using X.509 certificates.


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