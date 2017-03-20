# znc-self-msg

Allow self-message capability for all clients that connect to znc

## Disclaimer
### Under Constriction

## Requirements

This application requires znc 1.6.4
For installation, it is strongly recommended you have znc-buildmod

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

To use this module either load it with /LoadMod or via the webadmin. After that it should work.
