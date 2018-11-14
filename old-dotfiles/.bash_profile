# Load the shell dotfiles, and then some:
# * ~/.path can be used to extend `$PATH`.
# * ~/.extra can be used for other settings you don’t want to commit.
# e.g (aliases,functions,path,dockerfunc,extra,exports)
for file in ~/.{bash_prompt,aliases,functions,path,dockerfunc,extra,exports}; do
	if [[ -r "$file" ]] && [[ -f "$file" ]]; then
		# shellcheck source=/dev/null
		source "$file"
	fi
done
unset file

# Ensure user-installed binaries take precedence
export PATH=/usr/local/bin:$PATH

# Load .bashrc if it exists
test -f ~/.bashrc && source ~/.bashrc

# Creates an alias 'config' to use instead of the 'git' command when interacting with the .cfg (configuration) repo
alias config='/usr/bin/git --git-dir=/Users/cole/.cfg/ --work-tree=/Users/cole'

# Creates an alias 'brewup' to make it easier to keep Homebrew up and running:
alias brewup='brew update; brew upgrade; brew prune; brew cleanup; brew doctor'

# Creates an alias 'la' to get the current directory listing with long form file names and hidden files
alias la='ls -lhA'

# Enables bash-completion
if [ -f $(brew --prefix)/etc/bash_completion ]; then
    source $(brew --prefix)/etc/bash_completion
fi

# Creates an alias to use neovim in place of vim
#if type nvim > /dev/null 2>&1; then
#  alias vim='nvim'
#fi
