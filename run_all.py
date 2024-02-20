#! /usr/bin/env python
import os
import commands
def main():
    #open the file that has all the commands
    #append -a to run all
    code_dir = "/f1x/codeflaws"
    buglist = open("{}/bug-list".format(code_dir), 'r').readlines()
    cmds = open("f1x_run_cmds", 'r').readlines()
    i = 0
    f = 0
    p = 0
    for b in buglist:
        d = "{}/{}".format(code_dir,b)
        print(d)
        ec = run_f1x(d.strip(), cmds[i].strip())
        if ec == 0:
            f += 1
        if ec == 1:
            p += 1
        print("{} pass, {} fail".format(p,f))
        i += 1
    #open bug-list
    #cd into each so we can run the command from the file
    #process the output so we know what failed and passed

def run_f1x(d,cmd):
    #cd into the working directory
    os.chdir(d)
    splt = cmd.split(' ')
    #make sure we are using the original file
    cp_cmd = "cp orig-{} {}".format(splt[2],splt[2])
    s,o = commands.getstatusoutput(cp_cmd)
    #append -a so we get all patches
    cmd += ' -a'
    s,o = commands.getstatusoutput(cmd)
    if "patches successfully generated" not in o:
        return 0
    else:
        return 1

if __name__ == "__main__":
    main()

