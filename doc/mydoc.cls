\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{mydoc}[2020/08/03 v0.1 Document Class]
\LoadClass[zihao=-4,fontset=none,a4paper]{ctexart}

\RequirePackage{graphicx}
\RequirePackage{geometry}
% \RequirePackage{fancyhdr}
\RequirePackage[newfloat]{minted}
\RequirePackage{newfloat}
\RequirePackage{tcolorbox}
\RequirePackage{hyperref}
\RequirePackage{titlesec}
\RequirePackage{enumitem}
\RequirePackage{unicode-math}
\RequirePackage{tikz}
\RequirePackage{awesomebox}
\RequirePackage{xurl} % url 自动换行

\usetikzlibrary{calc}

% hyperref 配置
\AtBeginDocument{
  \hypersetup{
    hidelinks,
    pdftitle={\@title},
    pdfauthor={\@author}
  }
}

\setCJKmainfont[%
  BoldFont=Source Han Serif CN Bold,%
  ItalicFont=FZKai-Z03S,%
  BoldItalicFont=FZCuKaiS-R-GB]{Source Han Serif CN}
\setCJKsansfont[%
  BoldFont=Source Han Sans CN Bold,%
  ItalicFont=FZKai-Z03S,%
  BoldItalicFont=FZCuKaiS-R-GB]{Source Han Sans CN}
\setCJKmonofont[%
  BoldFont=Source Han Sans CN Bold,%
  ItalicFont=FZKai-Z03S,%
  BoldItalicFont=FZCuKaiS-R-GB]{Source Han Sans CN}
\newCJKfontfamily\kai[BoldFont=FZCuKaiS-R-GB]{FZKai-Z03S}
\newCJKfontfamily\xiaobiaosong[BoldFont=FZXiaoBiaoSong-B05S]{FZXiaoBiaoSong-B05S}
\newCJKfontfamily\dabiaosong[BoldFont=FZDaBiaoSong-B06S]{FZDaBiaoSong-B06S}
% \setmainfont{TeX Gyre Termes}
% \setsansfont{Gotham Rounded Medium}
\setmainfont{TeX Gyre Pagella} % or URW Palladio L
\setsansfont{TeX Gyre Adventor}
\setmathfont{TeX Gyre Pagella Math}

% 页边距设置
\geometry{left=3cm,right=3cm,bottom=3cm,top=3cm}

% ctex 配置
\ctexset{%
  punct      = quanjiao,%
  space      = auto,%
  today      = small,%
  autoindent = true%
}

