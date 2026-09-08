# ⚙️ Funcionamiento de ATOM ArchBot V3

Este documento describe de forma detallada el funcionamiento de **ATOM ArchBot V3**, un sistema robótico desarrollado para apoyar el proceso de harneado arqueológico mediante automatización, sensores de distancia, visión artificial, control electrónico y sistemas de seguridad.

El controlador principal del robot es el **MATRIX Mini R4**, encargado de coordinar los sensores, actuadores, interfaces de usuario y comunicaciones del sistema.

ATOM ArchBot no busca reemplazar el trabajo de un arqueólogo.

Su función es actuar como una herramienta tecnológica de apoyo capaz de:

- Automatizar el movimiento del harnero.
- Medir aproximadamente la cantidad de material presente.
- Adaptar el movimiento del harnero.
- Detectar visualmente posibles objetos de interés.
- Detener automáticamente el proceso cuando sea necesario.
- Controlar la entrada y salida de la bandeja.
- Evitar determinadas acciones inseguras.
- Informar constantemente al usuario sobre el estado del sistema.
- Generar alertas visuales y sonoras.
- Comunicar eventos importantes al sistema ATOM.IA.
- Facilitar pruebas, diagnóstico y demostraciones del prototipo.

> [!IMPORTANT]
> ATOM ArchBot identifica **posibles hallazgos**.
>
> La clasificación definitiva de un objeto como pieza arqueológica debe ser realizada por una persona capacitada.

---

# 🏺 Objetivo general

El harneado es una etapa importante durante determinados trabajos arqueológicos.

Normalmente el material se deposita sobre una malla y posteriormente se mueve para separar la tierra de elementos de mayor tamaño.

ATOM ArchBot busca apoyar este proceso utilizando robótica.

La idea principal del sistema es:

```text
MEDIR
   ↓
ANALIZAR
   ↓
ADAPTAR
   ↓
HARNEAR
   ↓
OBSERVAR
   ↓
DETECTAR
   ↓
PROTEGER
```

El robot intenta que el proceso sea más:

```text
Controlado
Adaptable
Repetible
Observable
Seguro
Interactivo
```

---

# 🧠 Cerebro del sistema

El controlador principal es el:

## MATRIX Mini R4

El MATRIX Mini R4 actúa como el centro de coordinación de ATOM ArchBot.

Recibe información desde:

```text
Sensores ToF
Pulsadores
Final de carrera
M-Vision Cam
Botones integrados
```

Y controla:

```text
Servomotores
Motores DC
LCD 16×2
LCD 16×4
OLED
Buzzer
RGB izquierdo
RGB derecho
```

De forma simplificada:

```text
                    ENTRADAS
                       │
        ┌──────────────┼──────────────┐
        │              │              │
      ToF         M-Vision       Pulsadores
        │              │              │
        └──────────────┼──────────────┘
                       ▼
              ┌─────────────────┐
              │ MATRIX MINI R4  │
              └────────┬────────┘
                       │
        ┌──────────────┼──────────────┐
        │              │              │
      Servos         Motores       Pantallas
        │              │              │
        ├──────────────┼──────────────┤
        │              │              │
       RGB           Buzzer        ATOM.IA
```

---

# 🔄 Funcionamiento general

El funcionamiento normal puede resumirse mediante esta secuencia:

```text
┌─────────────────────┐
│      ENCENDIDO      │
└──────────┬──────────┘
           ▼
┌─────────────────────┐
│   INICIALIZACIÓN    │
└──────────┬──────────┘
           ▼
┌─────────────────────┐
│ COMPROBACIÓN SISTEMA│
└──────────┬──────────┘
           ▼
       ¿TODO OK?
        /      \
      NO        SÍ
      │          │
      ▼          ▼
┌──────────┐ ┌──────────┐
│  ERROR   │ │  LISTO   │
└──────────┘ └────┬─────┘
                  │
            Pulsador 1
                  │
                  ▼
          ┌──────────────┐
          │  HARNEANDO   │
          └──────┬───────┘
                 │
        ┌────────┴────────┐
        ▼                 ▼
   Sensores ToF       M-Vision
        │                 │
        ▼                 ▼
 Medir material       Analizar
        │                 │
        ▼                 ▼
 Ajustar velocidad   Buscar objeto
        │                 │
        └────────┬────────┘
                 ▼
          ¿HAY HALLAZGO?
             /      \
           SÍ        NO
           │          │
           ▼          └───────┐
     ┌────────────┐           │
     │ HALLAZGO   │           │
     └─────┬──────┘           │
           │                  │
           ▼                  │
      Detener harnero         │
      Alertar usuario         │
      Informar ATOM.IA        │
           │                  │
           └──────────────────┘
```

---

# 🚀 1. Encendido del robot

El funcionamiento comienza cuando se activa el interruptor general.

La energía sigue aproximadamente la siguiente ruta:

