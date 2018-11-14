## dotfiles

The technique for version-controlling these dotfiles relies on using a Git bare repository, which I found out about [here](https://developer.atlassian.com/blog/2016/02/best-way-to-store-dotfiles-git-bare-repo/)!

Also key to using this method is the ```config``` bash alias, which we'll use exclusively to run commands against our configuration repo (which will prevent us from messing around with other .git repos we might have).

**To install:**

First you should install Homebrew (which will also prompt you to install the XCode command-line tools):
```console
$ /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

It's a good idea to add ```/usr/local/bin``` to your ```$PATH``` to make sure your using programs installed by Homebrew (rather than their macOS defaults):

```console
$ echo 'export PATH="/usr/local/bin:$PATH"' >> ~/.bash_profile
```

Then run ```brew doctor``` to make sure everything works correctly:
```console
$ brew doctor
```

Once Homebrew is installed, install Git:
```console
$ brew install git
```

Now might be a good time to set up your Git user information:
```console
$ git config --global user.name "Your Name Here"
$ git config --global user.email "your_email@youremail.com"
```

Clone the Git repo:
```console
$ git clone --bare https://github.com/colemujadzic/dotfiles.git $HOME/.cfg
```

Define the ```config``` alias within the current shell scope:
```console
$ alias config='/usr/bin/git --git-dir=$HOME/.cfg/ --work-tree=$HOME'
```

Checkout the bare repo to ```$HOME```:
```console
$ config checkout
```
If this last step failed, you'll likely need to backup your existing configuration/dotfiles and remove them from ```$HOME``` and try again (Git should warn you that they'll be overwritten if such files are present)

Finally, set the showUntrackedFiles flag to 'no' for this new local clone:
```console
$ config config --local status.showUntrackedFiles no
```

**Adding & updating your configuration:**

Once you've set up the Git bare repo with the 'config' alias, changing your configuration is trivial using the ```alias``` command in lieu of ```git```

For example:
```console
$ config status
$ config add .bashrc
$ config commit -m "Add .bashrc"
$ config push
```

## resources
- [Nicola Paolucci's article](https://developer.atlassian.com/blog/2016/02/best-way-to-store-dotfiles-git-bare-repo/)
- [Jess Frazelle's dotfiles](https://github.com/jessfraz/dotfiles)
