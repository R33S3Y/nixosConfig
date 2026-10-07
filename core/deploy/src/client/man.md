---
title: DEPLOYCLIENT
section: 1
date: 2026-10-20
footer: version
header: R33S3Y/nixosConfig Manual
---

# NAME

deployClient - a cli interface used by the deploy command to rebuild on remote hosts. Users should really use that instead.

# SYNOPSIS

**deployClient** \-\-directory=dir \-\-signature=signature

**deployClient** -d dir -s signature

# DESCRIPTION

Deploy Client is a cli interface for remote rebuilding on other hosts used internally by deploy. It is best to go used the deploy command instead.

# OPTIONS

**-d dir**, **\-\-directory=dir**
: The directory containing tarball.tar

**-s signature**, **\-\-signature=signature**
: The signature to verify the tarball (Base 64 encoded)

**-l**, **\-\-lazy**
: Allows Deploy Client to be lazy and skip rebuild if there are no changes to the host.
