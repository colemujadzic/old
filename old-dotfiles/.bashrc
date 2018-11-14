# Creates an alias 'config' to use instead of the 'git' command when interacting with the .cfg (configuration) repo
alias config='/usr/bin/git --git-dir=/Users/cole/.cfg/ --work-tree=/Users/cole'

# Creates an alias 'brewup' to make it easier to keep Homebrew up and running:
alias brewup='brew update; brew upgrade; brew prune; brew cleanup; brew doctor'

# Creates an alias to use neovim instead of vim
#if type nvim > /dev/null 2>&1; then
#  alias vim='nvim'
#fi
