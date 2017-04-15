# znc-self-msg

Many irc clients that can connect to znc are not 'irc3.0 capable'. As such, they do not have the ability to recieve their own messages. This module fixes that issue, giving self-message capability to _all_ clients that connect to znc. For more info, please go to the [wiki](https://github.com/zwimer/znc-selfmsg/wiki)

## Disclaimer

This application is written for znc 1.6.4. Unfortunatly, the functions required for this to work for private channels do not yet exist. They will however be implemented in 1.7. Once 1.7 has been released and is stable, adding private message support should be trivial but unil then this will only work for public buffers.

## Requirements

This application requires znc 1.6.4
For installation, it is strongly recommended you have znc-buildmod unless you wish to compile and link this manually.

## Installation Instructions

First clone this repository
Then git clone this repository
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

If this fails because no modules folder exists in ~/.znc, simply create it with
```bash
mkdir ~/.znc/modules
```

## Usage

To use this module either load it with /LoadMod or via webadmin. After that it should work.
