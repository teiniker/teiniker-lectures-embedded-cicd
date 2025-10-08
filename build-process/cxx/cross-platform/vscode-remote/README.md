# VS Code Remote Development 

With the **Remote - SSH** extension from Microsoft, VS Code can access the remote 
server via SSH, edit, compile, start and debug files there.

* Establish an SSH remote connection
    - click on the SSH button at the bottom left `[><]`.
    - Connect to Host...
    - Add new SSH Host
    - ssh student@xxx.xxx.xxx.xxx

![VS Code Remote](figures/VSCode-Remote.png)

The Remote - SSH extension lets us use any **remote machine with a SSH server 
as our development environment**. This can greatly simplify development and 
troubleshooting in a wide variety of situations. 

* Develop on the same operating system we deploy to or use larger, faster, or
    more specialized hardware than your local machine.
* Quickly swap between different, remote development environments and safely 
    make updates without worrying about impacting our local machine.
* Access an existing development environment from multiple machines or locations.
* Debug an application running somewhere else such as a customer site or in the cloud.

No source code needs to be on your local machine to gain these benefits since 
the extension **runs commands and other extensions directly on the remote machine**. 

We can **open any folder on the remote machine** and work with it just as you would 
if the folder were on your own machine.


## References

* [Remote - SSH](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-ssh) 

* [YouTube (Visual Studio Code): Getting Started with Remote Development](https://youtu.be/QA9jlp-o5vQ?si=wQbXyXuzBiXIDxxL)
* [VS Code Remote Development](https://code.visualstudio.com/docs/remote/remote-overview)

*Egon Teiniker, 2025, GPL v3.0*