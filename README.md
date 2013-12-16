Antares
=======

ANTARES is an all-in-one buildsystem, aimed at baremetal targets.
It is designed to be portable across a lot of MCUs and adding another 
MCU is trivial. 
It uses Kconfig as seen in linux kernel and a set of homebrew Makefiles
designed to ease the development process. 
This stuff is still pre-alpha quality, so if things don't work for you
out of the box - drop me a line, or create an issue on github

Abstract.
---------
Traditional buildsystems (cmake, scons, autotools etc.) are very
painful to use when it comes to bare metal targets. 
Proprietary crippled IDEs from vendors like IAR/CodeComposer/Keil, etc.
provide their own project management, but they are generally a pain 
to use especially if you are mostly used to VIM/emacs (like me).
While the above IDEs make it simpler to create some 'write&forget'
projects fast, they suck when it comes to maintaining a lot of shared
code between different projects, different revisions of projects. etc
(and that is what really lazy coders (like me) want).

The best way to manage a very configurable project with a lot of 
library code is Kconfig as seen in linux kernel. Mostly because it
handles dependencies of parameters and you won't eventually have a 
huge mess in your config.h (Lots of hatred go towards u-boot and its
crappy buildsystem). But you won't rip kconfig from kernel for each 
of your avr-led-blinker project, will you?
So, there we go. 

Yet another Arduino?
--------------------
Let's be honest.
It is not Arduino. It doesn't aim to provide any crippled IDE for your
projects. You are free to use whatever IDE/editor you like. VIM/emacs 
users welcome. Eclipse? You're welcome. Kdevelop? Sure.  
It doesn't end up with lies, trying make you think that you are writing 
in any 'innovative' programming language, nor aims to provide an IDE. 
It's mostly just plain C. Antares just handles configuring your project, 
building and deploying and stays out of your way. Consider it an 
overgrown Makefile on steroids. Although antares comes with quite some 
library code, you are free NOT to use any of it and exclude it from 
compilation. 
The choice is yours to make, after all.

Why not just stick to Arduino?
------------------------------
Personal reasons. It's unportable, ugly, very little hardware is actually 
supported. Most tasty chips are NOT supported at all.

So, in short antares is...
--------------------------
A set of Makefiles, scripts and a bunch of library code, that can be 
used (and reused) easily in your projects, bundled with kconfig. 

Supported architectures
------------------------
Currently a few architectures are supported: 
avr, msp430, arm(stm32), 8051 (at89, stc), pic32
Others are being added as the hardware comes along. 
If you want more - patches are always welcome.

Supported host operating systems
--------------------------------
Currently only Linux and FreeBSD have been tested.
Mac OS X has been recently proved to work fine after a few patches
Windows *might* work (cygwin), but it's currently NOT the platform 
antares is aimed to be used with. If it turns out to be rather simple
to add support - patches welcome.

Is antares buildsystem okay to compile in multiple threads?
-----------------------------------------------------------
Feel free to supply the -jXX flag to speed things up. It should work. 
If it doesn't - submit a bug report, and it will work sooner or later.

Any documentation?
------------------
The documentation is maintained in doc/, avaliable in
English and Russian.  

Any 'official' development boards?
----------------------------------
Antares is NOT a hardware project, nor it aims to sell crippled
overpriced and underpowered boards to newbies. Feel free to take any 
board you have around, starting with the 'closest' example and playing
with menuconfig. If MCU family is supported - getting stuff to work is
fast and fun. 

Examples?
---------
See antares-examples repository. These contain the ready to use 
templates for your projects. You can use them via aproj wizard to 
quickly bootstrap a new project. 

Opensource?
-----------
Hell, yes. It wouldn't be any fun the other way. Although the licence 
is GPLv2, some library code from third parties may come with a 
different license. Figuring out the licensing stuff is also WiP.  

3rd party code and their licensing.
----------------------------------
ST Peripheral Libraries. Proprietary (BSD-like?) license
VUSB (Dual, GPLv2+obdev exception and proprietary)
lwIP (BSD-like) 

Why is there so little library code for arch XXX?
------------------------------------------------
Patches welcome. 

Quickstart?
-----------
Have a look at docs/en/quickstart

Who?
----
Andrew 'Necromant' Andrianov - Initial author and core dev of Antares
Nikita 'webconn' Maslov - Testing, first version of aproj wizard, bugfixing.
Dmitry Suhotsky - Mostly testing in live environment 
Drakmail - Mac OS X testing
