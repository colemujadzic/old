#!/bin/bash

# Autocorrect typos in path names when using `cd`
shopt -s cdspell

# Enable kubectl bash completion
if hash kubectl 2>/dev/null; then
  source <(kubectl completion bash)
fi


# Enable google cloud sdk autocompletion
source '/usr/local/Caskroom/google-cloud-sdk/latest/google-cloud-sdk/path.bash.inc'
source '/usr/local/Caskroom/google-cloud-sdk/latest/google-cloud-sdk/completion.bash.inc'

for file in ~/.{bash_prompt,aliases,path}; do
	if [[ -r "$file" ]] && [[ -f "$file" ]]; then
		# shellcheck source=/dev/null
		source "$file"
	fi
done
unset file


