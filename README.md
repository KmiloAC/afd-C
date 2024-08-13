# <div style='background: #fa0'>Implementación de un Autómata Finito Determinista en C </div>

Integrantes: <br>
-Camilo Andres López Contreras <br>
-Miguel Fernández De La Cruz <br>
-Kevin Nicolai Rodríguez García <br>

Donde:
Estados: q0, q1, q2 <br>
Alfabeto: a,b <br>
Estado inicial: q0 <br>
Estado/s de aceptación: q2 <br>

Este programa se encarga de leer un archivo que debe de estar previamente configurado, además de que se haran verificaciones tales que si el estado inicial se encuentra dentro de nuestros estados<br>
## Ejemplo de archivo de configuración

```plaintext
q0,q1,q2
a,b
q0
q2
q0,a,q1
q1,b,q2
q2,a,q0
q2,b,q1
```
Para luego leer una cadena de caracteres donde verificara que cada una de las letras o números se encuentre dentro del alfabeto, como tambien nos dira si la cadena es aceptada o rechazada dependiendo de factores como que debe de llegar a un estado de aceptación, seguir una ruta. <br>


 ```plaintext
Ejemplo de cadena: ab
Esta cadena debe de ser aceptada
```

![Grafo TikZ](afd.svg)
