#!/usr/bin/env bash

####################################################################################
# this script installs my dotfiles & configures the host to my preferences
# it borrows ~heavily~ from @atomantic and @jessfraz's dotfiles and install scripts:
# https://github.com/atomantic/dotfiles
# https://github.com/jessfraz/dotfiles
####################################################################################

# include helpers for colorized echo
source bin/echo.sh

# Close any open System Preferences panes, to prevent them from overriding
# settings we’re about to change
osascript -e 'tell application "System Preferences" to quit'

bot "This script is about to install new software on your host & configure your system settings the way I like them. Here we go!"

# get admin password before we do anything else
bot "Please enter your sudo password, so we can install a few tools:"
# Ask for the administrator password upfront
sudo -v

# Keep-alive: update existing sudo time stamp until the script has finished
while true; do sudo -n true; sleep 60; kill -0 "$$" || exit; done 2>/dev/null &

bot "Do you want me to setup this machine to allow you to run sudo without a password?\n"
read -r -p "Make sudo passwordless? [y|N] " response
if [[ $response =~ (yes|y|Y) ]];then
    sudo cp /etc/sudoers /etc/sudoers.back
    echo '%wheel		ALL=(ALL) NOPASSWD: ALL' | sudo tee -a /etc/sudoers > /dev/null
    sudo dscl . append /Groups/wheel GroupMembership $(whoami)
    bot "You can now run sudo commands without password!"
fi

# setup .gitconfig info
grep 'user = GITHUB_USER' $HOME/.gitconfig > /dev/null 2>&1
if [[ $? = 0 ]]; then
    read -r -p "What is your github.com username? " githubuser

    fullname=`osascript -e "long user name of (system info)"`

    if [[ -n "$fullname" ]];then
        lastname=$(echo $fullname | awk '{print $2}');
        firstname=$(echo $fullname | awk '{print $1}');
    fi

    if [[ -z $lastname ]]; then
        lastname=`dscl . -read /Users/$(whoami) | grep LastName | sed "s/LastName: //"`
    fi
  
    if [[ -z $firstname ]]; then
        firstname=`dscl . -read /Users/$(whoami) | grep FirstName | sed "s/FirstName: //"`
    fi
    email=`dscl . -read /Users/$(whoami)  | grep EMailAddress | sed "s/EMailAddress: //"`

    if [[ ! "$firstname" ]];then
    response='n'
    else
        echo -e "I see that your full name is $COL_YELLOW$firstname $lastname$COL_RESET"
        read -r -p "Is this correct? [Y|n] " response
    fi

    if [[ $response =~ ^(no|n|N) ]];then
    read -r -p "What is your first name? " firstname
    read -r -p "What is your last name? " lastname
    fi
    fullname="$firstname $lastname"

    bot "Okay, $fullname, "

    if [[ ! $email ]];then
    response='n'
    else
        echo -e "The best I can make out, your email address is $COL_YELLOW$email$COL_RESET"
        read -r -p "Is this correct? [Y|n] " response
    fi
    
    if [[ $response =~ ^(no|n|N) ]];then
    read -r -p "What is your email? " email
        if [[ ! $email ]];then
            error "you must provide an email to configure .gitconfig"
            exit 1
        fi
    fi

    running "replacing items in .gitconfig with your info ($COL_YELLOW$fullname, $email, $githubuser$COL_RESET)"

    # test if using gnu-sed or MacOS sed
    sed -i "s/GITHUB_FULL_NAME/$firstname $lastname/" $HOME/.gitconfig > /dev/null 2>&1 | true
    if [[ ${PIPESTATUS[0]} != 0 ]]; then
        echo
        running "looks like you are using MacOS sed rather than gnu-sed, accommodating"
        sed -i '' "s/GITHUB_FULL_NAME/$firstname $lastname/" $HOME/.gitconfig;
        sed -i '' 's/GITHUB_EMAIL/'$email'/' $HOME/.gitconfig;
        sed -i '' 's/GITHUB_USER/'$githubuser'/' $HOME/.gitconfig;
        ok
    else
        echo
        bot "looks like you are already using gnu-sed. nice!"
        sed -i 's/GITHUB_EMAIL/'$email'/' $HOME/.gitconfig;
        sed -i 's/GITHUB_USER/'$githubuser'/' $HOME/.gitconfig;
    fi