```text
Pack de baterías
       ↓
     BMS 2S
       ↓
Interruptor general
       ↓
 Jack DC
       ↓
MATRIX Mini R4
```

Una vez energizado el controlador, comienza automáticamente la secuencia de inicio.

---

# ✨ 2. Secuencia visual de inicio

El robot utiliza sus diferentes interfaces para indicar que ha comenzado correctamente.

Durante los primeros segundos pueden utilizarse:

```text
OLED
LCD 16×2
LCD 16×4
RGB
Buzzer
```

La OLED puede comenzar mostrando:

```text
ATOM-ArchBot
```

Posteriormente puede aparecer el logo del equipo:

```text
LOS HARNERITOS
```

Mientras tanto el LCD principal puede mostrar:

```text
Hola mundo,
soy ATOM-ArchBot
```

y posteriormente:

```text
Conectando a
ATOM.IA
```

Si la conexión correspondiente se establece:

```text
ATOM.IA
Conectado
```

Finalmente el robot pasa a su pantalla de espera.

---

# 🧠 3. Inicialización de componentes

Antes de mover cualquier mecanismo, el programa configura los diferentes componentes.

La inicialización incluye:

```text
MATRIX Mini R4
│
├── M-Vision Cam
│
├── ToF izquierdo
│
├── ToF derecho
│
├── Servo izquierdo
│
├── Servo derecho
│
├── Motor DC izquierdo
│
├── Motor DC derecho
│
├── Pulsadores
│
├── Final de carrera
│
├── LCD 16×2
│
├── LCD 16×4
│
├── OLED
│
├── RGB izquierdo
│
├── RGB derecho
│
└── Buzzer
```

El objetivo es que todos los sistemas comiencen desde un estado conocido.

---

# 🦾 4. Posición inicial de los servomotores

Los dos servomotores encargados del movimiento del harnero deben comenzar desde una posición de referencia.

No es obligatorio que esta posición corresponda exactamente a `0°`.

La posición inicial puede ser calibrada posteriormente según la geometría definitiva del mecanismo.

Conceptualmente:

```text
ENCENDIDO
   ↓
Mover servos
   ↓
POSICIÓN INICIAL
   ↓
Detener movimiento
```

Esto evita que un servo permanezca en una posición desconocida después de iniciar el robot.

También permite que después de determinadas acciones los servos puedan regresar nuevamente a una posición segura.

---

# 🩺 5. Comprobación del sistema

ATOM ArchBot puede realizar comprobaciones antes de permitir el movimiento principal.

Estas comprobaciones buscan detectar problemas evidentes.

Por ejemplo:

| Sistema | Comprobación |
|---|---|
| M-Vision | Comunicación disponible |
| ToF izquierdo | Lectura válida |
| ToF derecho | Lectura válida |
| LCD | Inicialización |
| RGB | Inicialización |
| Final de carrera | Estado lógico |
| Pulsadores | Lectura de entradas |
| ATOM.IA | Estado de comunicación |
| Sistema general | Estado correcto |

Si los sistemas necesarios están disponibles:

```text
COMPROBACIÓN
     ↓
   TODO OK
     ↓
    LISTO
```

Si existe un problema importante:

```text
COMPROBACIÓN
     ↓
    ERROR
     ↓
BLOQUEAR ACCIÓN
```

---

# ⚠️ 6. Estado ERROR

El estado `ERROR` busca evitar que el robot realice una acción cuando existe una condición que podría producir un comportamiento incorrecto.

Al entrar en error:

```text
Servos        → detener
Motores DC    → detener
RGB           → indicar error
Buzzer        → alerta
Pantallas     → mostrar causa
```

Ejemplo:

```text
ERROR
SIN BANDEJA
```

o:

```text
ERROR
SENSOR
```

El objetivo es indicar no solamente que existe un problema, sino también ayudar a localizarlo.

---

# ✅ 7. Estado LISTO

Cuando la inicialización finaliza, ATOM ArchBot entra en:

```text
LISTO
```

En este estado:

```text
Servos       → detenidos
Motores DC   → detenidos
Sensores     → disponibles
M-Vision     → preparada
Pantallas    → activas
Pulsadores   → activos
RGB          → estado de espera
```

La pantalla principal puede indicar:

```text
Comenzar a
harnear
```

ATOM permanece esperando la orden del usuario.

---

# 🔘 8. Controles físicos

ATOM ArchBot utiliza cuatro pulsadores externos.

| Pulsador | Función principal |
|---|---|
| Pulsador 1 | Iniciar harneado |
| Pulsador 2 | Pausar / continuar |
| Pulsador 3 | Sacar bandeja |
| Pulsador 4 | Insertar bandeja |

Al detectar correctamente una pulsación, el buzzer puede reproducir un sonido corto de confirmación.

Esto permite al operador saber que el comando fue recibido.

---

# ⚙️ 9. Inicio del harneado

