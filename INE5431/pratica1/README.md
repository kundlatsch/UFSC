# Prática 1
(44.1 KHz, 16 bits/amostra, mono). Em seguida, faça as conversões abaixo acumuladamente usando o software Audacity e responda as questões:

**A) Em relação ao arquivo, responda i) qual é o tamanho teórico do áudio (parte de dados); ii) observando as propriedades do arquivo em seu sistema operacional (no linux execute du -s -B1 praticaI.wav) , e indique quais motivos o tamanho do arquivo em disco é maior que o tamanho teórico; iii) qual o tamanho deste disco se  o seu HD fosse formatado para um tamanho de bloco de bloco de 1024 bytes?**

**i)**

Tamanho = taxa de amostragem (frequência) \* amostras/s \* número de canais \* duração \/ 8

Taxa de amostragem: 44,1 KHz

Amostras/s: 16bps

número de canais: 1 (mono)

duração 10 s

Tamanho teórico do áudio: (1 \* 44100 \*16 \* 10) \/ 8 \/ 1024 = 861,328125 KiB    


(1*44100*16*10/8)/1024 = 861,328125 (882.000)

Tamanho total do arquivo: 861,4 KiB (882.044)

**ii)**

Metadados, tamanho do bloco do disco

**iii)**

ceil(Tamanho do arquivo \/ tamanho do bloco) \* tamanho do bloco
ceil(861,328125 \/ 1024) \* 1024 = 862KiB

**B) Baixe taxa de amostragem para 8000Hz (sem alteração do número de bits por amostra), e responda: i) o tamanho teórico da mídia; ii) qual a frequência do maior componente frequência teórico para o novo formato do áudio; e iii) explique porque ocorreu os períodos de silêncio no áudio convertido.**

**i)**

Tamanho teórico do áudio: 

Taxa de amostragem: 8 KHz

Amostras/s: 16bps

número de canais: 1 (mono)

duração 10 s

Tamanho teórico do áudio: (1 \* 8000 \*16 \* 10) \/ 8 \/ 1024 = 156,25 KiB    

**ii)**

Maior componente de frequência teórico: 4KHz

**iii)**

Devido ao limite de frequência referente à taxa de amostragem. As frequências que estão acima da metade da frequência de amostragem não são ouvidas visto que segundo Nyquist a maior frequência dita que a taxa de amostragem deve ser no mínimo o dobro dela.

**C) Após reduzir a taxa de amostragem em B), reduza também o número de bits por amostra pela metade (8 bits por amostra).  Abra o arquivo salvo e indique em seu relatório:  i) o tamanho teórico da mídia; ii) qual a frequência do maior componente frequência teórico para o novo formato do áudio; iii) explique o efeito na qualidade do áudio gerada pela redução do número de bits por amostra (ouça os períodos de silêncio em B e compare com C).**

**i)**

Taxa de amostragem: 8 KHz

Amostras/s: 8bps

número de canais: 1 (mono)

duração 10 s

Tamanho teórico do áudio: (1 \* 8000 \*8 \* 10) \/ 8 \/ 1024 = 78,125 KiB    


**ii)**

O maior componente de frequência teórico não foi alterado, permanecendo em 4KHz

**iii)**

Reduzindo a quantidade de bits por amostra é adicionado ruído de quantização. Esse ruído ficou mais perceptível nos períodos de silêncio.