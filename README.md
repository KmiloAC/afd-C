# <div style='background: #fa0'>Implementación de un Autómata Finito Determinista en C </div>

Integrantes: <br>
-Camilo Andres López Contreras <br>
-Miguel Fernández De La Cruz <br>
-Kevin Nicolai Rodríguez García <br>

Recordemos que un autómata finito determinista está definido por una 5-tupla $(Q, \sum, \delta, q_0, F)$ donde:
- $Q$ es un conjunto finito de estados.
- $\sum$ es un conjunto finito de símbolos (alfabeto).
- $\delta$ es una función de transición $\delta : Q \times \sum  \rightarrow Q$.
- $q_0$ es el estado inicial $(q_0 \in Q)$.
- $F$ es el conjunto de estados de aceptación $(F \subseteq Q)$.

Para implementar el automata, leemos un archivo txt que contiene la configuración del mismo, con la siguiente estructura:

1. Primera línea: conjunto de estados ($Q$), separados por comas.
2. Segunda línea: alfabeto ($\sum$), separado por comas.
3. Tercera línea: estado inicial ($q_0$).
4. Cuarta línea: estados de aceptación ($F$), separados por comas.
5. Las siguientes líneas: transiciones en el formato `estado_actual,símbolo,estado_siguiente`.

Por ejemplo, el archivo de configuración:

```
q0,q1,q2
a,b
q0
q2
q0,a,q1
q1,b,q2
q2,a,q0
q2,b,q1
```

produce el automata finito determinista que se muestra en la siguiente imagen

![Grafo TikZ](afd.svg)

## Aceptación de una cadena

Ahora, para decidir si aceptar o rechazar una cadena, construimos primero la matriz de transición teniendo en cuenta el conjunto de estados y el lfabeto, y considerando aquellas posiciones donde no haya una transición definida como una posicion vacía. Por ejemplo, con la configuración dada, visualizaríamos las trancisiones como

$$
\begin{array}{c|cc}
   & a & b \\
\hline
q_0 & q_1 & \emptyset \\
q_1 & \emptyset & q_2 \\
q_2 & q_0 & q_1 \\
\end{array}
$$

es decir, la matriz de transiciones está dada por 

$$
T = \begin{bmatrix}
q_1 & \emptyset \\
\emptyset & q_2 \\
q_0 & q_1
\end{bmatrix}.
$$

Esta matriz de transición nos permite implementar la funcion de transición $\delta : Q\times \sum \rightarrow Q$, a través de los indices de los estados y los símbolos, obteniendo el valor del estado siguiente dado un estado y un simbolo. De esta forma, podemos evaluar la cadena digitada caracter a caracter para decidir si esta debe ser aceptada o rechazada.

## Manejo de errores

1. Si el archivo de configuración tiene menos de 5 lineas, se muestra un mensaje de error.
2. Si el estado inicial no pertenece a los estados, se pide que se re ingrese un estado inicial correcto.
3. Alguno de los estados de acptación no pertenece a los estados, es eliminado.
4. Si alguna de las transiciones está mal definida, se muestra un mensaje de error.