El proceso comienza mediante:

```text
PULSADOR 1
```

La secuencia es:

```text
Pulsador 1
    ↓
MATRIX recibe comando
    ↓
Comprobar condiciones
    ↓
¿Es seguro comenzar?
    ↓
    SÍ
    ↓
HARNEANDO
```

Antes de comenzar se pueden revisar condiciones como:

```text
Bandeja presente
Sensores disponibles
Sin error crítico
Sistema preparado
```

Si alguna condición necesaria no se cumple, el movimiento no comienza.

---

# 🛡️ 10. Protección: no harnear sin bandeja

Una de las reglas de seguridad de ATOM es evitar iniciar el proceso si la bandeja no se encuentra correctamente dentro del sistema.

La lógica es:

```text
Usuario pulsa INICIAR
        ↓
¿Bandeja insertada?
     /       \
   NO         SÍ
   │           │
   ▼           ▼
BLOQUEAR    HARNEAR
```

Esto ayuda a evitar que el material pase a través del harnero sin una bandeja preparada para recibirlo.

---

# 🦾 11. Movimiento del harnero

El movimiento se genera mediante dos servomotores.

```text
Servo izquierdo
       ↘
        HARNERO
       ↗
Servo derecho
```

Los servos trabajan de forma coordinada.

El objetivo no es simplemente mover ambos servos de forma independiente, sino producir un movimiento estable y repetitivo en el harnero.

Los movimientos pueden alternarse para generar una oscilación.

Conceptualmente:

```text
Servo IZQ. → movimiento A
Servo DER. → movimiento B

          ↓

Servo IZQ. → movimiento B
Servo DER. → movimiento A
```

Esta alternancia genera el movimiento de harneado.

---

# ⚡ 12. Niveles de velocidad

ATOM puede utilizar diferentes niveles de movimiento.

Por ejemplo:

```text
Nivel 0 → detenido
Nivel 1 → lento
Nivel 2 → medio
Nivel 3 → rápido
```

El nivel seleccionado depende principalmente de la cantidad aproximada de material detectada.

La velocidad no tiene que permanecer fija durante todo el proceso.

---

# 📏 13. Sensores ToF

ATOM ArchBot utiliza dos sensores láser de distancia tipo ToF.

```text
ToF izquierdo          ToF derecho
      ↓                     ↓
      └────── HARNERO ──────┘
```

Cada sensor mide la distancia existente entre él y la superficie que tiene debajo.

Si existe bastante material:

```text
Sensor
  ↓
████████ tierra
```

la distancia medida será menor.

Si existe poco material:

```text
Sensor
  ↓


██ poca tierra
```

la distancia será mayor.

De esta forma es posible estimar aproximadamente la cantidad de material restante.

---

# 📐 14. Uso combinado de los dos sensores

No se depende únicamente de una medición.

El robot dispone de:

```text
Distancia izquierda
+
Distancia derecha
```

Esto ayuda a conocer mejor la distribución del material.

Por ejemplo:

```text
IZQ.: mucha tierra
DER.: poca tierra
```

indicaría una distribución irregular.

Mientras que:

```text
IZQ.: mucha tierra
DER.: mucha tierra
```

indicaría una mayor cantidad general de material.

Los valores exactos deben calibrarse experimentalmente.

---

# 🧠 15. Clasificación de cantidad de tierra

A partir de las mediciones se puede establecer un estado aproximado:

```text
SIN TIERRA
BAJA
MEDIA
ALTA
```

Esta información puede mostrarse directamente en el LCD 16×4.

Ejemplo:

```text
Tierra: Alta
```

La clasificación no pretende medir masa en gramos.

Los ToF miden **distancia**, y el software interpreta estas mediciones para estimar el nivel de material.

---

# ⚡ 16. Control adaptativo

Uno de los elementos principales de ATOM ArchBot es el control adaptativo del harneado.

La lógica general es:

```text
MÁS TIERRA
    ↓
Mayor intensidad
del movimiento
```

y:

```text
MENOS TIERRA
    ↓
Movimiento más
controlado
```

Esto permite evitar mantener continuamente el sistema a máxima velocidad.

La idea es:

```text
ALTA  → rápido
MEDIA → medio
BAJA  → lento
VACÍO → detener / finalizar
```

Los parámetros reales se ajustan durante las pruebas.

---

# 📊 17. Filtrado de las mediciones

Los sensores pueden presentar pequeñas variaciones entre una lectura y otra debido a:

```text
Movimiento de tierra
Vibraciones
Ángulo
Superficie irregular
Interferencia
```

Por esta razón, el programa puede utilizar varias mediciones antes de cambiar inmediatamente de estado.

La idea es evitar situaciones como:

```text
ALTA
MEDIA
ALTA
MEDIA
ALTA
```

en unos pocos milisegundos.

En cambio se busca obtener una transición estable.

---

# 📷 18. Visión artificial