\newcommand{\art}[1]{\textit{#1}}

\setlist{nosep,leftmargin=\parindent,listparindent=\parindent}
\setlist[enumerate,2]{label=\alph*.,font=\textup,labelindent=2\parindent,leftmargin=\parindent,itemindent=*}

\tcbuselibrary{minted,skins,breakable}
\newtcblisting{codes}[2][]{%
  listing engine=minted,%
  breakable,%
  colback=gray!5,%
  colframe=gray!50!black,%
  listing only,%
  minted style=friendly,%
  minted language=#2,
  minted options={%
    linenos=true,%
    numbersep=2.5mm,%
    breaklines=true,%
    fontsize=\small,%
    escapeinside=\@\@,%
    mathescape=true,%
  },%
  left=5mm,%
  top=0mm,%
  bottom=0mm,%
  enhanced,%
  overlay={\begin{tcbclipinterior}\fill[gray!25] (frame.south west)
           rectangle ([xshift=5mm]frame.north west);\end{tcbclipinterior}},%
  #1%
}

\newcommand{\code}[1]{{\texttt{#1}}}
\newcommand{\ccode}[1]{\mintinline{C}{#1}}
\newcommand{\file}[1]{{\texttt{#1}}}

\definecolor{crimson}{rgb}{0.86, 0.08, 0.24}
% https://tex.stackexchange.com/a/340935
\newtcolorbox{mybox}[4][]{%
  enhanced,
  breakable,
  colback=#2!5,
  colframe=#2,
  attach boxed title to top left={yshift=-2pt}, title={\hspace*{.5mm} #3},
  boxrule=0pt,
  size=title,
  top=1.5mm,
  bottom=1.5mm,
  boxed title style={%
    sharp corners,
    rounded corners=northwest,
    colback=tcbcolframe,
    top=3pt,
    bottom=3pt,
    boxrule=0pt
  },
  sharp corners=north,
  overlay unbroken={%
    \path[fill=tcbcolframe]
      (title.south east)
      to[out=0, in=180] ($(title.east)+(1cm,0.25cm)$)--
      ($(title.east-|frame.north east)+(0,0.25cm)$) [rounded corners=\kvtcb@arc] |- ([xshift=-1pt]title.north east);
    \path[fill=tcbcolback]
      (title.south east)
      to[out=0, in=180] ($(title.east)+(1cm,0.25cm)$)--
      ($(title.east-|frame.north east)+(0,0.25cm)$) |- (title.south east);
    \path[draw=tcbcolframe,line width=0.4mm]
      ([yshift=-3pt,xshift=-0.2mm]frame.north east) --
      ($(title.east-|frame.north east)+(-0.2mm,0.26cm)$);
    \foreach \x in {10,20,...,100}
      \path[opacity=\x*0.01,line width=1.1pt,draw=gray!\x]
        ($([xshift=.4mm]title.south west)+(0,\x/100pt-1.1pt)$) --
        ($(title.south east)+(0,\x/100pt-1.1pt)$) to[out=0, in=180]
        ($(title.east)+(1cm,0.25cm+\x/100pt-1.1pt)$) --
        ($(title.east-|frame.north east)+(-0.4mm,0.25cm+\x/100pt-1.1pt)$);
    \path[draw=white,line width=1.1pt]
      ([xshift=.4mm]title.south west) -- (title.south east) to[out=0, in=180] ($(title.east)+(1cm,0.25cm)$)--
      ($(title.east-|frame.north east)+(-0.4mm,0.25cm)$);
    \node[anchor=south east,minimum height=20pt,inner xsep=5pt] at ([yshift=-5pt]frame.north east)
      {\color{tcbcolframe} #4 \hspace*{1.5mm}};
    \draw[line width=.4mm, rounded corners=\kvtcb@arc,
      tcbcolframe]
      (title.north west) rectangle
      (frame.south east);
  },
  overlay first={%
    \path[fill=tcbcolframe]
      (title.south east)
      to[out=0, in=180] ($(title.east)+(1cm,0.25cm)$)--
      ($(title.east-|frame.north east)+(0,0.25cm)$) [rounded corners=\kvtcb@arc] |- ([xshift=-1pt]title.north east);
    \path[fill=tcbcolback]
      (title.south east)
      to[out=0, in=180] ($(title.east)+(1cm,0.25cm)$)--
      ($(title.east-|frame.north east)+(0,0.25cm)$) |- (title.south east);
    \path[draw=tcbcolframe,line width=0.4mm]
      ([yshift=-3pt,xshift=-0.2mm]frame.north east) --
      ($(title.east-|frame.north east)+(-0.2mm,0.26cm)$);
    \foreach \x in {10,20,...,100}
      \path[opacity=\x*0.01,line width=1.1pt,draw=gray!\x]
        ($([xshift=.4mm]title.south west)+(0,\x/100pt-1.1pt)$) --
        ($(title.south east)+(0,\x/100pt-1.1pt)$) to[out=0, in=180]
        ($(title.east)+(1cm,0.25cm+\x/100pt-1.1pt)$) --
        ($(title.east-|frame.north east)+(-0.4mm,0.25cm+\x/100pt-1.1pt)$);
    \path[draw=white,line width=1.1pt]
      ([xshift=.4mm]title.south west) -- (title.south east) to[out=0, in=180] ($(title.east)+(1cm,0.25cm)$)--
      ($(title.east-|frame.north east)+(-0.4mm,0.25cm)$);
    \node[anchor=south east,minimum height=20pt,inner xsep=5pt] at ([yshift=-5pt]frame.north east)
      {\color{tcbcolframe} #4 \hspace*{1.5mm}};
    \draw[line width=.4mm, rounded corners=\kvtcb@arc,
      tcbcolframe]
      (frame.south west) |- (title.north) -|
      (frame.south east);
  },
  overlay middle={%
    \draw[line width=.4mm, tcbcolframe]
    (frame.north west)--(frame.south west)
    (frame.north east)--(frame.south east);
  },
  overlay last={%
    \draw[line width=.4mm, rounded corners=\kvtcb@arc,
      tcbcolframe]
      (frame.north west) |- (frame.south) -|
      (frame.north east);
  },
  #1
}

\definecolor{danger-col}{HTML}{dc3545}
\definecolor{info-col}{HTML}{17a2b8}
\definecolor{warning-col}{HTML}{ffc107}
\definecolor{question-col}{HTML}{a2de96}

\newcommand{\bugbox}[2]{%
  \begin{mybox}{crimson}{\faBug\ \faBug\ \faBug}{\file{#1}}%
    #2%
  \end{mybox}%
}

\newenvironment{info}{%
    \begin{awesomeblock}[info-col]{2pt}%
      {\normalsize\faInfoCircle}{info-col!70!black}%
  }{%
    \end{awesomeblock}%
}

\newenvironment{warning}{%
    \begin{awesomeblock}[warning-col]{2pt}%
      {\normalsize\faExclamationTriangle}{warning-col!70!black}%
  }{%
    \end{awesomeblock}%
}

\newenvironment{important}{%
    \begin{awesomeblock}[danger-col]{2pt}%
      {\normalsize\faExclamationCircle}{danger-col!70!black}%
  }{%
    \end{awesomeblock}%
}

\newenvironment{question}{%
    \begin{awesomeblock}[question-col]{2pt}%
      {\normalsize\faQuestionCircle}{question-col!70!black}%
  }{%
    \end{awesomeblock}%
}

\AtBeginDocument{%
  \setlength{\aweboxleftmargin}{2em}%
  \setlength{\aweboxvskip}{5mm}%
  \setlength{\aweboxsignraise}{0mm}%
}