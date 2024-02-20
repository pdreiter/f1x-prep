#! /usr/bin/env python
#this file is to be run inside the f1x container in the directory /f1x
import os
import re
def main():
    #get the file in codeflaws that has a list of directories
    code_dir = "/f1x/codeflaws"
    buglist = open("{}/bug-list".format(code_dir), 'r').readlines()
    cmd_strs = []
    for b in buglist:
        d = "{}/{}".format(code_dir,b)
        cmd = gen_bash(d.strip())
        cmd_strs.append(cmd)
    of = open("f1x_run_cmds", 'w')
    for c in cmd_strs:
        of.write(c+'\n')
    of.close()
    #visit each directory
    #make a test.sh file that f1x can use
    #may need to copy the file orig-,trans- to the test file

def gen_bash(dir_name):
    #get program name from directory
    #might change this later
    name = dir_name[dir_name.rfind('/')+1:]
    name = name[:name.rfind('-')].replace('bug-', '')

    #get pos and neg test file input and output
    d = os.listdir(dir_name)
    inp = [x for x in d if x.startswith("input")]
    outp = [x for x in d if x.startswith("output")]

    #go through all inp and cull things that are not in test-genprog.sh
    of = open("{}/test-genprog.sh".format(dir_name), 'r')
    lns = of.readlines()
    tests = []
    for l in lns:
        m = re.match('p[0-9]+\)', l)
        if m:
            tests.append(l)
        m = re.match('n[0-9]+\)', l)
        if m:
            tests.append(l)
    ts = [t.replace('"$INPUT_NAME"', "input-pos") for t in tests]
    ts = [t.replace('"$OUTPUT_NAME"', "output-pos") for t in ts]
    ts = [t.replace('"$NEGINPUT_NAME"', "input-neg") for t in ts]
    ts = [t.replace('"$NEGOUTPUT_NAME"', "output-neg") for t in ts]
    t_names = []
    for t in ts:
        splt = t.split(' ')
        t_names.append((splt[2],splt[3]))

    rs = ""
    rs += "#!/bin/bash\ncase $1 in\n"
    t_n = 0
    for i,o in t_names:
        rs += "t{})\n".format(t_n)
        rs += "    eval ./{} < {} > tmp\n".format(name,i)
        rs += "    diff -w tmp {}\n;;\n".format(o)
        t_n += 1
    rs += "esac\n"
    of = open("{}/test.sh".format(dir_name), 'w')
    of.write(rs)
    of.close()
    st = os.stat("{}/test.sh".format(dir_name))
    os.chmod("{}/test.sh".format(dir_name), st.st_mode | 0o111)
    t_string = ""
    for i in range(len(t_names)):
        t_string += "t{} ".format(i)
    f1x_cmd = "f1x --files {}.c --driver test.sh --tests {}--test-timeout 1000".format(name,t_string)
    return f1x_cmd
if __name__ == "__main__":
    main()