Mientras ATOM está funcionando, la **M-Vision Cam** observa la zona del harnero.

La secuencia general es:

```text
HARNERO
   ↓
M-VISION
   ↓
Captura
   ↓
Procesamiento
   ↓
Clasificación
   ↓
Resultado
   ↓
MATRIX Mini R4
```

La cámara busca posibles objetos previamente configurados en el sistema.

---

# 👁️ 19. Qué analiza la visión

Dependiendo del modelo entrenado y de la configuración utilizada, la cámara puede analizar características visuales de los objetos.

El objetivo es diferenciar un objeto de interés de:

```text
Tierra
Sombras
Reflejos
Movimiento
Partículas
Elementos no relevantes
```

ATOM no debe detener el robot simplemente porque aparece un píxel o color diferente.

Por eso se utilizan filtros de confirmación.

---

# 🎯 20. Confianza de detección

La cámara puede entregar un porcentaje de confianza asociado al objeto detectado.

Ejemplo:

```text
Objeto: Punta
Confianza: 83 %
```

Una detección de baja confianza puede ser ignorada.

En la configuración base de pruebas se ha trabajado con un umbral cercano al:

```text
70 %
```

Esto significa que una detección debe superar el valor definido para poder avanzar hacia una posible confirmación.

> [!NOTE]
> Este porcentaje puede modificarse durante la calibración.

---

# 🎞️ 21. Confirmación durante varios fotogramas

Una única detección no necesariamente significa que exista un objeto real.

El sistema puede comprobar si el mismo objeto aparece durante varios fotogramas consecutivos.

```text
FRAME 1 → detectado
FRAME 2 → detectado
FRAME 3 → detectado
FRAME 4 → detectado
...
        ↓
CONFIRMACIÓN
```

Esto reduce alertas provocadas por:

```text
Sombras
Movimiento rápido
Reflejos
Tierra pasando
Errores puntuales
```

En las pruebas del sistema se ha trabajado con confirmación durante varios frames antes de generar un evento.

---

# 🔁 22. Evitar detecciones repetidas

Después de confirmar un objeto, el sistema debe evitar enviar la misma detección cientos de veces mientras el objeto continúa frente a la cámara.

Conceptualmente:

```text
OBJETO APARECE
      ↓
CONFIRMAR
      ↓
ENVIAR EVENTO
      ↓
BLOQUEAR REPETICIÓN
      ↓
OBJETO DESAPARECE
      ↓
REARMAR DETECCIÓN
```

Esta lógica permite registrar un hallazgo una sola vez hasta que el sistema se encuentre preparado para detectar nuevamente.

---

# 🏺 23. Posible hallazgo

Cuando la detección supera los criterios definidos, ATOM considera que existe un:

```text
POSIBLE HALLAZGO
```

No significa que el robot determine científicamente que el objeto sea arqueológico.

Significa que la visión ha encontrado un elemento suficientemente similar a uno de los objetos configurados.

---

# 🛑 24. Estado HALLAZGO

Cuando se confirma el posible hallazgo:

```text
M-Vision
   ↓
Objeto confirmado
   ↓
MATRIX Mini R4
   ↓
HALLAZGO
```

ATOM puede ejecutar varias acciones simultáneamente.

---

## Detener movimiento

Los servos dejan de harnear.

```text
Servo izquierdo → detener
Servo derecho   → detener
```

Posteriormente pueden regresar de forma controlada a su posición de referencia.

---

## Activar luces

Las barras RGB pueden utilizar una animación especial.

```text
RGB izquierdo
+
RGB derecho
      ↓
ALERTA VISUAL
```

Esto permite que incluso una persona que no esté mirando directamente las pantallas pueda notar el evento.

---

## Activar sonido

El buzzer reproduce un sonido diferente al de una pulsación normal.

El objetivo es diferenciar claramente:

```text
Confirmación de botón
≠
Hallazgo
≠
Error
≠
Inicio
```

---

## Mostrar datos

Las pantallas pueden mostrar:

```text
POSIBLE HALLAZGO
```

además de información como:

```text
Objeto
Confianza
ID
Estado de envío
```

---

# 🧠 25. Comunicación con ATOM.IA

ATOM ArchBot puede comunicar determinados eventos al sistema complementario **ATOM.IA**.

La arquitectura general es:

```text
M-Vision
   ↓
MATRIX Mini R4
   ↓
Evento
   ↓
ATOM.IA
```

Cuando existe una detección se pueden enviar datos como:

```text
Tipo de objeto
ID
Confianza
Estado
Momento de detección
```

ATOM.IA puede utilizar esta información para mostrar el evento en su interfaz.

---

# 📡 26. Estado de comunicación

El sistema puede indicar si ATOM.IA se encuentra disponible.

Ejemplo:

```text
IA: Conectado
```

o:

```text
IA: Desconectado
```

