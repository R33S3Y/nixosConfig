#!/bin/bash

set -euo pipefail

CONFIG_SRC="/home/reese/Projects/nixosConfig"
CONFIG_DST="/tmp/config_current"
GIT_REPO="$CONFIG_SRC"

# List of remote hosts (can be just "host" or "user@host")
REMOTE_HOSTS=(
    "rebuild@obsidian"
    "rebuild@morganite"
    "rebuild@bort"
    "rebuild@ghostQuartz"
)

GOOD="\033[94m"
OK="\033[35m"
BAD="\n\033[31m"
RESET="\033[0m"

# Ensure the script is run with sudo
if [[ $EUID -ne 0 ]]; then
    echo -e "${BAD}Please run as root (sudo $0)${RESET}"
    exit 1
fi

# Get the current NixOS generation locally
CURRENT_GEN=$(nixos-rebuild list-generations | awk 'NR==2 {print $1}')
NEXT_GEN=$((CURRENT_GEN + 1))
COMMIT_MSG="$(date '+%Y-%m-%d') - Attempting Build for gen: $NEXT_GEN"

# Commit changes before rebuilding
sudo -u "$(logname)" bash <<EOF
    cd "$GIT_REPO"
    git add .
    git commit -m "$COMMIT_MSG" || echo -e "${BAD}No changes to commit${RESET}"
EOF

# Copy the configuration files locally
rm -rf $CONFIG_DST
mkdir -p $CONFIG_DST
cp -r "$CONFIG_SRC"/* "$CONFIG_DST"/

# Rebuild NixOS locally (diamond)
if ! nixos-rebuild switch --flake "$CONFIG_DST/#diamond"; then
    echo -e "${BAD}Local NixOS rebuild failed on diamond. Aborting.${RESET}"
    exit 1
fi

sudo nix-env --profile /nix/var/nix/profiles/system --delete-generations +25
sudo nix-collect-garbage

echo -e "\n${OK}Local NixOS rebuild succeeded on diamond.${RESET}"
echo -e "${OK}Pushing to remote hosts...${RESET}\n"

eval "$(ssh-agent -s)"
ssh-add /home/reese/.ssh/id_ed25519

# Loop through each remote host
for HOST in "${REMOTE_HOSTS[@]}"; do
    echo -e "\n${OK}Preparing $HOST...${RESET}"
    ssh "$HOST" "rm -rf $CONFIG_DST && mkdir -p $CONFIG_DST"

    # Sync configuration files
    echo -e "${OK}Copying files to $HOST...${RESET}"
    scp -q -r "$CONFIG_SRC"/* "$HOST:$CONFIG_DST/"

    # Extract just the hostname (after @ if present)
    echo -e "${OK}Rebuilding $HOST...${RESET} \n"
    HOSTNAME=$(echo "$HOST" | cut -d@ -f2)
    # Rebuild NixOS remotely
    if ! ssh "$HOST" "sudo nixos-rebuild switch --flake $CONFIG_DST/#$HOSTNAME"; then
        echo -e "${BAD}Remote NixOS rebuild failed on $HOST. Aborting.${RESET}"
        exit 1
    fi
    echo -e ""
    echo -e ""

    # Removing older generations
    echo -e "${OK}Removing older generations $HOST...${RESET}"
    echo -e ""
    ssh "$HOST" sudo nix-env --profile /nix/var/nix/profiles/system --delete-generations +25
    ssh "$HOST" sudo nix-collect-garbage

done

echo -e "\n${GOOD}All rebuilds succeeded. Pushing changes to GitHub...\n${RESET}"

# Push changes to GitHub
sudo -u "$(logname)" bash <<EOF
    cd "$GIT_REPO"
    git push
EOF
