# Introduction-to-Reverse-Engineering
This repo aims to be a material on information about cybersecurity, especifically, **reverse engineering**. Not only it will present to the reader the concepts related to vulnerabilities and their exploits, but also contain **practical examples** to make the learning process more dynamic. This material is being written by myself: currently I'm a  student at Universidade de São Paulo (USP), under a Computer Engineering degree. The objective is to write down all the knowledge taht I've acquired on reverse engineering during graduation, in the most **comprehensive** way, so I can keep visual track about what I've learned and anyone that feels interested about this subject may read and learn it, just as I did. 

Although it's impossible to truly understand the conditions which makes programs vulnerable without getting deeper into processor and memory concepts, my goal is to make the explanation seems **as natural as possible**. Essentially, I'll start from initial exploitation techniques, such as Buffer Overflow and Shellcode, but as I gradually advance my studies and gather more knowledge, I'll **update** this material with new information.

## Structure
For each vulnerability covered in this repo, there will be a corresponding directory with the name of that vulnerability, for example: the complete material, with explanation and examples, about Buffer Overflow will be in the directory named "Buffer Overflow". Inside each directory, there will be a theory-introduction file, which will contain the theoretical explanation about that directory's vulnerability, and another directory, "Examples", which will have the practical examples. So under `buffer-overflow` directory, `introduction.md` file contains the explanation about the Buffer Overflow subject and `examples` directory, the examples.

## General Requirements
* C programmig language;
  * Reverse engineering has an intrinsic relation with memory, and so in order to create examples that manage memory usage we'll need to use C.
* x86_64;
  * This will be the architecture used for explanation in general, so one needs to know, at least superficially, about the processor instructions and registers of this particular architecture.
* Linux terminal;
  * Basic commands and VIM.
* Python3.
  * Initially, python will be presented with the use of `pwn` library, but the basic syntax won't be explained.
