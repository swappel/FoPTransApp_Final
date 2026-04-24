# FoP-Translator

The application is not yet downloadable. It is still under heavy development. Thanks for your patience.

## About this application

This application allows you to contribute to the Avatar: Frontiers of Pandora Na'vi Localization project.
<br>It will manage the writing to game files and the synchronization of the progress with the others for you.

## Setup

#### IMPORTANT: You NEED a GitHub Account to be able to use this application

### Here are the steps for the setup of the application:

Start the application.

#### GitHub Setup

1. Enter your username. Please remember this in case you need to repeat the setup.
2. The application will generate an SSH Keypair. Please open your GitHub Account Settings by <br>
   Clicking on your profile picture(top right) > Settings > SSH and GPG keys > "New SSH key" button
3. Copy the public SSH key that has been printed in the console(Ctrl + Shift + C or select > right-click > Copy)
4. Enter a title on the GitHub page
5. Paste the public SSH key in the "Key" field on the GitHub page.
6. Confirm with "Add SSH key". You might have to go through 2FA to complete this action.

#### Game File Setup

1. Extract your game files(see below if not already done)
2. Copy the contents of the `blue/localization/english/package/` folder to the `game_files/` folder created where the
   application was started.
3. Verify you have the following files in the `game_files/` folder:
    * `menus.locpack` and `menus.locpacbin`
    * `subtitles.locpack` and `subtitles.locpackbin`
    * `subtitles_nocc.locpack` and `subtitles_nocc.locpackbin`
4. Enter the `reload` command in the application.
5. The output should now say "Successfully loaded all .locpack and .locpackbin files."
   <br>If this is not the case, please verify that the paths are correct and that ALL the files(.locpack and
   .locpackBIN) are in the `game_files/` directory.

## The editing process(work in progress)

The editing process is currently still work in progress, as I am trying to fix other bugs.

When I say "hash", I refer to the 16 character long string of hexadecimal values in the .locpack files.<br>
These hashes will be used as reference for a specific line when displaying and editing a specific line.
<br>Example of a hash: `35FC120A59CA144C00006EDDFBDC8E0E` is the hash referencing "Default" on the 17th line in
menus.locpack

### /!\ Important notes when editing files /!\

# PLEASE READ CAREFULLY!!!!!!!!

- Please make sure to copy these hashes perfectly when editing anything, as even slight changes could cause the line to
  get pasted to the wrong place.

- When you see things like `{0}` or lines with `"bla bla bla<some text> bla bla bla"` around and in them, KEEP THE
  QUOTES AND DO NOT EDIT ANYTHING BUT THE bla bla bla!

- For multi-line entries, please **DO NOT EDIT** them as of now, as the application is not yet built to handle these.
  This will break the whole file.

## Commands

Here is a list of commands, how to use them and what they do:

* `display <changes|repo|hash|all> [hash]` - Displays lines with the text and all other information.
    * `changes` - Displays all the lines you changed that were not yet synced.
    * `repo` - Displays all the lines anyone has published to the repository.
    * `hash [hash]` - Displays a line with a specific hash.
    * `all` - Displays ALL lines in the current file. **WARNING: These files can contain 40 000 lines and up**