 The lmake Manual       <!-- a.summary-letter {text-decoration: none} blockquote.indentedblock {margin-right: 0em} div.display {margin-left: 3.2em} div.example {margin-left: 3.2em} div.lisp {margin-left: 3.2em} kbd {font-style: oblique} pre.display {font-family: inherit} pre.format {font-family: inherit} pre.menu-comment {font-family: serif} pre.menu-preformatted {font-family: serif} span.nolinebreak {white-space: nowrap} span.roman {font-family: initial; font-weight: normal} span.sansserif {font-family: sans-serif; font-weight: normal} ul.no-bullet {list-style: none} -->

The lmake Manual
================

Short Table of Contents
-----------------------

*   [1 Overview of `lmake`](#toc-Overview-of-lmake)
*   [2 An Introduction to Lmakefile.py syntax](#toc-An-Introduction-to-Lmakefile_002epy-syntax)
*   [3 User Commands](#toc-User-Commands)
*   [4 Job Commands](#toc-Job-Commands)
*   [5 The Python `lmake` module](#toc-The-Python-lmake-module)
*   [6 Directories](#toc-Directories)
*   [7 Config](#toc-Config)
*   [8 Sources](#toc-Sources-1)
*   [9 Rules](#toc-Rules-1)
*   [10 The LMAKE directory](#toc-The-LMAKE-directory)
*   [11 Some considerations](#toc-Some-considerations)
*   [12 FAQ](#toc-FAQ)
*   [13 Glossary](#toc-Glossary)

Table of Contents
-----------------

*   [1 Overview of `lmake`](#Overview-of-lmake)
    *   [1.1 Preparing and running `lmake`.](#Preparing-and-running-lmake_002e)
    *   [1.2 Problems and Bugs](#Problems-and-Bugs)
*   [2 An Introduction to Lmakefile.py syntax](#An-Introduction-to-Lmakefile_002epy-syntax)
    *   [2.1 Configuration](#Configuration)
    *   [2.2 Sources](#Sources)
    *   [2.3 Rules](#Rules)
        *   [2.3.1 `name`](#name)
        *   [2.3.2 `stems`](#stems)
        *   [2.3.3 `targets`](#targets)
        *   [2.3.4 `deps`](#deps)
        *   [2.3.5 `cmd`](#cmd)
    *   [2.4 A simple Lmakefile.py](#A-simple-Lmakefile_002epy)
*   [3 User Commands](#User-Commands)
    *   [3.1 `lmake`](#lmake-1)
        *   [3.1.1 output](#output)
    *   [3.2 `lshow`](#lshow)
    *   [3.3 `lfreeze`](#lfreeze)
    *   [3.4 `lforget`](#lforget)
    *   [3.5 `autodep`](#autodep-1)
    *   [3.6 `xxhsum`](#xxhsum)
*   [4 Job Commands](#Job-Commands)
    *   [4.1 `ldepend`](#ldepend)
    *   [4.2 `lcheck_deps`](#lcheck_005fdeps)
    *   [4.3 `lcritical_barrier`](#lcritical_005fbarrier)
    *   [4.4 `ltarget`](#ltarget)
*   [5 The Python `lmake` module](#The-Python-lmake-module)
    *   [5.1 `class pdict`](#class-pdict)
    *   [5.2 `def version(major,minor)`](#def-version_0028major_002cminor_0029)
    *   [5.3 Helpers for Lmakefile.py](#Helpers-for-Lmakefile_002epy)
        *   [5.3.1 `class Rule`](#class-Rule)
        *   [5.3.2 `class AntiRule`](#class-AntiRule)
        *   [5.3.3 `class PyRule`](#class-PyRule)
        *   [5.3.4 `class DynamicPyRule`](#class-DynamicPyRule)
        *   [5.3.5 `class RustRule`](#class-RustRule)
        *   [5.3.6 `class GitRule`](#class-GitRule)
        *   [5.3.7 `class HomelessRule`](#class-HomelessRule)
        *   [5.3.8 `class DirtyRule`](#class-DirtyRule)
        *   [5.3.9 `def search_root_dir(cwd=os.getcwd())`](#def-search_005froot_005fdir_0028cwd_003dos_002egetcwd_0028_0029_0029)
    *   [5.4 constants](#constants)
    *   [5.5 Job execution support](#Job-execution-support)
*   [6 Directories](#Directories)
    *   [6.1 `lmake.local_admin_dir`](#lmake_002elocal_005fadmin_005fdir)
    *   [6.2 `lmake.remote_admin_dir`](#lmake_002eremote_005fadmin_005fdir)
*   [7 Config](#Config)
    *   [7.1 backends](#backends-1)
    *   [7.2 caches](#caches)
*   [8 Sources](#Sources-1)
    *   [8.1 `manifest_sources(manifest='Manifest',**kwds)` :](#manifest_005fsources_0028manifest_003d_0027Manifest_0027_002c_002a_002akwds_0029-_003a)
    *   [8.2 `git_sources(recurse=True,ignore_missing_submodules=False,**kwds)` :](#git_005fsources_0028recurse_003dTrue_002cignore_005fmissing_005fsubmodules_003dFalse_002c_002a_002akwds_0029-_003a)
    *   [8.3 `auto_sources(**kwds)` :](#auto_005fsources_0028_002a_002akwds_0029-_003a)
*   [9 Rules](#Rules-1)
    *   [9.1 `combine`](#combine)
    *   [9.2 `name`](#name-1)
    *   [9.3 `virtual`](#virtual)
    *   [9.4 `prio`](#prio)
    *   [9.5 `stems`](#stems-1)
    *   [9.6 `job_name`](#job_005fname)
    *   [9.7 `targets`](#targets-1)
    *   [9.8 `target`](#target)
    *   [9.9 `post_targets`](#post_005ftargets)
    *   [9.10 `post_target`](#post_005ftarget)
    *   [9.11 `deps`](#deps-1)
    *   [9.12 `dep`](#dep)
    *   [9.13 `chroot`](#chroot)
    *   [9.14 `cwd`](#cwd)
    *   [9.15 `env`](#env)
    *   [9.16 `python`](#python)
    *   [9.17 `shell`](#shell)
    *   [9.18 `cmd`](#cmd-1)
        *   [9.18.1 if it is a `function`](#if-it-is-a-function)
        *   [9.18.2 if it is a `str`](#if-it-is-a-str)
    *   [9.19 `cache`](#cache)
    *   [9.20 `backend`](#backend)
    *   [9.21 `autodep`](#autodep-2)
    *   [9.22 `resources`](#resources)
    *   [9.23 `stderr_len`](#stderr_005flen)
    *   [9.24 `allow_stderr`](#allow_005fstderr)
    *   [9.25 `auto_mkdir`](#auto_005fmkdir)
    *   [9.26 `ignore_stat`](#ignore_005fstat)
    *   [9.27 `keep_tmp`](#keep_005ftmp)
    *   [9.28 `force`](#force)
    *   [9.29 `timeout`](#timeout)
    *   [9.30 `job_tokens`](#job_005ftokens)
    *   [9.31 `n_tokens`](#n_005ftokens)
    *   [9.32 `start_delay`](#start_005fdelay)
    *   [9.33 `kill_sigs`](#kill_005fsigs)
*   [10 The LMAKE directory](#The-LMAKE-directory)
    *   [10.1 LMAKE/config](#LMAKE_002fconfig)
    *   [10.2 LMAKE/rules](#LMAKE_002frules)
    *   [10.3 LMAKE/sources](#LMAKE_002fsources)
    *   [10.4 LMAKE/makefiles](#LMAKE_002fmakefiles)
    *   [10.5 LMAKE/outputs/<date>](#LMAKE_002foutputs_002f_003cdate_003e)
    *   [10.6 LMAKE/last\_output](#LMAKE_002flast_005foutput)
    *   [10.7 LMAKE/targets](#LMAKE_002ftargets)
*   [11 Some considerations](#Some-considerations)
    *   [11.1 Rule inheritance](#Rule-inheritance)
    *   [11.2 Backends](#Backends)
    *   [11.3 Local backend](#Local-backend)
    *   [11.4 Autodep](#Autodep)
        *   [11.4.1 `'None'` or `'none'`](#g_t_0027None_0027-or-_0027none_0027)
        *   [11.4.2 `'LdPreload'` or `'ld_preload'`](#g_t_0027LdPreload_0027-or-_0027ld_005fpreload_0027)
        *   [11.4.3 `'LdAudit'` or `'ld_audit'`](#g_t_0027LdAudit_0027-or-_0027ld_005faudit_0027)
        *   [11.4.4 `'Ptrace'` or `'ptrace'`](#g_t_0027Ptrace_0027-or-_0027ptrace_0027)
    *   [11.5 Link support](#Link-support)
    *   [11.6 Rule selection](#Rule-selection)
        *   [11.6.1 Up-hill directory](#Up_002dhill-directory)
        *   [11.6.2 Match a target](#Match-a-target)
        *   [11.6.3 Check static dependencies](#Check-static-dependencies)
        *   [11.6.4 Group recap](#Group-recap)
    *   [11.7 Hierarchical repositories](#Hierarchical-repositories)
    *   [11.8 ETA estimation](#ETA-estimation)
    *   [11.9 Video mode and colors](#Video-mode-and-colors)
*   [12 FAQ](#FAQ)
    *   [12.1 How do I prevent `lmake` to set dependencies to a given directory](#How-do-I-prevent-lmake-to-set-dependencies-to-a-given-directory)
    *   [12.2 `gcc` does not generate dependencies on some generated .h files, how to handle this ?](#gcc-does-not-generate-dependencies-on-some-generated-_002eh-files_002c-how-to-handle-this-_003f)
*   [13 Glossary](#Glossary)

`lmake`
=======

This file documents the `lmake` utility, which determines automatically which pieces of a large workflow need to be remade and issues the commands to remake them.

This is Edition _work in progress_, last updated 22 April 2023, of The lmake Manual, for `lmake` version 0.1.

Copyright © 2023 Doliam.

> Permission is granted to copy, distribute and/or modify this document under the terms of the GNU Free Documentation License, Version 3 or any later version published by the Free Software Foundation

1 Overview of `lmake`
---------------------

The `lmake` utility automatically determines automatically which pieces of a large workflow need to be remade and issues the commands to remake them. This manual describes `lmake`, which was implemented by Cesar Douady.

Our examples show C/C++ and Python programs, since they are most common, but you can use `lmake` with any programming language to run any phase of your CI/CD as long as these can be scripted. Indeed, `lmake` is not limited to programs. You can use it to describe any task where some files must be (re)computed automatically from others whenever recomputing would generate such files differently than what they currently are. Such situations include dependency modifications, but also command modifications, dependency list modifications, apparition of an include file that was in the search path before a include file was actually accessed, symbolic link modifications, etc.

`lmake` is designed to be scalable, robust and efficient.

By scalable, we mean that `lmake` can manage millions of files and tens of thousands of CPU hours without any difficulty, so there is never any reason to have any kind of recursive invocation, `lmake` can handle the whole project all at once.

By robust, we mean that `lmake` guarantees that if a job is not rerun, then rerunning it would lead to the same content (or a content that is equally legal). This includes automatic capture of so called hidden dependencies (i.e. dependencies that are not explicitely stated in the rules, e.g. include files).

We also mean that `lmake`, as any software, it may have bug. Such bugs can lead to crashes and to pessimistic behavior (a job is rerun while it was not necessary). But special attention has been devoted in its design to ensure that it is never optimistic (a job not being rerun while it should have been). In case of any adverse event (lmake crashes or spurious system reboot), `lmake` automatically recovers potentially corrupted states in a safe way to avoid having to remake the whole project because a few files are corrupted.

Note that `lmake` does not only recorver from its own flees, but also a lot of experience is embedded into it to work around system bugs. This includes for example violations by NFS of its close-to-open synchronization guarantee (which happens, under heavy system load, maybe once in 10.000 accesses) or SGE/SLURM spuriously disappearing.

By efficient, we mean that jobs are run in parallel, optionally using a batcher such as slurm, managing limited resources as declared in Lmakefile.py. We also mean that `lmake` makes a lot of effort to determine if it is necessary to run a job (while always staying pessismistic). Such effort includes checksum based modification detection rather than date based, so that if a job is rerun and produces an identical content, subsequent jobs are not rerun. Also, `lmake` embed a build cache whereby jobs can record their results in a cache so that if the same run needs to be carried out by another user, it may barely fetch the result from the cache rather than run the - potentially lengthy - job.

### 1.1 Preparing and running `lmake`.

To prepare to use `lmake`, you must write a file called the Lmakefile.py that describes the relationships among files in your workflow and provides commands for generating each file. In a program, typically, the executable file is updated from object files, which are in turn made by compiling source files. Then unit tests are run from the executable and input files, and the output is compared to some references.

Once a suitable Lmakefile.py exists, each time you change anything in the workflow (source files, recipes, ...) this simple shell command:

lmake <my\_target>

suffices to perform all necessary steps so that <my\_target> is reproduced as if all steps leading it were carried out while actually carrying out only necessary steps. The `lmake` program maintains an internal state in the LMAKE directory to decide which of the files need to be regenerated. For each of those files, it issues the recipes recorded in Lmakefile.py. During the execution of recipes, `lmake` instrument them in order to gather which files were read and written in order to determine hidden dependencies and whether such actions were legal. These informations are recorded in the LMAKE directory.

You can provide command line arguments to `lmake` to somewhat control this process.

### 1.2 Problems and Bugs

If you have problems with `lmake` or think you’ve found a bug, please report it to the developers; we cannot promise to do anything but we might well want to fix it.

Before reporting a bug, make sure you’ve actually found a real bug. Carefully reread the documentation and see if it really says you can do what you’re trying to do. If it’s not clear whether you should be able to do something or not, report that too; it’s a bug in the documentation!

Before reporting a bug or trying to fix it yourself, try to isolate it to the smallest possible Lmakefile.py that reproduces the problem. Then send us the makefile and the exact results `lmake` gave you, including any error messages. Please don’t paraphrase these messages: it’s best to cut and paste them into your report. When generating this small makefile, be sure to not use any non-free or unusual tools in your recipes: you can almost always emulate what such a tool would do with simple shell commands. Finally, be sure to explain what you expected to occur; this will help us decide whether the problem was really in the documentation.

if your problem is non-deterministic, i.e. it shows up once in a while, include the entire content of the LMAKE directory. This directory contains extensive execution traces meant to help developers to track down problems. Make sure, though, to trim it from any sensitive data.

Once you have a precise problem you can report it in one of two ways. Either send electronic mail to:

    xxx@yyy.zzz

or use our Web-based project management tool, at:

    https://xxx.yyy.zzz

In addition to the information above, please be careful to include the version number of `lmake` you are using. You can get this information from the file LMAKE/version. Be sure also to include the type of machine and operating system you are using. One way to obtain this information is by running the command ‘uname -a’.

2 An Introduction to Lmakefile.py syntax
----------------------------------------

You need a file called a Lmakefile.py to tell `lmake` what to do.

In this chapter, we will discuss a simple Lmakefile.py that describes how to compile, link and test a tiny executable hello\_world which consists of 3 C++ source files (hello\_world.cc, hello.cc and world.cc) and 2 header files (hello.hh and world.hh). This tiny executable just writes `hello world` on its output. There is a reference output (hello\_world.ref) that `lmake` uses to check if the execution is correct.

So, the entire project contains the following files :

*   hello.hh
*   hello.cc (includes hello.hh)
*   world.hh
*   world.cc (includes world.hh)
*   hello\_world.cc (includes hello.hh and world.hh)
*   hello\_world.ref

When `lmake` recompiles this tiny executable, each changed C++ source file must be recompiled. If a header file has changed, each C++ source file that includes the header file must be recompiled. Each compilation produces an object file corresponding to the source file. Then, if any source file has changed, all the object files, whether newly made or saved from previous compilations, must be linked together to produce the new tiny executable. Finally, i the tiny executable changed, its unit test must be rerun and if the resulting output changes (or if the reference is modified), the actual output must be recompared against the reference.

As its name suggests, Lmakefile.py is actually a Python file. `lmake` embeds a Python interpreter (version 3.6+) to interpret Lmakefile.py. Because Python is among the most powerful and widely used programming langage, this choice allows `lmake` users to have access to a very powerful langage to express complicated workflows without the need to learn a new langage. As a side effect, it also allows the developpers to provide such a powerful means without having to design and implement it. Among the features of Python that come handy when describing workflows, one can list variables, functions, f-strings, conditionals, loops and inheritance.

The goal of Lmakefile.py is to describe 3 aspects of your workflow :

*   some global configuration
*   the list of the sources of the workflow
*   the derivation rules

This is done by importing the `lmake` module and defining the 3 variables `lmake.config`, `lmake.sources` and `lmake.rules`. When reading Lmakefile.py, `lmake` simply imports this module by issuing `import Lmakefile` in a Python 3.6+ interpreter and looks at these 3 variables. `lmake` provides some help for these 3 goals, though.

Once Lmakefile.py has been read, `lmake` writes in the LMAKE directory a digest of these 3 variables in the files LMAKE/config, LMAKE/sources and LMAKE/rules. These may be consulted in case of doubt about what `lmake` has understood of your Lmakefile.py file.

### 2.1 Configuration

The configuration describes some global aspects of the workflow. The `lmake` module contains a reasonable default value for the `lmake.config` variable and in an introduction chapter, there is nothing much to say. In a simple workflow, ignoring the configuration step is a good starting point.

### 2.2 Sources

`lmake` needs to know the list of the source files. The mere presence on disk of a file is not sufficient for `lmake` to qualify it as a source. This is because the aim of `lmake` is to provide a reproductible workflow. For example, if you work under `git`, as it is very common, once you have been able to build a target, you must be able to commit and push then a colleague (or yourself in another repository) must be able to do a pull and build the same target to get the same result. If a file exists in your original repository, is not tracked by `git` and is used to build your target, your colleague will not be able to reproduce the same target with the same content. In such a situation, `lmake` will consider your file as _dangling_ and accessing it will be an error.

Source files are listed in `lmake.sources` as a `list` or a `tuple`. By default, if `lmake.sources` is not set, `lmake` will look for a file named Manifest and use it as a list of files, one per line. If no Manifest file is present, it will then issue a `git ls-files --recurse-submodules` command to gather the list of sources.

### 2.3 Rules

Rules are classes that listed in the `lmake.rules` variables. Rather than manually adding them to this variables, rules can simply be defined as classes inheriting from the class `lmake.Rule`. In addition to listing derived classes in the `lmake.rules` variables, the `lmake.Rule` base class provide defaults for various attributes that are of secondary importance for simple rules.

To be recognized as a usable rule, rules must have the following attributes :

*   `targets`
*   `cmd`

Generally, it will also have these attributes :

*   `stems`
*   `deps`

class Compile(lmake.Rule) :
	stems   = { 'File' : r'.\*'         }
	targets = { 'OBJ'  : '{File}.o'  }
	deps    = { 'SRC'  : '{File}.cc' }
	cmd     = 'gcc -c -o $OBJ $SRC'

Let’s look at each element in this rule.

#### 2.3.1 `name`

The first element is the class name `Compile`. This name will be used by `lmake` in reporting when it needs to refer to this rule, in error messages or when it reports execution. Generally speaking, all rules have a `name` attribute which defaults to the class name if not explicitly set. Here, the `name` of our rule is `'Compile'` but we could have set the attribute `name` to `'cc compilation'` for example.

#### 2.3.2 `stems`

Then comes the `stems` attribute. This attribute provides a vocabulary of (Python) regular expressions that can be further referenced in other attributes.

#### 2.3.3 `targets`

Then, there is the `targets` attribute. This attribute defines the files that may be generated by this rule. Its syntax looks like Python 3.6+ f-strings, except that variable parts (in `{}`) must be plain variables (not actual expressions). However, this works the other way around, compared to f-strings. With an f-string, you start from a value of `File`, say `'foo'`, and from that, you can derive the string `'foo.o'` from the f-string `'{File}.o'`. But here, we start from a file foo.o and we find that this rule applies if we define `File` as `'foo'`. More precisely, `lmake` matches the file name `'foo.o'` against the regular expression `r'(?P<File>.*)\.o'` and Python provides the group `'File'` as `'foo'`.

The reason why the Python 3.6+ f-strings syntax has been leverage is by symmetry with the `deps` attribute described below.

There may be more than a single target (and this is why this attribute is a `dict`). In that case, the rule is supposed to generate all of them when it is executed and any of them will trigger its execution.

#### 2.3.4 `deps`

Then, we find the `deps` attribute. This attribute define the explicit dependencies the rule need before it can be run. This is a `dict` mapping keys to actual Python 3.6+ f-strings. Once `File` has been determined by matching a target, it is fed to the f-strings to determine the explicit dependencies. `lmake` will automatically determine actual dependencies by spying the rule execution, so one could think mentioning them explicitly is useless. To some extent, this is true, with these limitations :

*   Explicit dependencies are not only used to determine if a rule must be run or not, they are also used to determine if a rule apply or not. For a rule to apply, it must match a given file as a target, but also, the computed explicit dependencies must be sources or recursively buildable. By contrast, if a hidden dependencies cannot be built (and if it does not actually exist), this is silently ignored (more to come later on this subject).
    
    Imagine for example that you have a rule to compile C++ files and another rule to compute C files. Given foo.o, `lmake` must determine which rule to apply. With explicit dependencies, `lmake` has the necessary information to select the adequate rule : it will look for files foo.cc and foo.c and if one is a source or is buildable, it can then select the corresponding rule. Note that if both are sources or buildable, the situation will be ambiguous and `lmake` will report an error. On the opposite, if none are sources or buildable, file.o will be deemed not buildable.
    
*   From a performance perspective, this allows `lmake` to know up front that a dependency is required before running a job. If a dependency is not explicitly mentioned, a target may be built by `lmake`, spying its accesses. `lmake` then discovers the dependency, and if it is not up to date, build it and rerun the rule with up to date dependencies. This double run (which occurs only the first time `lmake` hit the given target) can be avoided if the dependency is explicit.

Generally speaking, obvious dependencies (such as the source file for a compilation step) should be mentioned explicitly and the other ones (such as include files for a compilation step) can be left as hidden dependencies.

#### 2.3.5 `cmd`

Finally we see the `cmd` attribute. This attribute specifies the recipe to use to build targets from dependencies. The `cmd` attribute can be either a `str` or a function.

If it is a `str`, it contains a shell script that will be executed when the rule is triggered (the shell can be customized and defaults to the default system shell, usually /bin/sh). Before execution, its environment will be augmented with the definitions of stems, targets and dependencies (and a few others that will be described later). This way, scripts are easy to read and write.

It can also be a function and our example rule could have been written as :

import subprocess as sp
class Compile(lmake.Rule) :
	stems   = { 'File' : r'.\*'       }
	targets = { 'OBJ'  : '{File}.o'  }
	deps    = { 'SRC'  : '{File}.cc' }
	def cmd() :
		sp.run( ('gcc','-c','-o',OBJ,SRC) , check=True )

In that case, this function is executed in a Python interpreter when the rule is triggered (the Python used to run this function can be customized and defaults to the Python used to read Lmakefile.py). Before execution, the definitions of stems, targets and dependencies are put in the global `dict` (and a few others that will be described later). This way, functions are easy to read and write.

### 2.4 A simple Lmakefile.py

Here is a simple Lmakefile.py that sustains our hello\_world example :

import lmake

# use default config

# automatic sources

class Base(lmake.Rule) :
	stems = { 'File' : r'.\*' }

class Compile(Base) :
	targets = { 'OBJ' : '{File}.o'  }
	deps    = { 'SRC' : '{File}.cc' }
	cmd     = 'gcc -c -o $OBJ $SRC'

class LinkHelloWorld(lmake.Rule) :
	targets = { 'EXE' : 'hello\_world' }
	deps    = {
		'HELLO\_WORLD' : 'hello\_world.o'
	,	'HELLO'       : 'hello.o'
	,	'WORLD'       : 'world.o'
	}
	cmd = 'gcc -o $EXE $HELLO\_WORLD $HELLO $WORLD'

class Run(Base) :
	target = '{File}.out'
	deps   = { 'EXE' : '{File}' }
	cmd    = '$EXE'

class Check(Base) :
	target = '{File}.ok'
	deps = {
		'OUT' : '{File}.out'
	,	'REF' : '{File}.ref'
	}
	def cmd() :
		assert open(OUT).read()==open(REF).read()

Note a certain number of elements we have not introduced yet :

*   We use a base class to define the stem `File`, so we do not need to repeat its definition in each rule.
*   In rule `Run`, we have defined the attribute `target` as a `str` rather than attribute `targets` as a `dict`.  
    This this handy in simple situations and has the following meaning :
    *   There is a single target
    *   Its content is generated from the stdout of `cmd` execution
*   If `cmd` is a `str`, the status of the job is determined from the status of the script. If `cmd` is a function, raising an exception or returning convertible to `True` will trigger an error status for the rule execution

To run our example, we need to execute `lmake hello_world.ok` and this will do all necessary steps that ensure proper generation of the `hello_world` executable, including compilation, link, execution and check.

Note that we do not have to ever mention hidden dependencies (hello.h and world.h), these are handled automatically.

A few reactions of `lmake` to user actions when you execute `lmake hello_world.ok` :

*   If you modify hello.cc, `lmake` will recompile to hello.o, then relink to hello\_world, then re-generate to hello\_world.out and finally re-check to hello\_world.ok if hello\_world.out changed or stop there if it did not.
*   If you modify hello.hh, `lmake` will recompile to hello.o and to hello\_world.o in parallel (if resources permit), then proceed with the rest of the flow as above.
*   If you merely modify a comment in hello.cc, `lmake` will recompile to hello.o (which will be identical to its previous content) and stop there.
*   If you modify the compilation script (e.g. to add the `'-O'` option), all compilations are rerun and most probably the rest as well.
*   If you `rm` hello.o, nothing happens.
*   If you `rm` hello.o and modify world.cc, `lmake` will recompile to world.o, then understands that it needs to hello.o for the link phase and will recompile to it, then proceed with the rest of the workflow.
*   If you modify hello.cc and hello.o, `lmake` will refuse to regenerate hello.o over your modifications as it fears to destroy an information which is important for you. For example, you may have saved an important work in this wrong file and you may not have a copy elsewhere.

3 User Commands
---------------

All commands may be launched from anywhere in a repository. A repository is a directory containing Lmakefile.py. This means that before being executed, all commands walk up the directory tree until it finds a file named Lmakefile.py, which is then considered to be the repository.

Argument and reports are systematically localized to the current working directory. For example, if you launch `lmake b` from directory a in your repository, `lmake` will execute jobs from the repository root, and during this execution, it will refer to file a/b as b and file c as ../c.

If launched from a terminal, output is colored. Colors are different depending on whether terminal if dark/light or light/dark. These colors can be configured. The colors bears a semantic :

*   Green means success.
*   Red means error.
*   Magenta means warning.
*   Blue means notes generated by the tool.
*   Gray means information of secondary importance.
*   Uncolored means output from user code. This output can be colored by user code (e.g. gcc generates colored error messages, which is very helpful).

### 3.1 `lmake`

Usage : `lmake [-a|--archive] [-e|--forget-old-errors] [-m|--manual-ok] [-o|--live-out] targets...`

`lmake` launches all necessary jobs to ensure that targets are generated following rules specified in Lmakefile.py.

`lmake` generates an output line for each significant event :

*   When a job starts if it is long (duration that qualifies a job as long is configurable). This start line is deemed of secondary importance.
*   When a job terminates. Such lines are followed by the content of stderr if any and status is known.
*   When the job status is finally known if it was not known when it terminated. such lines are followed by the content of stderr if any.

At the end of the execution, if the asked targets are not successfully generated, a summary is generated reminding the errors (with a configurable max, 20 by default) and the stderr of the first error. Intermediate targets are deemed of secondary importance.

When run `lmake` launches a server and direct its request to it. This server can then serve other `lmake` requests, so that several `lmake` process can run simultaneously in the same repository.

During a `lmake` process, all jobs are launched according to the state of the repository at the time the `lmake` command was launched. This way, edition in the repository can go on while the `lmake` process runs. If it turns out that a file is needed that has been modified since `lmake` was launched, it will be deemed in error for that process. This way, if you want a reliable run, then you must not modify the repository while `lmake` is running. But if you are running `lmake` in an interactive mode, it is very comfortable to be able to go on with your edition session while some regression testing is running. At worst, you will have an error mentionning a file has been overwritten, but you will not have an incoherent result based on some old files and some newer files.

Options :

*   `-a`|`--archive` : ensure all intermediate files are up to date, in addition to the asked targets. This is useful for example if you want to archive a fully built repository.
*   `-e`|`--forget-old-errors` : assume encountered errors are transicent. Contrarily to the `lforget -e` command, this only concerns this execution, not subsequent ones.
*   `-m`|`--manual-ok` : normally, `lmake` refuses to launch a job that may overwrite a modification done by the user as `lmake` fears to jettison important informations from the user. With this option, the user instructs `lmake` that this fear is not pertinent.
*   `-o`|`--live-out` : normally, `lmake` does not output the stdout of jobs (such stdout is accessible with the `lshow` command). However, sometimes it is practical to have the output while jobs are running. Generating such output for all jobs would produce an intermixed flow of characters of all jobs running in parallel making such an output unreadable. When this options is used, only the jobs directly producing the asked targets have their output generated on the output of `lmake`. Because most of the time there is a single target, this ensures that there is a single job generating its output, avoiding the intermixing problem.
*   `-s`|`--source-ok` : normally, `lmake` refuses to launch a job that may overwrite a source. With this option, the user instructs `lmake` that this is allowed.
*   `-t`|`--keep-tmp` : normally, `lmake` washes the temporary directory allocated to a job at the end of job execution. With this option, the user instructs `lmake` to keep the temporary directories, as if the `keep_tmp` attribute was set for all rules. The kept temporary directory can be retreived with `lshow -i`.

#### 3.1.1 output

While `lmake` runs, it outputs a log. This log is also recorded in LMAKE/outputs/<start date> with the following differences :

*   \- It is not colored.
*   \- Files are relative to the root of the repository, not to the current working directory where the `lmake` command has been launched.

The log contains a line, possibly followed by attached information when the following events occur :

*   \- A job is started, if the job duration is longer than the `start_delay` attribute of the rule.
*   \- A job is completed.
*   \- A job status is known, while it was not known when it completed.
*   \- A source file has been seen as modified.
*   \- A frozen file or a target of a frozen job is needed.

Once the build process is complete, a summary is generated with :

*   \- The frozen files and jobs that we necessary to carry out the build.
*   \- The jobs in error (the first of them is accompanied with its stderr).

### 3.2 `lshow`

Usage : `lshow [ [-a|--all-deps] | [-d|--deps] | [-D|-inv-deps] | [-i|--info] | [-s|--script] | [-S|--exec-script] | [-E|--env] | [-e|--stderr] |[-o|--stdout] [-t|--targets] ] targets...`

`lshow` provides various informations about jobs. The jobs are those officially generating the targets (i.e. the jobs that would be selected by `lmake` if needing to generate it) if any, else it may be the jobs that actually generated the target.

Options :

*   `-a`|`--all-deps` : output the list of all the deps of the jobs. The first character of each line indicates if dep is a static one (’S’) or a hidden one (’ ’). And the second character is a rudimentary ascii art to show parallel deps (the ones that are generated in a single call). Criticality is shown with indentation. If a dep does not exist, it is deemed secondary information.
*   `-d`|`--deps` : same as `-a` except that only existing deps are shown.
*   `-D`|`--inv-deps` : show jobs that depend on targets.
*   `-i`|`--info` : show various self-reading info about jobs, such as reason to be launched, execution time, host that executed it, ...
*   `-s`|`--script` : show the scripts used to execute the jobs.
*   `-S`|`--exec-script` : show the scripts used to execute the jobs in a form that can be directly executed by standard `sh`.
*   `-E`|`--env` : show the environment used to run the script
*   `-e`|`--stderr` : show the stderr of the jobs.
*   `-o`|`--stdout` : show the stdout of the jobs.
*   `-t`|`--targets` : show the non-dirty targets of the jobs.

### 3.3 `lfreeze`

Usage : `lfreeze [ [-a|--add] | [-d|--delete] ] [-j|--job] [-f|--file] targets...` or `lfreeze [-l|--list] | [-D|--delete-all] [-j|--job] [-f|--file]`

`lfreeze` is used to manipulate frozen jobs and files. Frozen files are considered by `lmake` as sources although they are not listed as such by Lmakefile.py. Frozen jobs are not run and considered up to date. Each time such a file or job is referenced by `lmake`, an output line is generated mentioning it and such usages appear in the summary.

These precautions are taken because frozen files presence goes against repeatability.

Frozen files and jobs are useful to run a flow from A to B. To do that you type `lfreeze A` followed by `lmake B`.

Options :

*   `-a`|`--add` : add targets to the list of frozen files.
*   `-d`|`--delete` : remove targets from the list of frozen files.
*   `-l`|`--list` : list frozen files.
*   `-D`|`--delete-all` : delete all frozen files.
*   `-j`|`--job` : restrict action to jobs
*   `-f`|`--file` : restrict action to files

### 3.4 `lforget`

Usage : `lforget targets...` or `lforget [-e|-r]`

`lforget` is used to ask lmake to forget about some of its history. In its first form, subsequent `lmake` commands will forget about the build history of the mentioned targets. As a consequence, these targets will appear out of date with not hidden dependencies. This is exceptionally useful in situations where `lmake`’s hypotheses are broken :

*   \- A file outside the repository is modified. Because `lmake` does not track modifications outside the repository, it canot rerun jobs that must be rerun, this must be done manually.
*   \- An error is transcient. Another option is to launch `lmake -e` which asks to consider jobs in error as out-of-date.

In its second form, subsequent `lmake` commands will :

*   \- `-e`|`--error` : consider errors seen so far as transcient errors and will rerun corresponding jobs.
*   \- `-r`|`--resources` : not trust builds with old resources an rerun targets that have been successfully built with old resources.
*   \- -

The `-e` option is useful when you have actully seen transcient errors. Just rerunning jobs in error will wash these. If you need finer control, then you must use the first form of `lforget` to control what must be forgotten on a job by job basis.

The `-r` option is useful in the following scenario :

*   \- You have run jobs J1 and J2. J1 completed successfully but J2 lacked some memory and ended in error.
*   \- Then you have modified the allocated memory, increasing J2’s memory and decreasing J1’s memory because you think it is better balanced this way.
*   \- Then you remade both jobs. J2 reran because it was in error and now completes successfully. J1 did not rerun because it was ok and modifying some resources would not change the result.
*   \- However, it could be that now J2 does not have enough memory any more. It is not a problem in itself because its content is correct, but it may not be reproducible.
*   \- You want to make sure your repository is fully reproducible.
*   \- In that case, you run `lforget -r`. J1 will rerun because it was not run with the new resources, as if its command was modified. J2 will not because it has already run since then.

### 3.5 `autodep`

Usage : `autodep [[-o|--out] file] [[-m|--autodep-method] method] [[-s|--link-support] level] [[-d|--auto-mkdir]] [[-i|--ignore-stat]] executable args...`

`autodep` is a small tool that may be useful when designing a flow. It allows to see what informations (dependencies and targets) are recorded by code instrumentation `lmake` injects in jobs to track activity without actually run a job under its control.

The trace generated by `autodep` is halfly digested in the sens that :

*   \- Only a summary of pertinent accesses are provided, unlike `strace` can generate a line for each access. There is a line for each accessed file, which may be a read or nothing, possibly followed by a write or an unlink.
*   \- Accesses outside the repository are not tracked.
*   \- When symbolic links are supported, they are resolved according to the required support level and additional dependencies may be generated when needed for `lmake` to work properly.
*   \- It is not fully digested as target flags that are provided in rules are not taken into account. A full digestion to mimic `lmake` would require a whole lot of information that would make the user interface significantly heavier, defeating the usefulness of this tool.

Accesses are split into 2 categories :

*   \- Targets : a prefix mention the type of target, unlink versus write, preceded by a read or not.
*   \- Deps : a kind of ascii art render parallel accesses and lines are indented for each critical barrier crossing.

Options :

*   `-o`|`--out` file : generate output to file, by default output is generated to `stderr`.
*   `-m`|`--autodep-method` `method` : use `method` to instrument job. Can be one of :
    
    *   \- `none` : command is not instrumented (except that the environment variable `LMAKE_AUTODEP_ENV` is set). The only way to report activity is through the use of `ldepend` and co.
    *   \- `ld_preload` : command is run with a library loaded using the `LD_PRELOAD` environment variable that catches some calls libc.so (such as `open`) to track activity. This requires libc.so to be dynamically linked.
    *   \- `ld_audit` : this is similar to `ld_preload` except that `LD_AUDIT` mechanism is used instead of `LD_PRELOAD`, which is less invasive (cases where it is not transparent are very awkward). This also requires libc.so to be dynamically linked.
    *   \- `ptrace` : command is run `ptrace`’ed. The seccomp mechanism is used to reduce the performance hit to the minimum possible but still, it is often unacceptable. There is no requirement that libc.so be dynamically linked.
    
    Not all methods are supported on all systems.
    
*   `-s`|`--link-support` `level` : level of support for symbolic links (see [link-support](#link_002dsupport)).
*   `-d`|`--auto-mkdir` : Directories are automatically created behind the scene upon calling the `chdir` system call. This may be necessary if your command does an equivalent of `cd a ; cat b` to read a/b where the existence of a is not guaranteed by some other means. In that case, without this option, if directory a does not exist, the `cd` is not done and `lmake` generates a dependency to b instead of a/b.
*   `-i`|`--ignore-stat` : Stat-like accesses (i.e. access that just access the inode) are ignored. This may be a performance boost as it is not uncommon that such accesses represent the vast majority of tracked system calls, at the expense of reliability.

### 3.6 `xxhsum`

Usage : `xxhsum file`

XXH is a very high performance, high quality checksum generation algorithm internally used by `lmake` to find out whether file was actually modified or not when its date is changed. XXH is high quality but **not** crypto-robust. This means that you can defeat it if you write a code specially aimed at this purpose, but not otherwise, by chance. The version used in `lmake` is 56/64 bits : Checksums are computed on 64 bits but when comparing 2 checksums, if they match on 56 (lsb) bits but not on the full 64 bits, `lmake` will consider we enter into a _danger zone_ and will stop. Theoretical computation gives that you can generate thousands of files per second for thousands of year before entering the _danger zone_. However this theoretical computation assumes that XXH is nearly perfect, which seems to be the case with regards to experiments, but of course it is impossible to do such experiments with a particular data set that may be far from random and from the actual data set used to evaluate XXH. In the very improbable case where your data set would be particularly unfriendly with XXH, `lmake` will crash with an error rather than silently forgetting to rerun a job.

`xxhsum` allow you to generate the checksum of a file.

Because `lmake` handles symbolic links as themselves and not as the file they point to (i.e. `lmake` works in the physical world), `xxhsum` does not follow symbolic links. The checksum generated for a symbolic link is computed after the link content and such checksums do not clash with regular files with the same content (a different salt is used).

Also, the execute permission bit is also used to compute the checksum of regular files. `lmake` does not handle security bits (read and write permissions), but the execute bit is a semantic bit (possibly in addition to security) and thus is managed by `lmake`.

4 Job Commands
--------------

The commands listed hereafter are meant to be executed from within a job, not as standalone commands.

### 4.1 `ldepend`

Usage : `ldepend [--no-error|-e] [--no-required|-r] [--no-essential|-s] [--verbose|-v] files ...`

`ldepend` may be used to inform `lmake` that some files must be deemed as read as if you called the `open` system call on each of them. To this extent, it is not that much different from `cat` (except performance wise, depend is of course much faste since `files` are not really accessed).

Also, generated dependencies are parallel, i.e. a modification on a dependency will not mask an error on another one. For example, if you do `cat a b`, `lmake` sees 2 `open` system calls, to a then to b, exactly the same sequence that if you did `cat $(cat a)` where a contains `b` as its only content.

Suppose now that b is an error. This is a reason for your job to be in error. But if you modify a, in the former case, this **cannot** solve your error while in the later case, it may, if the new content of a points to file that may be successfully built. Because `lmake` cannot distinguish between the 2 cases, upon a modification of a, the job will be rerun in the hope that the error is solved. Parallel dependencies wil prevent this to happen.

If you do `ldepend a b` and a is modified and b is in error, the job is not rerun and stays in error.

Also, if for some reasons you run with autodep method `none`, this is the way to report dependencies.

The following options are supported :

*   `-e`|`--no-error` : ignore the error status of the passed dependencies.
*   `-r`|`--no-required` : if passed dependencies are not buildable, this does not prevent this job from being run (as it is for dependencies automatically generated after accesses).
*   `-s`|`--no-esential` : passed dependencies will not appear in the flow shown with a graphical tool.
*   `-v`|`--verbose` : write lines composed of the crc and the name separated by a space for each required dependency.

### 4.2 `lcheck_deps`

Usage : `lcheck_deps`

`lcheck_deps` ensures that all dependencies accessed so far are up-to-date. If this not the case, the job is killed, generating a `rerun` report, the dependencies are rebuilt and the job is rerun.

This is useful before starting a heavy computation. Typically, dependencies are computed and accessed before the heavy sequence and calling `check_deps` allows to avoid running a heavy computation with bad inputs. It is not a semantic problem as `lmake` will realize the situation, rebuild the dependencies are rerun the job, but it may be performance problem.

Imagine for exemple that you have to compile heavy.c that includes a file generated.h generated by the command generator. Imagine then that you type `lmake generated.h`, you look at it, find that the file is syntactically correct but contains a bug. You then modify generator and because you are confident in your modification, you type `lmake heavy.o`.

`lmake` will run the compilation of heavy.o, which lasts 10 minutes and discover that you need generated.h, which is out-of-date. It will then rebuild generated.h and rerun the compilation to heavy.o, another 10 minutes.

Suppose now that your compilation script separate the preprocessor (say 10 secondes) phase from the compilation (10 minutes) phase and call `lcheck_deps` inbetween. In that case, the first run will stop after preprocessing as `lcheck_deps` will kill the job at that moment and the overall time will be 10 minutes 10 secondes instead of 20 minutes.

Note the following points :

*   \- This is a (performance) problem only during the first run of heavy.o. On subsequent runs, in particular during a typical edit-compile-debug loop, `lmake` will know the dependencies and will launch jobs in the proper order. But during the first run, it has no knowledge that heavy.c actually includes generated.h.
*   \- Most often, when generated.h is out-of-date, it is syntactically incorrect (for example it does not exist), so the first run fails quite early (without spending its heavy optimization time).
*   \- In the case where generated.h is rebuilt identically to its previous content, there will be no second run without `lcheck_deps` call, so `lcheck_deps` has a (minor) adverse effect leading to an overall time of 10 minutes 10 seconds instead of 10 minutes.

Despite these remarks, there are case where `lcheck_deps` is very welcome.

### 4.3 `lcritical_barrier`

Usage : `lcritical_barrier`

`lcritical_barrier` inserts a critical barrier in the list of dependencies.

During dependency analysis, `lmake` consider them by groups (called criticality levels) separated by critical barriers and considers the next group only when the previous group is up-to-date.

Typical use of criticality levels is when you have a report that is built from the results of tests provided by a list of tests (a test suite).

For example, let’s say you have :

*   \- 2 tests whose reports are built in test1.rpt and test2.rpt by some rather heavy means
*   \- a test suite test\_suite.lst listing these reports
*   \- a rule that builds test\_suite.rpts by collating reports listed in test\_suite.lst

In such a situation, the rule building test\_suite.rpts typically has test\_suite.lst as a static dependency but the actual reports test1.rpt and test2.rpt are hidden dependencies, i.e. automatically discovered when building test\_suite.rpts.

Suppose now that you make a modification that makes test2.rpt very heavy to generate. Knowing that, you change your test\_suite so list a lighter test3.rpt instead. The succession of jobs would then be the following :

*   \- test1.rpt and test2.rpt are rebuilt as they are out-of-date after your modification.
*   \- test\_suite.rpts is rebuilt to collate theses reports.
*   \- `lmake` then sees that test3.rpt is needed instead of test2.rpt.
*   \- Hence, test3.rpt is (re)built.
*   \- test\_suite.rpts is finally built from test1.rpt and test3.rpt.

There are 2 losses of performance here :

*   \- test2.rpt is unnecessarily rebuilt.
*   \- test1.rpt and test3.rpt are rebuilt sequentially.

The problem lies in the fact that test1.rpt and test2.rpt are rebuilt before `lmake` had a chance to re-analyze the test suite showing that the new tests are test1 and test3. Generally speaking, this is a good strategy : such modifications of the dependency graph happens rather rarely and speculating that it is pretty stable by building known dependencies before launching a job is the right option. But here, because collating is very light (something like just executing `cat` on the reports), it is better to check tests\_suilte.lst first, and if it changed, rerun the collation before ensuring (old) tests have run. This is the purpose of `lcritical_barrier`. The collating rule would look like :

*   \- read test\_suite.lst
*   \- call `lcritical_barrier`
*   \- call `ldepend` on the report listed in test\_suite.lst. This is optional, just to generate parallel dependencies instead of automatic sequential dependencies.
*   \- collate reports listed in test\_suite.lst

And the succession of job would be :

*   \- test\_suite.rpts is rebuilt before analyzing test1.rpt and test2.rpt because test\_suite.lst has changed.
*   \- `lmake` sees that test3.rpt is needed instead of test2.rpt.
*   \- Hence, test1.rpt and test3.rpt are (re)built in parallel.
*   \- test\_suite.rpts is finally built from test1.rpt and test3.rpt.

### 4.4 `ltarget`

Usage : `ltarget [-<short-option>|<--<long-option>]... files ...`

`ltarget` may be used to inform `lmake` that some files must be deemed as written as if you called the `open` system call on each of them. If for some reasons you run with autodep method `none`, this is the way to report targets.

It is possible to alter the target flags for these targets which, by default, have the flags mentioned in the rule.

The following options are supported :

*   `-u` or `--unlink` : report an unlink rather than writing
*   `-c` or `--crc` : generate a crc for this target (compulsery if Match)
*   `-d` or `--dep` : reads not followed by writes trigger dependencies
*   `-e` or `--essential` : show when generating user oriented graphs
*   `-m` or `--manual-ok` : ok to overwrite manual files
*   `-p` or `--phony` : unlinks are allowed (possibly followed by reads which are ignored)
*   `-s` or `--source-ok` : ok to overwrite source files
*   `-t` or `--stat` : inode accesses (stat-like) are not ignored
*   `-w` or `--write` : writes are allowed (possibly followed by reads which are ignored)
*   `-C` or `--no-crc` : do not generate a crc for this target (compulsery if Match)
*   `-D` or `--no-dep` : reads not followed by writes do not trigger dependencies
*   `-E` or `--no-essential` : do not show when generating user oriented graphs
*   `-M` or `--no-manual-ok` : not ok to overwrite manual files
*   `-P` or `--no-phony` : unlinks are not allowed (possibly followed by reads which are ignored)
*   `-S` or `--no-source-ok` : not ok to overwrite source files
*   `-T` or `--no-stat` : inode accesses (stat-like) are ignored
*   `-W` or `--no-write` : writes are not allowed (possibly followed by reads which are ignored)

5 The Python `lmake` module
---------------------------

The `lmake` Python module serves 2 purposes :

*   provide base classes and helpers to write Lmakefile.py
*   provide some support during job execution

While unavoidable for the first purpose (writing Lmakefile.py), it is rarely necessary for job execution. When imported during job execution, the parts specificly targetted at writing Lmakefile.py are not defined. These are described in their respective chapters below.

Besides necessary variables, functions and classes to write Lmakefile.py config, rules and sources parts that are described in their respective chapters below, the `lmake` module defines a few constants that are of general use.

### 5.1 `class pdict`

This is a helpful tiny `class` deriving from `dict` that allows item access as attribute. This is just syntactic sugar to help the manipulation of configuration `dict` objects.

### 5.2 `def version(major,minor)`

This function is used to check that the expected version is compatible with the actual version.

Upon new releases of `lmake`, the major version is incremented if it is not backward compatible, else the minor version is increased if the interface is modified (i.e. new features are supported). Hence, the check is that major versions must match equal and the actual minor version must be at least the expected minor version.

This function must be called right after having imported the `lmake` module as it may adapt itself to the required version when this function is called. For example, some default values may be modified and if they are used before this function is called, a wrong (native) value may be provided instead of the correct (adjusted to required version) one.

### 5.3 Helpers for Lmakefile.py

#### 5.3.1 `class Rule`

This `class` is used as a base `class` for rules.

#### 5.3.2 `class AntiRule`

This `class` is used as a base `class` for anti-rules.

Anti-rules specify that target files that are not buildable. They have not dependencies and no execution.

#### 5.3.3 `class PyRule`

This `class` may be used as a base `class` for Python code doing imports.

It manages `.pyc` files so that user is not annoyed by Python generating `.pyc` files at esoteric places.

It relies on `stat` access made by Python to access the original `.py` to check `.pyc` validity. As a consequence, if you want to set the `ignore_stat` attribute on a `PyRule`, you must run python with the `-B` option set. Otherwise, dependencies to the original `.py` will be lost when the `.pyc` is available an up-to-date.

#### 5.3.4 `class DynamicPyRule`

This `class` may be used as a base `class` for Python code doing imports that are generated dynamically.

In addition to `PyRule` features, it patches Python import machinery to prevent optimizations that suppress accesses to inexisting modules. Accessing inexisting modules is the way to inform Python that this module is needed and must be generated.

#### 5.3.5 `class RustRule`

This `class` may be used as a base `class` to execute executable written in rust.

Rust uses a special link mechanis which defeats the default `ld_audit` autodep mechanism. This base `class` merely sets the autodep method to `ld_audit` which works around this problem.

#### 5.3.6 `class GitRule`

This `class` may be used to ignore all git management files. This files can be recognized as they lie in a directory named .git.

By itself, it is a dangerous `class` and must be used with care. User must ensure that all modifications to such git management files are reflected to the actual visible files outside these .git directories.

#### 5.3.7 `class HomelessRule`

This `class` opies the `TMPDIR` environment variable the `HOME` one. This is a way to ensure that various tools behave the same way as if they were run for the first time. By default the `HOME` environment variable points to the root of the repository, which permits to put various init code there.

#### 5.3.8 `class DirtyRule`

This `class` may be used to ignore all writes that are not an official target.

By itself, it is a dangerous `class` and must be used with care. It is meant to be a practical way to do trials without having to work out all the details, but in a finalized workflow, it is better to avoid the usage of this class.

#### 5.3.9 `def search_root_dir(cwd=os.getcwd())`

Return the root dir as seen from passed `cwd` argument.

Note that his may be different from the `root_dir` variable in case there is a Lmakefile.py file in a subdir.

### 5.4 constants

The following constants are defined :

`Kilo`

`1_000`

`Mega`

`1_000_000`

`Giga`

`1_000_000_000`

`inf`

`float('inf')`

`root_dir`

The root directory of the repository as determined when the `lmake` command was run.

Recursive sub-repositories are ignored. Use the `search_root_dir` function if you want to get the nearest hierarchical root dir.

`has_ld_audit`

`True` if autodep method `'ld_audit'` is supported, else `False`.

`has_ld_preload`

`True` if autodep method `'ld_preload'` is supported, else `False`.

`has_ptrace`

`True` if autodep method `'ld_ptrace'` is supported, else `False`.

`no_crc`

The 64-bit `int` checksum when it has not been computed (yet).

`crc_no_file`

The 64-bit `int` checksum produced when computed on a non-existent file.

### 5.5 Job execution support

The Python `lmake` module also provides a Python API to the job supporting command described above. The correspondance is :

`def check_deps()`

`lcheck_deps`

`def critical_barrier()`

`lcritical_barrier`

`def depend(*files,error=True,required=True,essential=True)` or `def depend(files,error=True,required=True,essential=True)`

`ldepend`

`def target(*files,unlink=False,crc,dep,essential,phony,source_ok,stat,write)` or `def target(files,unlink=False,crc,dep,essential,phony,source_ok,stat,write)`

`ltarget`. When a keyword argument other than `unlink` is not mentioned, the corresponding flag is the one mentioned in the rule.

6 Directories
-------------

By default, all administration data `lmake` needs to track the repository state is in the LMAKE directory at the root of the repository.

But most data are either accessed only by the `lmake` server, or by individual jobs, and need not be on a shared disk that is visible from the server and all execution hosts. Such disk are typically rather slow, compared to local disks.

So there a possibility to store administration data that do not mandate sharing into local disks.

### 6.1 `lmake.local_admin_dir`

This variable specifies the directory to use for data that are accessed by the `lmake` server only. It can be set to a directory mounted on a local disk which may be visible only by the `lmake` server (i.e. on the host on which you launch the `lmake` command). This directory must be unique for each repository, i.e. `lmake` considers it as a sandbox within which it can freely play.

### 6.2 `lmake.remote_admin_dir`

This variable specifies the directory to use for data that are accessed by the remote executing hosts only (not shared among them). It can be set to a directory mounted on a local disk of each remote hosts, i.e. it must exist on each remote host but may not designate the same are on all of them. This directory must be unique for each repository, i.e. `lmake` considers it as a sandbox within which it can freely play.

7 Config
--------

`lmake` can be configured to tailor its parameters to the user needs.

Configuration is done by setting attributes (or items) of the `pdict` `lmake.config`.

After `lmake` has been run (at least once), the configuration, as understood by `lmake` can be retrieved in the file LMAKE/config.

Recognized attributes are :

Attribute

Default

Description

`heartbeat`

`60`

`lmake` launches job through a batcher, one of them being the `local` batcher that launches jobs locally. also, a remote host executing job may reboot, or the job may be killed, or ... the batcher may simply lose it (yes, it happens from time to time, for example with SGE or slurm). In order to save resources (file descriptors), no connection is maintained between `lmake` and jobs while they are running (there may be thousands of them). As a consequence, `lmake` has no idea whether a job is running or disappeared. Without protection, a killed job would lead `lmake` to block, waiting undefinitely (or until its timeout if it already started) for it to start or complete. To avoid this, submitted jobs are watched periodically to check they still exist. This attriute provides the interval in seconds between such heartbeats.

`link_support`

`'Full'`

`lmake` has several levels of symbolic link support (see [link-support](#link_002dsupport)). This attribute specifies the used level.

`max_dep_depth`

`1000`

The rule selection process is a recursive one (see [rule-selection](#rule_002dselection)). Several means are provided to avoid infinite recursion and this one is the last resort. The search stops if the depth of the search reaches the value of this attribute, leading to the selection of a special internal rule called `'infinite'`.

`trace_size`

`100_000_000`

While `lmake` runs, it generates an execution trace recording a lot of internal events meant for debugging purpose. Such traces are stored in the directory LMAKE/trace. The larger the trace, the more probable the root cause of a potential problem is still recorded, but the more space it takes on disk. This attributes contains the maximum number of lines this trace can hold (`lmake` keeps the 5 last traces in case the root cause lie in a previous run).

`path_max`

This is one of the infinite recursion protection in the rule selection process (see [rule-selection](#rule_002dselection)). Any file with name longer than the value of this attribute is deemed not buildable.

`sub_prio_boost`

`1`

Increment to add to rules defined in sub-repository to boost local rules. It is multiplied by the directory depth of the sub-repository.

`console.date_precision`

`None`

This attribute specifies the precision (as the number of digit after the second field, for example 3 means we see milli-seconds) with which timestamps are generated on the console output. If `None`, no timestamp is generated.

`console.host_length`

`None`

This attribute specifies the width of the field showing the host that executed or is about to execute the job. If `None`, the host is not shown. Note that no host is shown for local execution.

`console.has_exec_time`

`True`

If this attribute is true, execution time is reported each time a job is completed.

`colors`

`lmake` generate colorized output if it is connected to a terminal (and if it understands the color escape sequences) (see [video-mode](#video_002dmode)). This attribute is a `pdict` with one entry for each symbolic color (see [video-mode](#video_002dmode)). Each entry is a 2-tuple of 3-tuple’s. The first 3-tuple provides the color in normal video mode (black/white) and the second one the color in reverse video (white/black). Each color is a triplet RGB of values between 0 and 255.

`backends`

This attribute is a `pdict` with one entry for each backend (see [backends](#backends) for an explanation of what is the purpose of backends, and see the backends section for a description of the attributes). Each entry is a pdict providing resources. Such resources are backend specific.

`caches`

This attribute is a `pdict` with one entry for each cache. Caches are named with an arbitrary `str` and are referenced in rules using this name. The attributes are described in the caches section below.

### 7.1 backends

Attribute

Default

Description

`backends.*.margin`

`0`

Special case of backends attribute is the `margin` entry, that must be present for all backends, that provide the minimum age (i.e. the difference between its ctime and the access time) for a freshly discovered dependency after which it is deemed acceptable. This margin may be necessary as some network file systems (for example some versions of NFS) do not **always** respect their promess of close-to-open consistency in heavy workload situations. When a dependency is known before job execution, `lmake` can insert code to check the file is actually accessible in its last version, but if a dependency is discovered during the execution, there is nothing `lmake` can do. As a work around, `lmake` only trusts such dependencies if the file is old enough that its update has had a chance to propagate. This value is to be determined empirically. Too small a value and you have (exceptionnally but not never) files with wrong content (or not existing at all). Too large a value, you have more jobs being rerun while they went perfectly fine.

`backends.*.address`

best guess

When job are launched remotely, they must connect to `lmake` when they start and when they complete. This is done by connecting to a socket `lmake` has open for listening, which requires that we must have a means to determine an IP address to connect to. The host running `lmake` may have several network interfaces, one of them (typically only one) being usable by such remote hosts. There is no generic way to determine this address, so in general, `lmake` cannot determine it automatically.  
This address may be empty (loop-back), given in standard dot notation, the name of an interface (as shown by `ifconfig`) or the name of a host (looked up as for `ping`). By default, loop-back is used for the local backend and the hostname (as given by `hostname`) for the others.

`backends.local.cpu`

number of physical CPU’s

This is a normal resource rules can require (which is the case if resources are defaulted)

`backends.local.mem`

size of physical memory in MB

This is a normal resource rules can require (which is the case if resources are defaulted)

### 7.2 caches

By default, no cache is configured, but an example can be found in lib/lmake.py, commented out.

Attribute

Default

Description

`caches.*.tag`

\-

This attribute specifies the method used by `lmake` to cache values. In the current version, only 2 tags may be used :

*   \- `'none'` is a cache that caches nothing. No further configuration is required for such a cache.
*   \- `'dir'` is a cache working without daemon. The data are stored in a directory.

`caches.<dir>.repo`

\-

Valid only when `tag` is `'dir'`. This attribute specifies a key identifying the repository. In order to avoid poluting the cache during typical edit-run-debug loops with data that will never be reused, the cache restrict its data to at most one entry for each job in each repo. This attribute is used to identiy a repository. If 2 repositories use the same key, then results produced in one will replace those produced in the other one. Besides this restriction, a classical LRU algorithm is used.

`caches.<dir>.dir`

\-

Valid only when `tag` is `'dir'`. This attribute specifies the directory in which the cache puts its data.

`caches.<dir>.size`

\-

Valid only when `tag` is `'dir'`. This attribute specifies the maximum size the cache can occupy.

8 Sources
---------

Sources are files that are deemed as intrinsic. They cannot be derived using rules as explained in the following chapter.

Also, if a file cannot be derived and is not a source, it is deemed unbuildable, even if it actually exists. In this later case, it will be considered dangling and this is an error condition. The purpose of this restriction is to ensure repeatability : all buildable files can be (possibly indirectly) derived from sources using rules.

The list of sources is provided to `lmake` by setting the lmake.sources variable.

The following helper functions can be used to provide sources. If nothing is mentioned in Lmakefile.py, `auto_sources()` is called.

### 8.1 `manifest_sources(manifest='Manifest',**kwds)` :

This function sets `lmake.sources` from the content of the file provided in `manifest`.

`kwds` is ignored.

It raises a `FileNotFoundError` exception if `manifest` cannot be found.

### 8.2 `git_sources(recurse=True,ignore_missing_submodules=False,**kwds)` :

This function sets `lmake.sources` by listing files managed by `git`.

If `recurse`, sub-modules are searched as well.

If `ignore_missing_submodules`, missing sub-modules are ignored rather than generating an error.

`kwds` is ignored.

It raises a `FileNotFoundError` exception if the repository is not controled by `git`.

### 8.3 `auto_sources(**kwds)` :

This function successively tries `manifest_sources` and `git_sources` by passing its `kwds` argument until one succeeds.

It raises a `FileNotFoundError` exception if none succeeds.

9 Rules
-------

Rules are described as Python `class`’es inheriting from `lmake.Rule` or `lmake.AntiRule`.

Inheriting from `lmake.Rule` is used to define production rules that allows deriving targets from dependencies.

Inheriting from `lmake.AntiRule` is (rarely) used to define rules that specify that matching targets **cannot** be built. Antirules only require the `targets` attribute (or those that translate into it, `target`, `post_targets` and `post_target`) and may usefully have a `prio` attribute. Other ones are useless and ignored.

Rules are described by a series of attribute as follows.

### 9.1 `combine`

Inheritance

Combined

Type

`set`

Default

The list of attributes hereinafter described as having combined inheritance

Example

`{'my_dict_attribute'}`

This attribute specify a set of attribute names to be processed with combined inheritance (see [rule-inheritance](#rule_002dinheritance)). Combined attributes may only be `dict`, `set` and `list`. `dict`’s and `set`’s are `update`d, `list`’s are `append`ed. `dict`’s and `list`’s are ordered in reverse MRO, base classes being before derived classes.

### 9.2 `name`

Inheritance

None

Type

`str`

Default

`cls.__name__`

Example

`'compile and link'`

This attribute specify a name for the rule. This name is used each time `lmake` needs to mention the rule in a message.

All rules must have a unique name. Usually, the default value is fine, but if your rule is defined in a for loop for example, then you have several definitions with the same `__name__` and you must distinguish them from each other with this attribute (usually an f-string with the loop index in it).

### 9.3 `virtual`

Inheritance

None

Type

`bool`

Default

`True` if `cls` lacks the essential attributes to make it a rule (`targets` and, if not anti, `deps` and `cmd`).

Example

`True`

When this attribute exists and has a `True` value, this `class` is not a rule and is only used as a base `class` to define concrete rules.

### 9.4 `prio`

Inheritance

Python

Type

`float`

Default

`0` if inheriting from `lmake.Rule`, `float('+inf')` if deriving from `lmake.AntiRule`.

Example

`1`

This attribute is used to order matching priority. Rules with higher priorities are tried first and if none of them are applicable, rules with lower priorities are then tried (see [rule-selection](#rule_002dselection)).

### 9.5 `stems`

Inheritance

Combined

Type

`dict`

Default

`{}`

Example

`{'File':r'.*'}`

Stems are regular expressions that represent the variable parts of targets which rules match.

Each entry <key>:<value> define a stem named <key> whose associated regular expression is <value>.

### 9.6 `job_name`

Inheritance

Python

Type

`str`

Default

The first matching target of the most derived `class` in the inheritance hierarchy (i.e. the MRO) having a matching target.

This attribute may exceptionally be used for cosmetic purpose. Its syntax is the same as target name (i.e. a target with no option).

When `lmake` needs to include a job in a report, it will use this attribute. If it contains star stems, they will be replaced by `*`’s in the report.

If defined, this attribute must have the same set of static stems (i.e. stems that do not contain \*) as any matching target.

### 9.7 `targets`

Inheritance

Combined

Type

`dict`

Default

`{}`

Example

`{ 'OBJ' : '{File}.o' }`

This attribute is used to define the regular expression which targets must match to select this rule (see [rule-selection](#rule_002dselection)).

Keys must be Python identifiers. Values are `list`’s or `tuple`’s whose first item defines the target regular expression and following items define flags. They may also be a simple `str` in which case it is as if there were no associated flags.

The regular expression looks like Python f-strings. The fixed parts (outside {}) must match exactly. The variable parts, called stems, are composed of :

*   An optional name. If it exists, it is used to ensure coherence with other targets and the `job_name` attribute, else coherence is ensured by position. This name is used to find its definition in the stems `dict` and may also be used in the `cmd` attribute to refer to the actual content of the corresponding part in the target.
*   An optional \*. If it exists, this target is a star target, meaning that a single job will generate all or some of the targets matching this regular expression. if not named, such stem must be defined.
*   An optional : followed by a definition (a regular expression). This is an alternative to refering to an entry in the `stems` `dict`. Overall, all stems must be defined somewhere (in the `stems` `dict`, in a target or in `job_name`) and if defined several times, definitions must be identical. Also, when defined in a target, a definition must contain balanced `{}`’s, i.e. there must be as many `{` as `}`. If a regular expression requires unbalanced `{}`, it must be put in a `stems` entry.

The flags may be any combination of the following flags, optionally preceded by - to turn it off when it is present by default.

CamelCase Flag

snake\_case Flag

Default

Description

Crc

crc

Yes

A checksum is computed is computed on these targets when generated. Compulsery for matching targets.

Dep

dep

Yes

If such a file is read and not written, it will actually be a dependency, not a target.

Essential

essential

Yes

This target will be shown in a future graphic tool to show the workflow, it has no algorithmic effect.

Incremental

incremental

No

Previous content may be used to produce these targets. In that case, these are not unlinked before execution.

ManualOk

manual\_ok

No

Modifying these targets outside `lmake` control will not prevent job from executing.

Match

match

Yes

This entry may be used to select this rule.

Phony

phony

No

A possible content for target is to be not present on disk. If a star target, all matching files are deemed generated by this rule.

SourceOk

source\_ok

No

Do not trigger an error if target is indeed a source. This may occur if another target is required or if this target is a star target (otherwise, the rule will not be executed as target will be recognized as a source).

Star

star

if star stem present

Target is a star target, even if no star stem is present (which means next rule will be tried if file is not generated).

Stat

stat

Yes

`inode` accesses (e.g. stat) are deeemed to be read (else they are ignored).

Warning

warning

No

Warning is reported if a target is unlinked before job execution while generated by another job.

Write

write

Yes

Job is allowed to write to this target.

All matching targets must have the same set of static stems (i.e. stems with no \* in its name).

Matching is done by first trying to match static targets (i.e. which are not star) then star targets. The first match will provide the associated stem definitions and flags.

In order to support hierarchical repository (see [hierarchical-repositories](#hierarchical_002drepositories)) :

*   \- if target regular expression starts with a `/`, it is removed
*   \- else target is prepended with the `cwd` attribute.

### 9.8 `target`

Inheritance

Python

Type

`str` or `list` or `tuple`

Default

\-

This attribute defines an unnamed target as the last target. Its syntax is the same as any target entry except that it may not be `incremental`, `phony` nor `star`.

During execution, `cmd` stdout will be redirected to this (necessarily unique since it cannot be a `star`) target.

It is exclusive with the `post_target` attribute.

### 9.9 `post_targets`

Inheritance

Combined

Type

`dict`

Default

`{}`

This attribute is identical to `targets` except that :

*   \- their entries are in reversed order
*   \- they are put at the end of the targets

### 9.10 `post_target`

Inheritance

Python

Type

`str` or `list` or `tuple`

Default

\-

This attribute is identical to `target` except that :

*   \- it is defined as the last `post_targets` (before being reversed)
*   \- it is exclusive with the `target` attribute.

### 9.11 `deps`

Inheritance

Combined

Type

`dict`

Default

`{}`

Example

`{ 'SRC' : '{File}.c' }`

This attribute defines the static dependencies. It is a `dict` which associates Python identifiers to files computed from the available environment.

They are f-strings, i.e. their value follow the Python f-string syntax and semantic but they are interpreted when `lmake` tries to match the rule (the rule only matches if static dependencies are buildable, see [rule-selection](#rule_002dselection)). Hence they lack the initial `f` in front of the string.

Accessible variables when evaluating the f-string include the `stems`, the `targets`, attributes and the module dictionary.

Alternatively, values can also by `list` or `tuple` whose first item is as described above, followed by flags.

The flags may be any combination of the following flags, optionally preceded by - to turn it off when it is present by default.

CamelCase Flag

snake\_case Flag

Default

Description

Error

error

Yes

This dependency must not be in error for this job to run.

Essential

essential

Yes

This dependency will be shown in a future graphic tool to show the workflow, it has no algorithmic effect.

Required

required

Yes

This dependency must be buildable for this job to run.

Flag order and dependency order is not significative.

In order to support hierarchical repository (see [hierarchical-repositories](#hierarchical_002drepositories)) :

*   \- if dep starts with a `/`, it is removed
*   \- else dep is prepended with the `cwd` attribute.

### 9.12 `dep`

Inheritance

Python

Type

`str`

Default

\-

This attribute defines an unnamed static dependency.

During execution, `cmd` stdin will be redirected to this dependency.

### 9.13 `chroot`

Inheritance

Python

Type

`str`

Default

\-

This attribute defines a directory in which jobs will `chroot` into before execution begins.

The job wrapper itself stays in the original file system in which it is launched, only the job does a `chroot`.

### 9.14 `cwd`

Inheritance

Python

Type

`str`

Default

The nearest root dir as seen from the module defining the rule.

This attribute defines a directory in which jobs will `chdir` into before execution begins.

This attribute is also prepended to targets and deps unless they start with a `/`.

### 9.15 `env`

Inheritance

Combined

Type

`dict`

Default

See description

Example

`{ 'MY_TOOL_ROOT' : '/install/my_tool' }`

This attributes defines the environment in which the jobs will execute, much the same way as Python’s `os.environ`.

The syntax of the values can be :

*   \- a `str`, in which case it is used as the value of the environment variable with the tag `'cmd'`.
*   \- a 2-`tuple`, in which case the first entry is used as the value of the environment variable and the second value is a tag that may be :
    *   \- `'Cmd'` or `'cmd'` : `lmake` will interpret the value as part of the `cmd`, i.e. jobs will be rerun upon modification.
    *   \- `'Resources'` or `'resources'` : `lmake` will interpret the value as part of the `resources`, i.e. jobs will be rerun upon modification if they are in error.
    *   \- `'None'` or `'none'` : `lmake` will interpret the value as part of nothing, i.e. jobs will not be rerun upon modification.

The environment in which the `lmake` command is run is ignored so as to favor reproducibility.

By default the following environment variables are defined :

`HOME`

The root directory of the repository, so as to prevent tools from accessing startup files that may vary from user to user. Necessary startup files can be put inside the repository, administered with your prefered versioning tool as any other source.

`PATH`

The standard path with `lmake` bin directory in front. The standard path is the one you get with the standard shell in absence of any startup file.

`PYTHONPATH`

The `lmake` lib directory.

Note that the current environment is accessible when Lmakefile.py is read, so that it is quite simple to copy some variables from the user environment although this practice is discouraged and should be used with much care.

### 9.16 `python`

Inheritance

Python

Type

`list` or `tuple`

Default

system Python

This attribute defines the interpreter used to run the `cmd` if it is a `function`. At the end of the supplied executable and arguments, `'-c'` and the actual script is appended.

### 9.17 `shell`

Inheritance

Python

Type

`list` or `tuple`

Default

`/bin/bash`

This attribute defines the interpreter used to run the `cmd` if it is a `str`. At the end of the supplied executable and arguments, `'-c'` and the actual script is appended.

### 9.18 `cmd`

Inheritance

Combined

Type

`function` or `str`

Default

\-

Example1

`'gcc -c -o $OBJ $SRC'`

Example2

`def cmd() : subprocess.run(('gcc','-c','-o',OBJ,SRC,check=True))`

#### 9.18.1 if it is a `function`

In that case, this attribute is called to run the job. Combined inheritance is a special case for `cmd`.

If several definitions exist along the MRO, They must all be `function`’s and they are called successively in reverse MRO. The first (i.e. the most basic) one must have no non-defaulted arguments and will be called with no argument. The other ones may have arguments, all but the first having default values. In that case, such `function`’s are called with the result of the previous one as unique argument. Else, if a `function` has no argument, the result of the previous function is dropped.

During evaluation, when the job runs, its global `dict` is populated to contain values referenced in these `function`’s. Values may come from (by order of preference) :

*   \- The `stems`, `targets`, `deps`, `resources` as named in their respective `dict`.
*   \- `stems`, `targets`, `deps`, `resources` that contain their respective whole `dict`, `job_tokens` that contain its value.
*   \- Any attribute defined in the class, or a base class (as for normal Python attribute access).
*   \- Any value in the module `dict`.
*   \- Any builtin value
*   \- undefined variables are not defined, which is ok as long as they are not accessed.

Because jobs are executed remotely using the interpreter mentioned in the `python` attribute and to avoid depending on the whole Lmakefile.py (which would force to rerun all jobs as soon as any rule is modified), these `function`’s and their context are serialized to be transported. The serialization process may improve over time but as of today, the following applies :

*   \- Basic objects are transported as is : `None`, `...`, `bool`, `int`, `float`, `complex`, `str`, `bytes`.
*   \- `list`, `tuple`, `set` and `dict` are transported by transporting their content. Note that reconvergences (and a fortiori loops) are not handled.
*   \- `function`’s are transported as their source accompanied with their context : global accessed variables and default values for arguments.
*   \- Imported objects (`function`’s and `class`’es and generally all objects with a `__qualname__` attribute) are transported as an `import` statement.
*   \- Builtin objects are transported spontaneously, without requiring any generated code.

Values are captured according to the normal Python semantic, i.e. once the `Lmakefile` module is fully imported. Care must be taken for variables whose values change during the `import` process. This typically concerns loop indices. To capture these at definition time and not at the end, such values must be saved somewhere. There are mostly 2 practical possibilities :

*   \- Declare an argument with a default value. Such default value is saved when the function is defined.
*   \- Define a class attribute. Class attributes are saved when its definition ends, which is before a loop index.

The job is deemed to be successful if the last function returns a false value.

#### 9.18.2 if it is a `str`

In that case, this attribute is run to run the job. Combined inheritance is a special case for `cmd`. While walking the MRO, if for a base class `cmd` is defined as a `function` and it has a `shell` attribute, the value of this attribute is used instead. The purpose is that it is impossible to combine `str`’s and `function`’s because they use different paradigms. As a consequence, a base class may want to have 2 implementations, one for subclasses that use Python `cmd` and another for subclasses that use shell `cmd`. For such a base class, the solution is to define `cmd` as a `function` and set its `shell` attribute to the `str` version.

If several definitions exist along the MRO, They must all be `str`’s and they are run successively in reverse MRO in the same process. So, it is possible for a first definition to define an environment variable that is used in a subsequent one.

During evaluation, when the job runs, its environment is augmented with :

*   \- The `stems`, `targets`, `deps`, `resources` as named in their respective `dict`.
*   \- The `job_tokens` variable contains the associated value.

Such variables must appear in the core of one of the definitions as a whole word.

The job is deemed to be successful if the return code of the overall process is `0`.

### 9.19 `cache`

Inheritance

Python

Type

`str`

Constraint

One of the caches listed in config.

Default

<not cached>

This attribute specifies the cache to use for jobs executed by this rule. When a job is executed, its results are stored in the cache. If space is needed (all caches are constrained in size), any other entry can be replaced. The cache policy (described in its own section, in the config chapter) tries to identify entries that are likely to be useless in the future.

### 9.20 `backend`

Inheritance

Python

Type

`str`

Constraint

One of the supported backends.

Default

`'local'`

This attribute specifies the backend to use to launch jobs (see [backends](#backends)).

### 9.21 `autodep`

Inheritance

Python

Type

`str`

Constraint

One of `'none'`, `'ld_preload'`, `'ld_audit'` or `'ptrace'`

Default

`'ld_audit'` if supported else `'ld_preload'`

This attribute specifies the method used by autodep (see [autodep](#autodep)) to discover hidden dependencies.

### 9.22 `resources`

Inheritance

Combined

Type

`dict`

Default

`{}`

Example

`{ 'MY_RESOURCE' : '1' }`

This attribute specifies the resources required by a job to run successfully. These may be cpu availability, memory, commercial tool licenses, access to dedicated hardware, ...

The syntax is the same as for `deps`, except that in addition to other variables, deps can be referenced.

After interpretation, the `dict` is passed to the `backend` to be used in its scheduling (see [local-backend](#local_002dbackend) for the local backend).

### 9.23 `stderr_len`

Inheritance

Python

Type

`int`

Default

\-

This attribute defines the maximum number of lines of stderr that will be displayed in the output of `lmake`. The whole content of stderr stays accessible with the `lshow -e` command.

### 9.24 `allow_stderr`

Inheritance

Python

Type

`bool`

Default

`False`

When this attribute has a false value, the simple fact that a job generates a non-empty stderr is an error. If it is true, writing to stderr is allowed and does not produce an error. The `lmake` output will exhibit a warning, though.

### 9.25 `auto_mkdir`

Inheritance

Python

Type

`bool`

Default

`False`

When this attribute has a true value, executing a `chdir` syscall (e.g. executing `cd` in bash) will create the target directory if it does not exist.

This is useful for scripts in situations such as :

*   \- The script does `chdir a`.
*   \- Then try to read file b from there.
*   \- What is expected is to have a dependency on a/b which may not exist initially but will be created by some other job.
*   \- However, if directory a does not exist, the `chdir` call fails and the file which is open for reading is b instead of a/b.
*   \- As a consequence, no dependency is set for a/b and the problem will not be resolved by a further re-execution.
*   \- Setting this attribute to true creates directory a on the fly when `chdir` is called so that it succeeds and the correct dependency is set.

### 9.26 `ignore_stat`

Inheritance

Python

Type

`bool`

Default

`False`

When this attribute is set to a true value, stat-like syscalls (i.e. syscalls that access the i-node only) are ignored. The `'-Stat'` flag on targets provide a more flexible mecanism. The only advantage of this attribute is that precisely because it is rather crude, stat-like syscalls can be filtered very early and performances are better.

### 9.27 `keep_tmp`

Inheritance

Python

Type

`bool`

Default

`False`

When this attribute is set to a true value, the temporary directory is kept after job execution. It can be retreived with `lshow -i`. Sucessive executions of the same job overwrite the temporary directory, though, so only the content corresponding to the last execution is available. When this attribute has a false value, the temporary directory is washed at the end of the job execution.

### 9.28 `force`

Inheritance

Python

Type

`bool`

Default

`False`

When this attribute is set to a true value, jobs are always considered out-of-date and are systematically rerun if a target is needed. It is rarely necessary

### 9.29 `timeout`

Inheritance

Python

Type

`float`

Constraint

\>=0

Default

no timeout

When this attribute has a non-zero value, job is killed and a failure is reported if it is not done before that many seconds.

### 9.30 `job_tokens`

Inheritance

Python

Type

`str` or `int`

Constraint

\>=0 and <256

Default

`1`

This attribute has the same syntax as a resource, except that a `float` is accepted. If it is a `str`, it must be convertible to a float after interpretation.

It is only used to estimate the ETA (see [eta-estimation](#eta_002destimation)).

### 9.31 `n_tokens`

Inheritance

Python

Type

`int` ()

Constraint

\>0

Default

`1`

This attribute indicates to `lmake` a quantity of abstract resources used to estimate the expected parallelism of jobs within this rule.

It is only used to estimate the ETA (see [eta-estimation](#eta_002destimation)).

### 9.32 `start_delay`

Inheritance

Python

Type

`float`

Default

`0`

When this attribute is set to a non-zero value, start lines are only output for jobs that last longer than that many seconds. The consequence is only cosmetic, it has no other impact.

### 9.33 `kill_sigs`

Inheritance

Python

Type

`list` or `tuple`

Default

`()`

This attributes provide a list of signals to send the job when `lmake` decides to kill it. A job is killed when :

*   \- ^C is hit if it is not necessary for another running `lmake` command which has not received a ^C.
*   \- When timeout is reached.
*   \- When `check_deps` is called and some dependencies are not up to date.

The signals listed in this list are sent in turn, once every second. If the list is exhausted and the job is still alive, `KILLSIG`’s are sent, once per second. Longer interval can be obtained by inserting `0`’s. `0` signals are not sent and anyway, these would have no impact if they were.

10 The LMAKE directory
----------------------

This directory contains numerous informations that may be useful to the user.

There are other files than those described here which are for `lmake` own management

### 10.1 LMAKE/config

This file contains a description of the `lmake.config` `dict` as it has been understood by `lmake` after having processed Lmakefile.py.

### 10.2 LMAKE/rules

This file contains a description of the rules as it has been understood by `lmake` after having processed Lmakefile.py.

### 10.3 LMAKE/sources

This file contains a description of the sources as it has been understood by `lmake` after having processed Lmakefile.py.

### 10.4 LMAKE/makefiles

This file contains a list of files that `lmake` has read to process Lmakefile.py.

### 10.5 LMAKE/outputs/<date>

This file contains a transcript of the `lmake` command that has been run at <date>.

### 10.6 LMAKE/last\_output

This file is a symbolic link to the last transcript.

### 10.7 LMAKE/targets

This file contains the targets that have been required by `lmake` commands in chronological order (with duplicates removed).

11 Some considerations
----------------------

This chapter contains a some considerations that appear in several places of this documentation. It is meant to be the target of cross-references.

### 11.1 Rule inheritance

Rules are Python `class`’es that inherit from `lmake.Rule` (or `lmake.AntiRule` for anti-rules).

However, Python’s native inheritance mechanism is not ideal to describe a rule as one would like to prepare a base `class` such as :

*   \- provide environment variables
*   \- provide some default actions for some files with given pattern
*   \- provide some automatic dependencies
*   \- ...

As these are described with `dict`, you would like to inherit `dict` entries from the base `class` and not only the `dict` as a whole. A possibility would have been to use the `__prepare__` method of a meta-class to pre-define inherited values of such attributes, but that would defeat the practical possibility to use multiple inheritance by suppressing the diamond rule.

The chosen method has been to walk through the MRO at class creation time and :

*   \- Define a set of attributes to be handled through combination. This set is defined by the attribute `combine`, itself being handled by combination.
*   \- Combined attribute are handled by updating/appending rather than replacing when walking through MRO in reverse order.
*   \- Entries with a value None are suppressed as update never suppress a given entry. Similarly, values inserted in a set prefixed with a `'-'` remove the corresponding value from the `set`.

Because this mechanism walks through the MRO, the diamond rule is enforced.

### 11.2 Backends

Backends are in charge of actually launching jobs when the `lmake` engine has identified that it had to be run. It is also in charge of :

*   \- Killing jobs when the `lmake` engine has identified it had to be so.
*   \- Scheduling jobs so as to optimize the runtime, based on some indications provided by the `lmake` engine.
*   \- Rescheduling jobs when new scheduling indications becomes available.

A backend has to take decisions of 2 kinds :

*   \- Is a job eligible for running. From a dependency perspective, the `lmake` engine guarantees it is so. But the job needs some resources to run and these resources may already be busy because of some other jobs already running.
*   \- If several jobs are eligible, which one(s) to actually launch.

Each backend is autonomous in its decisions and has its own algorithm to take them. However, generally speaking, they more or less work by following the following principles :

*   \- For the first question, the backend maintain a pool of available resources and a job is eligible if its required resources can fit in the pool. When launched, the required resources are subracted from the pool and when terminated, they are returned to it.
*   \- For the second question, each job has an associated pressure provided by the `lmake` engine and the backend actually launches the eligible job with the highest pressure.

The required resources are provided by the `lmake` engine to the backend as a `dict` which is the one of the job’s rule after f-string interpretation.

The pressure is provided in the form of `float` computed as the accumulated ETE along the critical path to the final targets asked on the `lmake` command line. To do that, future job ETE have to be estimated. For jobs that have already run, last successful execution time is used. When this information is not available, i.e. when the job has never run successfully, a moving average of the execution times of the jobs sharing the same rule is used as a best guess.

The `lmake` backend also provides the current ETA (see [eta-estimation](#eta_002destimation)) of the final targets to allow the backends from different repository to take the best collective decision.

As of now, the only existing backend is the local backend, but a slurm backend is scheduled shortly.

### 11.3 Local backend

The local backend launches jobs locally, on the host running the `lmake` command. Also, there is no cooperation between backends from different repositories and the user has to ensure there is no global resource conflict.

This backend is configured by providing entries in the `lmake.config.backends.local` `dict`. The key identifies the resource and the value is a `int` that identifies a quantity.

Then, each rule whose `backend` attribute is `'local'` provides a `resources` attribute such that :

*   \- The key identifies a resource (which must match a resource in the configuration).
*   \- The value (possibly tailored by job through the use of the f-string syntax) is either
    
    *   \- a `int` or a `str` that can be interpreted as `int`
    *   \- or a `str` of the form `'a<b'` where `a` and `b` can be interpreted as `int`
    
    This later form instruct the local backend that as much as `b` resources is preferable, but `a` is enough to run the job.
    

The variable available to the job as global variables (python case) or environment variables (shell case) contains the actual quantity of resources allocated to this job.

The local backend ensures that the sum of all the resources of the running jobs never overshoot the configured available quantity.

By default, the configuration contains 2 resources : `cpu` and `mem` configured respectively as the overall number of available cpus and the overall available memory (in MB). Each rule has a default `resources` attribute requiring `1` `cpu` and the corresponding share of `mem`.

### 11.4 Autodep

Autodep is a mechanism through which jobs are spied to automatically detect which disk accesses are done. From this information `lmake` can determine if accesses were within the constraints provided by the rule and the list of the hidden dependencies.

Several methods are available to spy jobs. Not all methods are supported on all systems.

#### 11.4.1 `'None'` or `'none'`

The job is not instrumented. The only intrusion is the presence of the environment variable `LMAKE_AUTODEP_ENV`.

The only way to report activity is through the use of `ldepend` and similar commands or functions available in the `lmake` module.

The main adavantage is that this method is not invasive (or marginally). The main inconvenient is that no automatic hidden dependencies are recorded, one has to explicitely call `ldepend` and experience shows it is very difficult not to forget some.

This method is **not** recommanded.

#### 11.4.2 `'LdPreload'` or `'ld_preload'`

The job is run with a library loaded using the `LD_PRELOAD` environment variable that catches some calls libc.so (such as `open`) to track activity. This requires libc.so to be dynamically linked. This method is very performant.

The main advantage is that this method is available even on rather old versions of Linux. The main inconvenient is that it is somewhat invasive and there are cases of incompatibilities (e.g. the rust compiler). Also, it will not run correctly if some commands have the libc statically linked and this will go undetected.

This method is recommanded on systems that lack the rtld-audit capability.

#### 11.4.3 `'LdAudit'` or `'ld_audit'`

This method is similar to `ld_preload` except that the rtld-audit (through the `LD_AUDIT` environment variable) mechanism is used instead of `LD_PRELOAD`, which is less invasive (cases where it is not transparent are very awkward). This also requires libc.so to be dynamically linked but failure to do so can be detected and an error is generated, mandating the use of another method (except `ld_preload`).

The main advantage is that is is both performant, very little invasive and static linkage of `libc` is exceptional (and can be detected). The main inconvenient is that it requires a rather recent version of Linux.

This method is recommanded on systems that support it.

#### 11.4.4 `'Ptrace'` or `'ptrace'`

The job is run `ptrace`’ed. The seccomp mechanism is used to reduce the performance hit to the minimum possible but still, it is often unacceptable. There is no requirement that libc.so be dynamically linked. It is almost not invasive (mostly the job cannot use ptrace itself, i.e. you will not be able to run an executable level debugger).

Th main adavantage is that it works with a statically linked `libc`. The main inconvenient is that the performance hit can be severe.

This method is recommanded as a fall back when the previous (`ld_preload` and `ld_audit`) methods cannot be used.

### 11.5 Link support

`lmake` has several levels of symbolic link support :

*   \- If `'None'` or `'none'`, Symbolic links are not supported, no effort is done to resolve them.
*   \- If `'File'` or `'file'`, Symbolic links are supported only when they point to files (much like hard links), so intermediate directories are not checked for symbolic links.
*   \- If `'Full'` or `'full'`, All symbolic links are supported. This is the most secure but also the most heavy in terms of performances as all intermediate directories have to be checked.

### 11.6 Rule selection

When `lmake` needs to ensure that a file is up to date, the first action is to identify which rule, if any, must be used to generate it. This rule selection process works in several steps described below.

A file is deemed buildable if the rule selection process leads to a job that generates the file.

#### 11.6.1 Up-hill directory

The first step is to see if a up-hill directory (i.e. one of the dirtory along the directory path leading to the file) is (recursively) buildable.

If it is the case, the rule selection process stops here and the file is not buildable. Else the process continue.

The rules are split into groups. Each group has either `Rule`’s or `AntiRule`’s and contain all of them which share a given `prio`. Groups are ordered with higher `prio` first and within each `prio`, the `AntiRule` group before the `Rule` group.

The following steps is executed for each group in order, until a rule is found or the file declared not buildable.

#### 11.6.2 Match a target

For a given rule, the file is matched against each target in turn. Static targets are tried first in user order, then star targets in user order, and matching stops at the first match.

If a target matches, the matching defines the value of the static stems (i.e. the stems that appear without a `*`). Else, the rule does not apply.

If the rule is an `AntiRule`, the rule selection process stops and the file is not buildable.

#### 11.6.3 Check static dependencies

The definition of the static stems allow to compute :

*   \- The other targets of the rule. Static targets become the associated file, star targets becomes regular expressions in which static stems are expanded.
*   \- Static dependencies by interpreting them as f-strings in which static stems and targets are defined.

Static dependencies are then analyzed to see if the are (recursively) buildable, and if any is not buildable, the rule does not apply.

#### 11.6.4 Group recap

After these 2 previous steps have been done for the rules of a group, the applicable rules are analyzed the following way :

*   If no rule apply, next group is analyzed.
*   If the file matches several rules as a sure target (i.e. a static target or a star target with the `phony` flag), the file is deemed buildable, but if required to run, no job will be executed and the file will be in error.
*   If the file matches some rules as a non-sure target (i.e. a star target without the `phony` flag), the corresponding jobs are run. If no such jobs generate the file, next group is analyzed. If several of them generate the file, the file is buildable and in error.

### 11.7 Hierarchical repositories

Hierarchical repositories are repositories that contain repositories, i.e. some Lmakefile.py are present in sub-directories.

In that situation, it is reasonable to assume that the Lmakefile.py are made to handle building files underneath it.

To support this situation, `lmake` allow you to simply import these Lmakefile.py and the `cwd` attributes will automatically be set to the right value so that :

*   Targets on match within the sub-repo (and escape is possibly by prepending a `/` to the target to provide global rules.
*   The same is applied to deps
*   `cmd` is run from this sub-repository, i.e. its cwd is set accordingly.
*   The priority of the rule is boosted by adding to it the directory depth of the sub-repository times `lmake.config.sub_prio_boost`.

### 11.8 ETA estimation

An ETA estimation is made possible because the execution time for each job is recorded in `lmake` book-keeping after all successful runs (if a job ends in error, it may very well have been much faster and the previous execution time is probably a better estimate than this one). When a job has never run successfully, an ETE is used instead of its actual execution time by taking a moveing average of all the jobs of the same rule.

This being given, a precise ETA would require a fake execution of the jobs yet to be run which can take all dependencies and resources into account. But this is way too expensive, so a faster process must be done, even at the expense of precision.

In all cases, the ETA assumes that no new hidden dependencies are discovered and that no file is steady so that all jobs currently remaining will actually be executed.

2 approaches can be considered to estimate the time necessary to carry out remaining jobs :

*   Resources limited : dependencies are ignored, only resources are considered. Roughly, the time is the division of the quantity of resources necessary by the quantity of resources available. For example, if you need 10 minutes of processing and you have 2 cpus, this will last 10/2=5 minutes.
*   Dependencies limited : resources are ignored and only dependencies are considered. This means you only look at the critical path. For example if you need to run a 2 minutes job followed by a 3 minutes job, and in parallel you must run a 4 minutes job, this will last 2+3=5 minutes.

`lmake` uses the first approach. For that it must know the critical resource for each job. This is the purpose of the `n_tokens` and `job_tokens` attributes of the rules. Each job accounts for ETE\*`job_tokens`/`n_tokens`, and all these are accumulated to produce an overall time. The ETA is then the current time + this accumulated time.

### 11.9 Video mode and colors

If lmake is connected a terminal, then the terminal foreground and background colors are probed and if the brightness of the background color is less than that of the foreground color, video mode is set to normal, else it is set to reverse.

In that case, lmake output is colored and the (configurable) color set is chosen depending on video mode.

12 FAQ
------

### 12.1 How do I prevent `lmake` to set dependencies to a given directory

Maybe somewhat counter-intuitive, the solution is to declare a target. If you declare a target matching all files in your directory with the following flags :

*   \- `'-Match'` : this prevents matching on these files from other rules. In exchange you gain a lot of flexibility (for example your are not obliged to specify all static stems).
*   \- `'-Dep'` : this prevents read matching files to becode a dependency (as normally the case, even for a target). This is the major goal of this target.
*   \- `'-Write'` : you may want to add this flag to consider writing to such files as an error, good if you are supposed to have read-only accesses.
*   \- `'Incremental'` : you may want to add this flag if, on the contrary, you want to allow the rule to do update (i.e. a read followed by a write) to matching files.
*   \- `'-Crc'` : good for peformance if you write a lot of files and these files are not going to be overwritten by a more official rule. In the later case, not computing a checksum will prevent `lmake` from detecting a steady file and this may trigger the spurious execution of a depending job.

### 12.2 `gcc` does not generate dependencies on some generated .h files, how to handle this ?

When `gcc` starts, it looks at all its include directories listed through options such as `-I` (`-iquote`, `-isystem` and `-idirafter`). If a directory does not exist, it is removed from its internal list and when a file is included with `#include`, such directories are not tried. As a consequence, no dep to files within these directories are generated. To circumvent this adverse optimization done by `gcc`, all directories that lie in the repository must exist before `gcc` is started. An easy way to ensure that by creating a dependency to a marker file within each such directory and to create a very simple rule that generates such marker file.

An exemple is shown here :

class Mrkr(lmake.Rule) :
	prio   = -lmake.inf         # in case of clash with another rule teh other rule is perfect, as long as mrkr is not a directory
	target = '{\_\_dir\_\_}mrkr'
	cmd    = ''                 # target is open as its stdout, this is enough, we have nothing to put in the target
class Gcc(lmake.Rule) :
	targets = { 'OBJ' : '{File:.+}.o' }
	deps    = { 'SRC' : '{File:.+}.c' }
	cmd     = 'ldepend a/mrkr b/mrkr c/mrkr ; gcc -Ia -Ib -Ic -c -o $OBJ $SRC'

13 Glossary
-----------

MRO

Method Research Order, the inheritance chain from the current class to its most basic base, usually `object`. Python computes the MRO in such a way as to enforce the diamong rule.

diamond rule

A feature of Python that allows the following behavior :

*   A class `D` inherits from `B` and `C` in that order.
*   Both `B` and `C` inherit from a class `A`.
*   A method `m` is defined on `A` and `C` but not on `B`.
*   Then if `m` is called from an instance of `D`, `C.m` will be called and not `B.m` (which turns out to be `A.m`).

This feature is a central point that makes Python multiple inheritance easy to use and enables the class hierarchy shopping list style.

ETA

Estimated Time of Arrival. This is the date at which a given event (such as a job being completed) is estimated to occur.

ETE

Estimated Time En route. This is the remaining time necessary to complete a task.

LRU

Least Recently Used. A classical cache replacement policy where the entry that was least recently used is discarded when a new one is allocated.

* * *
