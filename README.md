# sago_env
Sago's personal environment

This is my personal scripts. They are provided here for inspiration.

##Installation

Clone it so that this README is in:
````
$HOME/git/sago_env/README.md
````

Add this to .bashrc:
````
if [ -d "$HOME/git/sago_env/bin" ] ; then
    PATH="$HOME/git/sago_env/bin:$PATH"
fi
````

##Noteworthy commands:

````
sago_create_program [PROGRAMNAME]
````
Creates a basic C++ project that compiles by typing "make" and that can be opened in NetBeans 8.1. 
I recommend everyone having a script like that to quickly start a project. This is of course more important for compiled languages.
Once you have a script like this you will start creating programs over scripts even for smaller solutions that you earlier considered too small to bother with.
There is a *sago_create_java_program* too that will create a maven project from a basic template

````
sago_git_get_branchname
````
Some commands are just harder to remember. This just shows the name of the current branch in git.

````
avconv_copy_to_mp4 [VIDEOFILE]
````
Converts a video file to a mp4 container without doing any reincoding. This is mainly used to create PlayStation 3 compatible files or other places only allowing mp4. Can be run on multiple files with xarg:
````
ls *.mkv | xargs -n 1 avconc_copy_to_mp4
````

````
sago_create_jekyll_post [TITLE] [DATE]
#Example:
sago_create_jekyll_post "My latest blog post" 2016-03-31
````
Another example of creating a script to do the boilderplate things. 


##Test programs

Sometimes I just keep some demos to remind myself of how different libraries work. Feel free to copy from these examples for any purpose.
Try compiling smfltest5 to see SMFL working:

````
~/git/sago_env/programs/smfltest5
make
./smfltest5
````


