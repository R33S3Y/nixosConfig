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

**deploy** [deploy options] \-\-flake=flakeRef hosts...

**deploy** [-a -c -d] -f flakeRef hosts...

# DESCRIPTION

Deploy is a commandline utility for remotely rebuilding managing Nixos Rebuilds. Allowing you to manage and test dozen's of hosts at once.

# OPTIONS

**-a**, **\-\-all**
: Rebuilds all hosts, Even if not necessary.

**-c**, **\-\-commit**
: Commits the code before deploying. Only works when the flakeRef is a standard filepath.

**-d**, **\-\-dynamic**
: Skip hosts if none of the files touched in the latest commit are used in on that host.

**-f flakeRef**, **\-\-flake=flakeRef** (Required)
: The flake to deploy.

# HOSTS

Hosts can be listed at the end of the command. Eg: deploy options... host1 host2 host3
Alternatively a wildcard (\*) can be listed to deploy to all hosts available in the flake.

# EXAMPLE

Deploying to 3 hosts from a github repo:
: deploy -a -f github:your/repo host1 host2 host3
