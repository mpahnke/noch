# noch

(**n**etwork **o**nline **ch**eck)

At this time, this project is for educational purposes only. Therefore there is neither a warranty nor functionality or completeness intended and will be also not available. This includes partly also this documentation.

The goal of this project wants to show, how to start with Qt programming. Especially with qt-creator. 

To have a halfuseful goal it shows this with more or less small utilities, which checks if a network resource such as a website has been changed. If that's the case, then the user should be notified. 
___
Derzeit dient dieses Projekt ausschließlich Bildungszwecken. Daher ist weder eine Garantie noch eine Funktionalität oder Vollständigkeit vorgesehen und wird auch nicht zur Verfügung stehen.

Ziel dieses Projektes sind mehr oder weniger kleine Hilfsprogramme, welche prüfen, ob eine Netzwerkressource, wie zum Beispiel eine Website geändert wurde und in diesem Fall den Benutzer zu benachrichtigen.

## the build prerequisites

To build a binary from the sources, you need the Qt developer libraries and build tools as well as at least one  general make tool.For example cmake. The pair qmake and gnu make is also supported.

Important: Currently this project uses the old Qt4 libraries and tools yet.

### on debian/ubuntu

If you install the neccessary packages on debian/ubuntu, with the following commandline, you should be able to compile and link a binary.

    sudo apt-get install libqt4-dev qt4-dev-tools libqt4-dev-bin