Es importante que una falla de comunicación con la interfaz no produzca movimientos mecánicos inesperados.

El robot debe priorizar siempre el control local y la seguridad.

---

# 🖥️ 27. LCD 16×2

El LCD 16×2 funciona principalmente como interfaz de interacción directa con el usuario.

Puede mostrar mensajes como:

```text
Hola mundo,
soy ATOM-ArchBot
```

```text
Conectando a
ATOM.IA
```

```text
Conectado
```

```text
Comenzar a
harnear
```

```text
Modo
Exhibición
```

```text
Posible
hallazgo
```

Su objetivo es mostrar mensajes sencillos y fáciles de interpretar.

---

# 📊 28. LCD 16×4

El LCD 16×4 se utiliza para mostrar información más técnica.

Durante el harneado puede utilizar una estructura similar a:

```text
Tierra: Alta
Velocidad: ■■■■
San Fernando
IA: Conectado
```

Durante una detección:

```text
Obj: PUNTA
Confianza: 84%
Envio: OK
ID: 2
```

Esto permite observar el comportamiento interno del sistema sin conectarlo a un computador.

---

# 🖥️ 29. OLED integrada

La OLED del MATRIX puede utilizarse para información adicional y diagnóstico.

Entre sus funciones se encuentran:

```text
Pantalla de inicio
Logo ATOM ArchBot
Logo Los Harneritos
Estado del robot
Modo actual
Información técnica
Diagnóstico
Errores
```

Al estar integrada en el controlador, es especialmente útil durante las pruebas del prototipo.

---

# 🌈 30. Sistema RGB

ATOM ArchBot utiliza dos barras RGB WS2812B.

Las barras funcionan como una interfaz visual adicional.

Los colores y animaciones pueden representar diferentes eventos.

Conceptualmente:

| Estado | Uso visual |
|---|---|
| Inicio | Animación de encendido |
| Listo | Estado de espera |
| Harneando | Animación de funcionamiento |
| Detección | Señal especial |
| Error | Alerta |
| Pulsación | Confirmación |
| Modo prueba | Animación correspondiente |

Las dos barras poseen control independiente.

Esto permite crear animaciones simétricas o diferentes para cada lado.

---

# 🔊 31. Buzzer

El buzzer integrado funciona como interfaz sonora.

No todos los eventos deben utilizar el mismo pitido.

Se pueden diferenciar sonidos para:

```text
Encendido
Pulsación
Inicio de harneado
Cambio de velocidad
Movimiento de bandeja
Bandeja insertada
Hallazgo
Error
Conexión
Envío correcto
```

El objetivo es que el usuario pueda reconocer determinadas acciones incluso sin mirar las pantallas.

---

# ⏸️ 32. Pausar el proceso

El Pulsador 2 permite detener temporalmente el proceso.

```text
HARNEANDO
    ↓
Pulsador 2
    ↓
PAUSADO
```

Durante la pausa:

```text
Servos → detenidos

Motores de bandeja → detenidos si corresponde

Estado del proceso → conservado

Pantallas → PAUSADO
```

Al volver a ejecutar la acción correspondiente:

```text
PAUSADO
   ↓
CONTINUAR
   ↓
HARNEANDO
```

Antes de continuar deben mantenerse las condiciones de seguridad necesarias.

---

# 📦 33. Sistema de bandeja

Debajo del harnero existe una bandeja destinada a recibir el material que atraviesa la malla.

La bandeja utiliza dos motores DC.

ATOM puede controlar:

```text
EXTRACCIÓN
INSERCIÓN
DETENCIÓN
```

---

# 📤 34. Sacar la bandeja

La extracción se controla mediante:

```text
Pulsador 3
```

Flujo:

```text
Pulsador 3
    ↓
Comprobar seguridad
    ↓
Activar motores DC
    ↓
SACAR BANDEJA
    ↓
Detener motores
```

En la configuración de pruebas se puede utilizar un tiempo determinado para la extracción.

Por ejemplo, el movimiento puede mantenerse durante aproximadamente:

```text
15 segundos
```

Este tiempo puede ajustarse según la velocidad mecánica real.

---

# 🛡️ 35. No sacar la bandeja con tierra

Otra regla de seguridad importante es evitar sacar la bandeja cuando el robot considera que todavía existe una cantidad importante de tierra en el harnero.

La lógica es:

```text
Pulsador SACAR
      ↓
¿Existe tierra?
    /       \
  SÍ         NO
  │           │
  ▼           ▼
BLOQUEAR     SACAR
```

Esto ayuda a evitar que el contenido caiga fuera de la bandeja.

Si el usuario intenta realizar la acción:

```text
NO SE PUEDE
SACAR BANDEJA
```

puede mostrarse en pantalla junto con una señal sonora.

---

# 📥 36. Insertar la bandeja

La inserción utiliza:

```text
Pulsador 4
```

Secuencia:

```text
Pulsador 4
    ↓
Activar motores
    ↓
INSERTAR
    ↓
Leer final de carrera
    ↓
¿ACTIVADO?
    /    \
  NO      SÍ
  │        │
seguir   detener
```

En este movimiento no se depende únicamente de un tiempo.

El final de carrera proporciona información física de que la bandeja llegó a su posición interior.

---

# 🛑 37. Final de carrera

El final de carrera se encuentra en la zona interior de la bandeja.

Cuando la bandeja llega:

```text
Bandeja
   ↓
Presiona mecanismo
   ↓
Final de carrera
   ↓
MATRIX detecta señal
```

La lógica eléctrica utilizada es:

```text
Sin pulsar → HIGH
Pulsado    → LOW
```

Al detectar `LOW` durante la inserción:

```text
DETENER MOTORES
```

Esto evita que los motores continúen intentando introducir la bandeja después de alcanzar el límite.

---

# 🔧 38. Microempuje de inserción

Después de detectar el final de carrera puede utilizarse, si la calibración mecánica lo requiere, un movimiento muy breve y controlado.

Su objetivo no es forzar el mecanismo.

Su función es asegurar que la bandeja quede completamente asentada cuando la geometría del sistema lo necesite.

Este parámetro debe mantenerse pequeño y ajustarse mediante pruebas.

---

# 📍 39. Estado de la bandeja

El software puede mantener un estado interno aproximado:

```text
BANDEJA DENTRO
BANDEJA FUERA
MOVIÉNDOSE
DESCONOCIDA
```

Este estado permite tomar decisiones.

Ejemplo:

```text
BANDEJA FUERA
+
Usuario intenta HARNEAR
=
BLOQUEAR
```

---

# 🧪 40. Modos de prueba

Durante el desarrollo del robot es necesario poder comprobar subsistemas individualmente sin ejecutar todo el proceso.

Por esta razón ATOM dispone de funciones de prueba.

Estas funciones pueden utilizarse para:

```text
Probar sensores ToF
Ver distancias
Mover servos
Calibrar posición inicial
Probar cámara
Comprobar detecciones
Probar RGB
Probar buzzer
Comprobar pantallas
Ver estado de bandeja
Comprobar comunicaciones
```

Esto permite calibrar el robot antes de ejecutar un ciclo completo.

---

# 📏 41. Prueba de distancia del harnero

Uno de los modos de prueba permite observar directamente las mediciones de los sensores ToF.

Durante esta prueba las pantallas pueden mostrar valores como:

```text
ToF IZQ: 124 mm
ToF DER: 131 mm
```

Esto permite colocar distintas cantidades de tierra y estudiar cómo cambian las lecturas.

Con esos resultados se pueden definir posteriormente los límites de:

```text
Tierra baja
Tierra media
Tierra alta
Sin tierra
```

---

# 🎪 42. Modo Exhibición

ATOM también puede disponer de un modo destinado a demostraciones.

El objetivo del modo Exhibición es mostrar las capacidades del robot en una competencia, exposición o presentación sin necesariamente ejecutar todo el ciclo mecánico normal.

Durante este modo pueden mantenerse activos principalmente:

```text
M-Vision
Pantallas
RGB
Buzzer
Información de detección
ATOM.IA
```

Las funciones mecánicas pueden limitarse según la configuración utilizada durante la demostración.

La prioridad es poder enseñar el funcionamiento de la tecnología de forma segura.

---

# 🔍 43. Detección durante una demostración

En modo de prueba o exhibición se pueden colocar objetos frente a la cámara.

Cuando se detecta uno:

```text
OBJETO
   ↓
M-Vision
   ↓
Confianza
   ↓
MATRIX
   ↓
RGB + sonido
   ↓
Pantallas
   ↓
ATOM.IA
```

Así se puede demostrar la visión artificial sin tener que realizar un harneado completo cada vez.

---

# 🔄 44. Funcionamiento sin bloqueos largos

La programación de ATOM busca evitar utilizar esperas largas que congelen todo el sistema.

En lugar de:

```text
hacer acción
esperar muchos segundos
hacer otra acción
```

se busca trabajar mediante estados y temporizadores.

De esta forma, mientras ocurre una acción, el sistema puede continuar:

```text
Leyendo pulsadores
Actualizando pantallas
Leyendo sensores
Revisando errores
Controlando LEDs
Manteniendo comunicación
```

Esto hace que la respuesta del robot sea más fluida.

---

# 🧠 45. Máquina de estados

El programa puede entenderse como una máquina de estados.

Los estados principales son:

| Estado | Función |
|---|---|
| `INICIALIZANDO` | Preparar hardware |
| `LISTO` | Esperar al usuario |
| `HARNEANDO` | Ejecutar movimiento |
| `PAUSADO` | Detener temporalmente |
| `HALLAZGO` | Proteger posible objeto |
| `ERROR` | Bloquear acciones inseguras |
| `FINALIZADO` | Terminar ciclo |
| `PRUEBA` | Diagnóstico y calibración |
| `EXHIBICION` | Demostración del sistema |

