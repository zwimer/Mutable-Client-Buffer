# znc-self-msg

Many irc clients that can connect to znc are not 'irc3.0 capable'. As such, they do not have the ability to recieve their own messages. This module fixes that issue, giving self-message capability to _all_ clients that connect to znc. For more info, please go to the [wiki](https://github.com/zwimer/znc-selfmsg/wiki). For help with anything, more information on any topic, or setup guides, please go to either our [Guides Information and Help](https://github.com/zwimer/znc-selfmsg/wiki/Guides,-Information,-and-Help) wiki page or our [FAQ](https://github.com/zwimer/znc-selfmsg/wiki/FAQ) wiki page.

## Disclaimer

This application is written for znc 1.6.4. Unfortunatly, the functions required for this to work for private channels do not yet exist. They will however be implemented in 1.7. Once 1.7 has been released and is stable, adding private message support should be trivial but unil then this will only work for public buffers. If you wish to know how it will be changed once 1.7 is released or wish to do it yourself, click [here](https://github.com/zwimer/znc-selfmsg/wiki/Changes-for-Private-Channels) to visit that page of our wiki.

## Requirements

This module requires znc 1.6.4. Documentation is provided, however if you wish to build the [documentation](#documentation) for this module for yourself, Doxygen 1.8.3 is required. For installation, it is _strongly recommended_ you have znc-buildmod unless you wish to compile and link this manually.

## Installation Instructions

First clone this repository
```bash
git clone https://github.com/zwimer/znc-selfmsg
```

Next cd into it and call znc-buildmod on selfmsg.cpp
```bash
cd znc-selfmsg 
znc-buildmod selfmsg.cpp
```

Finally, relocate the .so file znc-buildmod created to znc's modules folder
```bash
mv ./selfmsg.so ~/.znc/modules
```

If this fails because no modules folder exists in ~/.znc, simply create it first with
```bash
mkdir ~/.znc/modules
```

![](https://github.com/zwimer/znc-selfmsg/blob/master/ReadMeFiles/Installation.gif)

## Usage

To use this module either load it with /LoadMod or via webadmin. After that it should work.

## Documentation

Additional documentation to each component of self\_msg.cpp is provided and can also be found [here](https://zwimer.github.io/znc-selfmsg/). This documentation was generated via Doxygen. If you wish to generate this documentation yourself, from the znc-selfmsg directory, run the following
```bash
doxygen Doxyfile
```

The documentation 'index.html' is located within Documentation/html/
