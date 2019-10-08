### University of Aveiro (UA) thesis template
[https://www.ua.pt/](https://www.ua.pt/)


### Files/directories description

- [fig/](fig/) - Figures.

- [original/](original/) - It contains the original UA thesis style
(LaTeX) file which this template is derived from. The original style
file was developed by professor Tomás Oliveira e Silva following the
rules set by the University of Aveiro. It is available from his homepage
[http://sweet.ua.pt/tos/](http://sweet.ua.pt/tos/). More information and
several examples can be found in his LaTeX document.

- [tex/contents/](tex/contents/) - LaTeX files with contents (e.g.
chapters).

- [tex/options/languages/](tex/options/languages/) - Support for English
and Portuguese languages.

- [tex/options/packages.tex](tex/options/packages.tex) - List of used
packages.

- [tex/options/posBegin.tex](tex/options/posBegin.tex) - Commands after
begin document.

- [tex/options/preBegin.tex](tex/options/preBegin.tex) - Commands before
begin document.

- [COPYING](COPYING) - GNU GPLv3 license.

- [main.tex](main.tex) - The main LaTeX file (to be compiled).

- [README.md](README.md) - This file. It gives information about this
template.

- [refs.bib](refs.bib) - BibTeX references.

- [uaThesis.sty](uaThesis.sty) - UA thesis style file (modified).


### Usage

```
$ latexmk -pdf main.tex
```


### Authors

This work is based on the original LaTeX template developed by professor
Tomás Oliveira e Silva, which can be accessed on his homepage
[http://sweet.ua.pt/tos/](http://sweet.ua.pt/tos/). For the sake of
preservation, a local copy of the original work is at
[original/ua_thesis.tgz](original/ua_thesis.tgz).

I did several modifications in the style file which is shared in this
repository
[https://github.com/ruiantunes/ua-thesis-template](https://github.com/ruiantunes/ua-thesis-template).

To the best of my knowledge, other similar works are being or were
developed by:
- João Paulo Barraca et al.
[http://code.ua.pt/projects/latex-ua/repository](http://code.ua.pt/projects/latex-ua/repository).
- Fábio Maia and Ricardo Jesus
[https://github.com/fabiomaia/ua-thesis-template](https://github.com/fabiomaia/ua-thesis-template).