Cada estado permite determinadas acciones y bloquea otras.

---

# 🧩 46. Diagrama de estados

```text
                           ┌────────────────┐
                           │    ENCENDIDO   │
                           └───────┬────────┘
                                   │
                                   ▼
                           ┌────────────────┐
                           │ INICIALIZANDO  │
                           └───────┬────────┘
                                   │
                                   ▼
                           ┌────────────────┐
                           │     LISTO      │
                           └───────┬────────┘
                                   │
                               INICIAR
                                   │
                                   ▼
                    ┌────────────────────────┐
              ┌────▶│       HARNEANDO        │◀─────┐
              │     └──────┬────────┬────────┘      │
              │            │        │               │
              │          PAUSA   HALLAZGO           │
              │            │        │               │
              │            ▼        ▼               │
              │      ┌─────────┐ ┌──────────┐       │
              └──────│ PAUSADO │ │ HALLAZGO │───────┘
                     └─────────┘ └──────────┘

Todos los estados
       │
       ├──── condición crítica ────▶ ERROR
       │
       └──── proceso terminado ────▶ FINALIZADO
```

---

# 🏁 47. Finalización del harneado

Cuando se determina que el ciclo ha terminado:

```text
HARNEANDO
    ↓
Condición de término
    ↓
FINALIZADO
```

El sistema puede realizar:

```text
Detener servos
Detener motores
Actualizar pantallas
Cambiar RGB
Reproducir sonido
Guardar estado
Regresar servos a referencia
```

La pantalla puede indicar:

```text
HARNEADO
FINALIZADO
```

---

# 🛡️ 48. Prioridad de seguridad

En ATOM ArchBot una orden del usuario no necesariamente significa que el robot deba ejecutarla inmediatamente.

Primero se verifica si la acción es segura.

Ejemplo:

```text
ORDEN DEL USUARIO
       ↓
COMPROBAR ESTADO
       ↓
¿SE PUEDE?
   /       \
 NO         SÍ
 │           │
 ▼           ▼
BLOQUEAR   EJECUTAR
```

Esto es especialmente importante para:

```text
Iniciar harneado
Sacar bandeja
Insertar bandeja
Mover servos
Recuperarse de un error
```

---

# 🏺 49. Protección de posibles objetos

La finalidad de combinar sensores y visión artificial es reducir acciones mecánicas innecesarias sobre el material.

La estrategia es:

```text
Mucha tierra
     ↓
Harneado más rápido
     ↓
Disminuye tierra
     ↓
Movimiento más controlado
     ↓
Aparece objeto
     ↓
Visión lo analiza
     ↓
Posible hallazgo
     ↓
DETENER
```

De esta forma el comportamiento puede cambiar durante un mismo ciclo.

---

# 👤 50. Supervisión humana

ATOM ArchBot es un sistema de apoyo.

Cuando informa:

```text
POSIBLE HALLAZGO
```

el siguiente paso corresponde al operador.

Una persona debe:

```text
Detener el proceso si corresponde
Revisar el objeto
Retirarlo cuidadosamente
Evaluarlo
Decidir si continuar
```

La inteligencia artificial no reemplaza la evaluación profesional.

---

# 🔬 51. Calibración experimental

El comportamiento final depende de pruebas reales.

Los principales parámetros que deben calibrarse son:

```text
Posición inicial de servos
Ángulo máximo de movimiento
Velocidad lenta
Velocidad media
Velocidad rápida
Distancia ToF con harnero vacío
Distancia con poca tierra
Distancia con tierra media
Distancia con mucha tierra
Tiempo de extracción
Posición del final de carrera
Microempuje
Umbral de confianza de visión
Número de frames de confirmación
Tiempo de rearme
Iluminación de la cámara
Sensibilidad ante objetos
```

Estos valores no deberían elegirse únicamente de forma teórica.

Deben obtenerse realizando pruebas con el robot completamente armado.

---

# 📊 52. Datos disponibles durante las pruebas

Durante la calibración es útil observar:

```text
ToF izquierdo
ToF derecho
Nivel de tierra
Velocidad actual
Estado del robot
Estado bandeja
Final de carrera
Objeto detectado
ID
Confianza
Comunicación IA
```

Esta información permite saber por qué el robot tomó una decisión.

---

# 🔍 53. Diagnóstico

Una de las ventajas de utilizar varias pantallas es disponer de información de diagnóstico sin conectar siempre un computador.

Si el robot no se comporta como se espera, se puede comprobar:

```text
¿Los sensores están leyendo?

¿La bandeja aparece dentro?

¿La cámara está detectando?

¿El objeto supera la confianza?

¿Los servos están en el nivel correcto?

¿ATOM.IA está conectado?

¿Existe algún estado de error?
```