fi

# set wallpaper
MD5_NEWWP=$(md5 .images/default.jpg | awk '{print $4}')
MD5_OLDWP=$(md5 /System/Library/CoreServices/DefaultDesktop.jpg | awk '{print $4}')
if [[ "$MD5_NEWWP" != "$MD5_OLDWP" ]]; then
    read -r -p "Do you want to use the desktop wallpaper included with these dotfiles? [Y|n] " response
        if [[ $response =~ ^(no|n|N) ]];then
            echo "Skipping...";
            ok
        else
            running "Set wallpaper included in repo"
            # `DefaultDesktop.jpg` is already a symlink, and
            # all wallpapers are in `/Library/Desktop Pictures/`.
            rm -rf ~/Library/Application Support/Dock/desktoppicture.db
            sudo rm -f /System/Library/CoreServices/DefaultDesktop.jpg > /dev/null 2>&1
            sudo rm -f /Library/Desktop\ Pictures/El\ Capitan.jpg > /dev/null 2>&1
            sudo rm -f /Library/Desktop\ Pictures/Sierra.jpg > /dev/null 2>&1
            sudo rm -f /Library/Desktop\ Pictures/Sierra\ 2.jpg > /dev/null 2>&1
            sudo cp .images/default.jpg /System/Library/CoreServices/DefaultDesktop.jpg;
            sudo cp .images/default.jpg /Library/Desktop\ Pictures/Sierra.jpg;
            sudo cp .images/default.jpg /Library/Desktop\ Pictures/Sierra\ 2.jpg;
            sudo cp .images/default.jpg /Library/Desktop\ Pictures/El\ Capitan.jpg;ok
        fi
fi

# install homebrew (and homebrew-cask)
running "checking to see if homebrew is installed"
brew_bin=$(which brew) 2>&1 > /dev/null
if [[ $? != 0 ]]; then
    action "installing homebrew"
        # this will also install the xcode command-line tools for you
        /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
        if [[ $? != 0 ]]; then
            error "unable to install homebrew, script $0 abort!"
            exit 2
        fi
else
    ok
    # check for any updates to homebrew
    brew update
    ok
    bot "before installing brew packages, we can upgrade any outdated packages."
    read -r -p "run brew upgrade? [y|N] " response
    if [[ $response =~ ^(y|yes|Y) ]];then
        # Upgrade any already-installed formulae
        action "upgrade brew packages..."
        brew upgrade
        ok "brews updated..."
    else
            ok "skipped brew package upgrades.";
    fi
fi

running "install command-line tools"
brew install \
    git \
    bash \
    vim \
    neovim \
    gcc \
    curl \
    go \
    ag \
    tree \
    awk \
    coreutils \
    findutils \
    grep --with-default-names \
    gnu-sed --with-default-names \
    make --with-default-names

running "install other tools"
brew cask install \
    iterm2
    visual-studio-code
    1password
    google-chrome
    google-cloud-sdk
    iina
    spotify
    docker

running "check to see if everything installed correctly"
brew update
brew upgrade
brew prune
brew cleanup
brew doctor

bot "configuring iterm2-specific settings"
running "Don’t display the annoying prompt when quitting iTerm"
defaults write com.googlecode.iterm2 PromptOnQuit -bool false;ok
running "hide tab title bars"
defaults write com.googlecode.iterm2 HideTab -bool true;ok
running "hide pane titles in split panes"
defaults write com.googlecode.iterm2 ShowPaneTitles -bool false;ok
running "Make iTerm2 load new tabs in the same directory"
/usr/libexec/PlistBuddy -c "set \"New Bookmarks\":0:\"Custom Directory\" Recycle" ~/Library/Preferences/com.googlecode.iterm2.plist
running "setting font"
defaults write com.googlecode.iterm2 "Normal Font" -string "Monaco 12";
ok
running "reading iterm settings"
defaults read -app iTerm > /dev/null 2>&1;
ok

# vs-code specific settings
# create symlinks to vscode files
ln -s ~/.vscode/settings.json ~/Library/Application\ Support/Code/User/settings.json
ln -s ~/.vscode/snippets ~/Library/Application\ Support/Code/User/snippets
