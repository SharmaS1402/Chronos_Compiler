$$
\begin{align}
[\text{prog}] &\to [\text{stmt}]^*\\
[\text{stmt}] &\to 
\begin{cases}
    exit({[expr]}); \\
    let \space \text{ident} = [\text{expr}];
\end{cases}
\\
[\text{expr}] &\to 
\begin{cases}
    \text{int\_lit} \\
    \text{ident}
\end{cases}
\end{align}
$$