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

**deploy** [deploy options] \-\-flake=flakeRef host...

**deploy** [deploy options] -f flakeRef host...

# DESCRIPTION

Deploy is a commandline utility for remotely rebuilding managing Nixos Rebuilds. Allowing you to manage and test dozen's of hosts at once.

# OPTIONS

**-a**, **\-\-all**
: Rebuilds all hosts, Even if not necessary.

**-d**, **\-\-dynamic**
: Skip hosts if none of the files touched are affected

**-f flakeRef**, **\-\-flake=flakeRef**
: The flake to deploy.

# EXAMPLE

Deploying to 3 hosts from a github repo:
: deploy -a -f github:your/repo host1 host2 host3