---

# 🧠 54. Lógica simplificada del programa

Conceptualmente el programa funciona así:

```text
ENCENDER

↓

Inicializar hardware

↓

Posicionar actuadores

↓

Inicializar pantallas

↓

Inicializar cámara

↓

Inicializar sensores

↓

Comprobar estados

↓

Estado = LISTO

↓

REPETIR CONSTANTEMENTE:

    Leer pulsadores

    Leer final de carrera

    Leer sensores ToF

    Actualizar estado de bandeja

    Revisar comunicación

    Actualizar pantallas

    Actualizar RGB

    Revisar cámara


    SI estado == LISTO:

        Esperar INICIAR


    SI usuario pulsa INICIAR:

        Comprobar bandeja

        Comprobar seguridad

        SI condiciones correctas:

            Estado = HARNEANDO


    SI estado == HARNEANDO:

        Medir tierra

        Calcular nivel

        Elegir velocidad

        Mover servos

        Analizar detecciones


        SI existe hallazgo confirmado:

            Detener servos

            Activar alerta

            Mostrar información

            Enviar evento

            Estado = HALLAZGO


        SI usuario pulsa PAUSA:

            Detener servos

            Estado = PAUSADO


    SI usuario solicita SACAR BANDEJA:

        Comprobar tierra

        SI es seguro:

            Activar motores hacia afuera


    SI usuario solicita INSERTAR BANDEJA:

        Activar motores hacia adentro

        Leer final de carrera

        SI final activado:

            Detener motores


    SI existe ERROR:

        Detener actuadores

        Mostrar causa


    SI proceso termina:

        Detener actuadores

        Estado = FINALIZADO
```

---

# 🔗 55. Relación entre todos los sistemas

ATOM ArchBot no funciona como componentes separados.

Todos forman parte de un único sistema.

```text
                    ┌───────────────┐
                    │     TIERRA    │
                    └───────┬───────┘
                            │
                            ▼
                    ┌───────────────┐
                    │ Sensores ToF  │
                    └───────┬───────┘
                            │
                            ▼
                    ┌───────────────┐
                    │ MATRIX Mini R4│
                    └───────┬───────┘
                            │
                            ▼
                    ┌───────────────┐
                    │ Elegir nivel  │
                    │ de velocidad  │
                    └───────┬───────┘
                            │
                            ▼
                    ┌───────────────┐
                    │    SERVOS     │
                    └───────┬───────┘
                            │
                            ▼
                       HARNEADO
                            │
                            ▼
                    ┌───────────────┐
                    │   M-Vision    │
                    └───────┬───────┘
                            │
                    ¿Objeto detectado?
                         /      \
                       NO        SÍ
                       │          │
                       │          ▼
                       │    ┌────────────┐
                       │    │ HALLAZGO   │
                       │    └─────┬──────┘
                       │          │
                       │     ┌────┼────┐
                       │     ▼    ▼    ▼
                       │    RGB Buzzer Pantallas
                       │               │
                       │               ▼
                       │            ATOM.IA
                       │
                       └──── continuar proceso
```

---

# 🏗️ 56. Filosofía de diseño

El desarrollo de ATOM ArchBot se basa en cuatro ideas principales.

## SENSAR

El robot obtiene información de su entorno.

```text
ToF
Cámara
Final de carrera
Pulsadores
```

## ANALIZAR

El MATRIX Mini R4 interpreta la información.

```text
Distancias
Estados
Confianza
Órdenes
Condiciones
```

## ADAPTAR

El comportamiento cambia según la situación.

```text
Cambiar velocidad
Detener
Permitir
Bloquear
Alertar
```

## PROTEGER

Cuando existe una condición que podría poner en riesgo el proceso, ATOM prioriza detenerse.

```text
Posible hallazgo
Sin bandeja
Error
Condición insegura
```

---

# 🏺 ATOM ArchBot V3

### Sistema robótico de apoyo al harneado arqueológico

ATOM ArchBot integra:

```text
ROBÓTICA
    +
SENSORES
    +
VISIÓN ARTIFICIAL
    +
AUTOMATIZACIÓN
    +
SEGURIDAD
    +
INTERFAZ HUMANA
    +
ATOM.IA
```

para desarrollar una plataforma capaz de apoyar el proceso de harneado de una forma más controlada y adaptable.

Su funcionamiento puede resumirse en:

```text
┌─────────┐
│ SENSAR  │
└────┬────┘
     ▼
┌─────────┐
│ANALIZAR │
└────┬────┘
     ▼
┌─────────┐
│ ADAPTAR │
└────┬────┘
     ▼
┌─────────┐
│PROTEGER │
└─────────┘
```

**ATOM ArchBot busca demostrar cómo la robótica y la visión artificial pueden transformarse en herramientas de apoyo para la conservación, investigación y protección del patrimonio arqueológico.**
