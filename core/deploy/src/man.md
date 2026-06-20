---
title: DEPLOY
section: 1
date: 2026-06-20
version: 1.0.0
header: R33S3Y/nixosConfig Manual
---

# NAME

deploy - A utility cmd for nixos rebuilds

# SYNOPSIS

**deploy** [deploy options] --flake=flakeRef host...
**deploy** [deploy options] -f flakeRef

# DESCRIPTION

Remotely Rebuilds the system as specified.

# OPTIONS

**-a**, **\-\-all**
: Rebuilds all hosts, Even if not necessary.

**-d**, **\-\-dynamic**
: Skip hosts if none of the files touched are affected

**-f flakeRef**, **\-\-flake=flakeRef**
: The flake to deploy.

# EXAMPLE

Deploying to 3 hosts from a github repo:

```
deploy -a -f github:you/repo host1 host2 host3

```
