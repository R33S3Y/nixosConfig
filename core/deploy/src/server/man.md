---
title: DEPLOY
section: 1
date: 2026-06-20
footer: version
header: R33S3Y/nixosConfig Manual
---

# NAME

deploy - A utility cmd for remote nixos rebuilds

# SYNOPSIS

**deploy** [deploy options] \-\-k=privateKey \-\-flake=flakeRef hosts...

**deploy** [-c -l] -k privateKey -f flakeRef hosts...

# DESCRIPTION

Deploy is a commandline utility for remotely rebuilding managing Nixos Rebuilds. Allowing you to manage and test dozen's of hosts at once.

# OPTIONS

**-c**, **\-\-commit**
: Commits the code before deploying. Only works when the flakeRef is a standard filepath. TODO!

**-f flakeRef**, **\-\-flake=flakeRef** (Required)
: The flake to deploy.

**-k privateKey**, **\-\-key=privateKey** (Required)
: The key to sign the config with and connect to hosts via ssh

**-l**, **\-\-lazy**
: Skip hosts if nothing has changed since the last rebuild. Is enabled by default.

**-s**, **\-\-strict**
: Rebuilds all hosts, Even if not necessary. **\-\-strict** is the opposite to the **\-\-lazy** and they are mutually exclusive.

# HOSTS

Hosts can be listed at the end of the command. Eg: deploy options... host1 host2 host3
Alternatively a "all" can be listed to deploy to all hosts available in the flake.

# EXAMPLE

Strictly _(A.K.A: Rebuild even if not needed)_ Deploying to 3 hosts from a github repo:
: deploy -s -f github:your/repo -k ~/.ssh/id_ed25519 host1 host2 host3

Deploying to all hosts in a flake if needing rebuild
: deploy -lfk github:your/repo ~/.ssh/id_ed25519 all
