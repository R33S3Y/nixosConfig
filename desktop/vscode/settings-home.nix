{ pkgs, ... }:

{
  stylix.targets.vscode = {
    enable = true;
    fonts.enable = true;
    colors.enable = true;
    profileNames = [ "default" ];
  };
  programs.vscode = {
    enable = true;
    profiles.default = {
      enableUpdateCheck = false;
      extensions = with pkgs; [
        vscode-extensions.streetsidesoftware.code-spell-checker
      ];
      userSettings = {
        editor = {
          tabSize = 2; # sets tabSize
          insertSpaces = true; # Space when tab
          detectIndentation = false; # turns off auto detect for tabSize
        };
        workbench.startupEditor = "none";
        files = {
          trimTrailingWhitespace = true;
          insertFinalNewline = true;
          trimFinalNewlines = true;
        };
        editor = {
          formatOnSave = true;
          formatOnPaste = true;
        };
        cSpell = {
          language = "en-GB,en-NZ";
          autocorrect = true;
          hideAddToDictionaryCodeActions = true; # hides some ctl + shift + p options that wont work.
          maxDuplicateProblems = 1000;
          maxNumberOfProblems = 10000;
          suggestionMenuType = "quickFix";
          minWordLength = 1;
          revealIssuesAfterDelayMS = 200;
          checkLimit = 10000;
          userWords = [
            # their is a option to use make a custom dictionary that may prove useful if this list gets too long

          ];
          ignoreWords = [
            # this is just to shut it up a bit
            "stylix"
            "pkgs"
            "nixos"
            "nixpkgs"
            "pihole"
            "networkmanager"
            "str"
            "dmarc"
            "attrset"
            "reesey"
            "stdenv"
          ];
        };
        # disable AI slop
        chat = {
          disableAIFeatures = true;
          agent.enabled = false;
          commandCenter.enabled = false;
        };
        editor.inlineSuggest = {
          enabled = false;
          suppressSuggestions = true;
        };
        github.copilot.enable = {
          "*" = false;
        };
      };
    };
  };
}
