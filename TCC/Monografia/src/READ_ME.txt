Testado para as seguintes configurações: 
________________________________________
Sistema Operacional - Windows 10 x64

Distribuição Tex - MiKTeX 2.9.7200 64-bit (MiKTeX Package Manager 2.9.7043)
Compilador - texstudio 2.12.16
Versão do pacote biber - biber-windows-x64 2.13

Configurações do texstudio
Comandos:
- pdfLatex: pdflatex.exe -synctex=1 -interaction=nonstopmode %.tex
- biber: biber.exe %
Compilação:
- Default Bibliography Tool: Biber
- pdflatex | biber | pdflatex | pdflatex
________________________________________
Sistema Operacional - Ubuntu 18.04.3

Distribuição Tex - TexLive 2019 (https://www.tug.org/texlive/acquire-netinstall.html)
Coompilador - texstudio 2.12.16 (Install Using Ubuntu PPA https://www.ubuntupit.com/how-to-install-texstudio-a-latex-editor-in-ubuntu-linux/)
Versão do pacote biber - biber-linux-X86_64 2.13

Configurações do texstudio
Comandos:
- pdfLatex: pdflatex -synctex=1 -interaction=nonstopmode %.tex
- biber: biber %
Compilação: 
- Default Bibliography Tool: Biber
- pdflatex | biber | pdflatex | pdflatex
________________________________________
Sistema Operacional - Ubuntu 16.04

Distribuição Tex - TexLive 2018
Compilador - texstudio 2.16.6
Versão do pacote biber - biber 2.11
(disponíveis em https://launchpad.net/~jonathonf/+archive/ubuntu/texlive)

Configurações do texstudio
Comandos:
- pdfLatex: pdflatex -synctex=1 -interaction=nonstopmode %.tex
- biber: biber %
Compilação:
- Default Bibliography Tool: Biber
- pdflatex | biber | pdflatex | pdflatex
________________________________________
Obs.:
Lembrar de atualizar os pacotes para a última versão.
No MiKTeX, por exemplo, pode ser feito pelo MiKTeX Console ou via linha de comando: mpm --verbose --update

Para quem estiver com problemas com a instalação ou configuração, uma alternativa é utilizar um compilador online como, por exemplo, o Overleaf.